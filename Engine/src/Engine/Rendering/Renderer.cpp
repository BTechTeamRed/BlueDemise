#pragma once
#include "glad/glad.h"

#include <glm/gtc/type_ptr.hpp>

#include "Renderer.h"
#include "Engine/SceneBuilder/Scene.h"
#include "Engine/SceneBuilder/Entity.h"
#include "Engine/SceneBuilder/InputSystem.h"

#include "Engine/ResourceManagement/ResourceManager.h"
#include "Engine/ResourceManagement/ShaderNorms.h"

#include "Engine/Utilities/Log.h"


namespace Engine
{
	Renderer* Renderer::m_pinstance{ nullptr };
	
	//When constructing a renderer, initialize the window and the renderer
	//When a scene is swapped, we need to change the camera. Renderer should be treated as a singleton
	Renderer::Renderer()
	{
		//Initialize GLFW
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		//Initialize the window and associated functions, and make the window the current context. If fails, close the program.
		if(!m_window.initialize())
		{
			GE_CORE_ERROR("[Renderer] Failed to initialize window");
			glfwTerminate();
			exit(0);
		}

		//Adds the callback to the inputsystem for when the window is resized
		if (!m_showUI) 
		{
			InputSystem::getInstance()->setResizeCallback([&](int x, int y) { m_window.resize(x, y); });
		}
		else 
		{
			InputSystem::getInstance()->setResizeCallback([&](int x, int y) { /*Resize Handled by GamePanel*/ });
		}
		
		//Setting the icon
		ResourceManager::getInstance()->setAppIcon((std::string)"BlueDemiseIcon.png", m_window.getWindow());

		//Initialize GLAD. Close program if fails.
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			GE_CORE_ERROR("[Renderer] Failed to initialize GLAD");
			glfwTerminate();

			throw std::runtime_error("Failed to initialize GLAD");
			
			exit(0);
		}

		//Initialize the frame buffer
		m_window.initFrameBuffer();
		
		glfwSwapInterval(1);

		//Enable transparency
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Sets the color of the 'clear' command. This is a dark grey
		glClearColor(1.f, 0.1f, 0.1f, 1);

		m_text.initializeText();
	}

	//Return the instance of resource manager. If one does not exist, create it, and return the pointer.
	Renderer* Renderer::getInstance()
	{
		if (m_pinstance == nullptr)
		{
			m_pinstance = new Renderer();
		}

		return m_pinstance;
	}

	void Renderer::updateUIHeiraarchy(std::string tag, Entity entity)
	{
		m_UI.updateHierarchyPanel(tag, entity);
	}

	glm::vec3 Renderer::screenToWorld(const glm::vec2& screenPosition)
	{
		return m_window.screenSpaceToWorldSpace(screenPosition, glm::vec2());
	}

	glm::vec2 Renderer::worldToScreen(const glm::vec3& worldPosition)
	{
		return m_window.worldSpaceToScreenSpace(worldPosition, glm::vec2());
	}

#pragma region Scene Management & Rendering
	//Initialize the Scene. Updates m_window's camera
	void Renderer::initializeScene(Scene& scene)
	{
		//Initialize input system.
		InputSystem::getInstance()->init(m_window.getWindow());

		//initialize the window for UI
		if (m_showUI)
		{
			if (!m_UI.initializeUI(m_window, scene))
				GE_CORE_WARN("[Renderer] UI not initialized properly.");
		}
		else {
			glEnable(GL_DEPTH_TEST);
		}

		//Create default shader.
		loadShaders();

		GLint prog = 0;
		glGetIntegerv(GL_CURRENT_PROGRAM, &prog);

		//Update m_window camera.
		auto cameraView = scene.getEntities<CameraComponent>();
		const auto camera = scene.m_registry.get<CameraComponent>(cameraView.back());
		m_window.updateCamera(camera);

		//Temp code, initialize animations for testing
		auto animations = scene.getEntities<AnimationComponent>();
		for (auto& [entity, anim] : animations.each()) {
			anim.animationClip = animationSystem.createAnimationClip(AnimationSystem::RT_LoopAll, anim);
			animationSystem.changeFrame(0, anim);
		}
	}

	//For when the scene must be stopped, perform cleanup
	void Renderer::stopScene(Scene& scene) 
	{
		glfwTerminate();
		
		glDeleteProgram(m_programId);
	}

	//From scene, we should grab all the entities within that scene object and render each one.
	void Renderer::renderScene(const DeltaTime& dt, Scene& scene)
	{
		//Bind the FBO for draw calls to render to.
		if (m_showUI)
		{
			m_window.bindFrameBuffer();
		}

		//Update window camera.
		auto cameraView = scene.getEntities<CameraComponent>();
		m_window.updateCamera(scene.m_registry.get<CameraComponent>(cameraView.back()));

		//Update all animations
		auto animations = scene.getEntities<AnimationComponent>();
		for (auto& [entity, anim] : animations.each()) {
			animationSystem.updateAnimation(dt, anim);
		}

		//Update Lerps
		auto lerps = scene.getEntities<PositionLerpComponent, TransformComponent>();
		for (auto& [entity, lerp, trans] : lerps.each()) {
			if(animationSystem.updatePositionLerps(dt, lerp, trans)) scene.m_registry.remove<PositionLerpComponent>(entity);
		}


		//Render all entities with vertices, and associated components.
		drawEntities(scene);

		

		//Unbind the FBO.
		if (m_showUI) 
		{
			m_window.unBindFrameBuffer();
		}
		else {
			glClear(GL_DEPTH_BUFFER_BIT);
		}

		//UI window
		if (m_showUI) 
		{
			m_UI.renderUI(scene, m_window);
		}
		//else
		//{
		//	//Draw the FBO to the screen.
		//	glBindFramebuffer(GL_FRAMEBUFFER, m_window.m_fboTextureID);
		//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		//}

		//Swap the glfw buffers.
		glfwSwapBuffers(m_window.getWindow());


		//Check if the 'X' was hit on the window. If so, close the program
		if (glfwWindowShouldClose(m_window.getWindow())) 
		{
			scene.m_closeScene = true;
		}
	}

	//Draw all Entities within scene that contain a vertices component.
	void Renderer::drawEntities(Scene& scene)
	{
		//https://skypjack.github.io/entt/md_docs_md_entity.html, "Sorting: is it Possible", "A full-owning group is the fastest tool a user can expect to use to iterate multiple components at once"
		//
		//Get entities that contain vertices component.
		auto renderables = scene.m_registry.group<VerticesComponent, TransformComponent>();
		renderables.sort<TransformComponent>([](const auto& lhs, const auto& rhs)
		{
			return lhs.position.z < rhs.position.z;
		});
		

		//Set the currentBound textures to 0 for this draw call.
		int currentBoundTextures = 0;

		glUseProgram(m_programId);

		//For each entitiy with a vertices component, render it.
		for (auto entity : renderables)
		{
			auto& vertices = renderables.get<VerticesComponent>(entity); 
			auto& transform = renderables.get<TransformComponent>(entity);


			//Set a default transform component and color if the object does not contain one.
			//TransformComponent transform;
			glm::vec4 color = m_defaultColor;

			//Change the transform component if the entity contains one.
			//if (scene.m_registry.all_of<TransformComponent>(entity)) {transform = scene.m_registry.get<const TransformComponent>(entity);}

			//Obtain MVP using transform and window's projection matrix.
			const glm::mat4 mvp = updateMVP(transform, m_window.getProjectionMatrix());

			//access to the advanced shader and tile bind if applicable
			int advancedShaderBind = -1;
			
			//Bind color, texture and shader if entity contains material.
			if (scene.m_registry.all_of<MaterialComponent>(entity))
			{
				const auto material = scene.m_registry.get<const MaterialComponent>(entity);
				advancedShaderBind = material.bind;
				
				if (setTexture(material.texID, currentBoundTextures)){currentBoundTextures++;}
			
				//Change color and shaderProgram to material components color and shader.
				color = material.color;
				//glUseProgram(material.shaderID);
			}

			//updates the shader based on vertices component's stride value and/or advanced shader
			ShaderNorms::getInstance()->update(glfwGetTime(), advancedShaderBind, vertices.stride,
				m_textureCoordinates, m_colorCoordinates, m_gradientCoordinates, m_programId);

			//Apply animation uniform if necessary
			if (scene.m_registry.all_of<AnimationComponent>(entity))
			{
				//Bind animation shader
				const auto anim = scene.m_registry.get<const AnimationComponent>(entity);
				//Update uvMatrix uniform
				GLuint uvUniformID = glGetUniformLocation(m_programId, "uvMatrix");
				glUniformMatrix3fv(uvUniformID, 1, false, glm::value_ptr(anim.uvTransformMatrix));
				auto s = scene.m_registry.get<const TagComponent>(entity).tag;
			}
			else {
				//Ignore UV transformation by setting uvMatrix to an identity matrix
				GLuint uvUniformID = glGetUniformLocation(m_programId, "uvMatrix");
				glUniformMatrix3fv(uvUniformID, 1, false, glm::value_ptr(glm::mat3(1.f)));
				auto s = scene.m_registry.get<const TagComponent>(entity).tag;

			}
			
			//Set the color of the object
			setColor(mvp, color, m_programId);

			//VAO is container for VBO, and is bound to ensure correct vertices are draw.
			glBindVertexArray(vertices.vaoID);
			
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertices.iboID);

			//Draw currently bound objects.
			glDrawElements(GL_TRIANGLES, vertices.numIndices, GL_UNSIGNED_INT, nullptr);
		}
		
		//Get entities that contain text component.
		const auto textRenderables = scene.getEntities<const TextComponent>();
		
		glUseProgram(m_text.m_textShaderProgram);
		GLuint mvpID = glGetUniformLocation(m_text.m_textShaderProgram, "mvp");
		
		for (auto [entity, text] : textRenderables.each())
		{
			//Set a default transform component and color if the object does not contain one.
			TransformComponent textTransform;
			glm::vec4 color = m_defaultColor;

			//Change the transform component if the entity contains one.
			if (scene.m_registry.all_of<TransformComponent>(entity)) { textTransform = scene.m_registry.get<const TransformComponent>(entity); }

			//Bind color, texture and shader if entity contains material.
			if (scene.m_registry.all_of<MaterialComponent>(entity))
			{
				const auto material = scene.m_registry.get<const MaterialComponent>(entity);

				if (setTexture(material.texID, currentBoundTextures)) { currentBoundTextures++; }

				//Change color and shaderProgram to material components color and shader.
				color = material.color;
			}

			std::string render = std::to_string(scene.score);

			renderText(text, textTransform, color, m_text.m_textShaderProgram, mvpID);
		}
	}
#pragma endregion
	
#pragma region Renderable Handling
	//Bind the provided textureID, and return true if successfully bound
	bool Renderer::setTexture(GLuint textureID, int currentBoundTextures)
	{
		if (textureID == 0){ return false; }
		
		//glActiveTexture(GL_TEXTURE0 + currentBoundTextures);
		glBindTexture(GL_TEXTURE_2D, textureID);
		return true;
	}
	
	//Set the color of the current drawable object using the current shader. This would need to be run per entity/renderable.
	void Renderer::setColor(glm::mat4 mvp, glm::vec4 color, GLuint shaderID)
	{
		GLuint colorUniformID = glGetUniformLocation(shaderID, "col");
		GLuint mvpID = glGetUniformLocation(shaderID, "mvp");

		//Sets color of shader
		glUniform4fv(colorUniformID, 1, glm::value_ptr(color));
		glUniformMatrix4fv(mvpID, 1, GL_FALSE, glm::value_ptr(mvp));
	}

	//Load default shader to 'fill' object. This should be changed when multiple shaders are utilized *******************
	void Renderer::loadShaders()
	{
		ShaderNorms::getInstance()->assignsNewStride(m_textureCoordinates);
		m_programId = ShaderNorms::getInstance()->getShaderReference();
		glUseProgram(m_programId);
	}

	//Render a string line of text based on the transform location. Based on https://learnopengl.com/In-Practice/Text-Rendering
	void Renderer::renderText(const TextComponent& text, const TransformComponent& transform, const glm::vec3& color, GLuint shader, GLuint mvpID)
	{
		TransformComponent charPos = transform;

		//Access the current shader data/program and pass the color XYZ to the textColor variable in the shader.
		glUniform3f(glGetUniformLocation(shader, "textColor"), color.x, color.y, color.z);

		std::string printLine = text.text;


		glBindVertexArray(m_text.m_textVertices.vaoID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_text.m_textVertices.iboID);
		glBindBuffer(GL_ARRAY_BUFFER, m_text.m_textVertices.vboID);

		// iterate through all characters in printLine, adjusting the scale and position for each character before rendering the quad.
		Text::Character ch;
		for (char c : printLine)
		{
			ch = m_text.Characters[c];

			if (c != ' ')
			{
				//Adjust the scale of the charPos transform to reflect the scale of text.
				charPos.scale.x = ((charPos.position.x + ch.Size.x) - (charPos.position.x + (ch.Size.x / 2)));
				charPos.scale.y = ((charPos.position.y + ch.Size.y) - (charPos.position.y));

				//Adjust the charPos transform to the correct position for the character.
				charPos.position.x += ch.Bearing.x;
				charPos.position.y += (((ch.maxAscent + ch.maxDescent) - ch.bitmap_top) + ((ch.Size.y - ch.Bearing.y) / (charPos.scale.y)));

				//Obtain MVP using transform and window's projection matrix.
				const glm::mat4 mvp = updateMVP(charPos, m_window.getProjectionMatrix());
				glUniformMatrix4fv(mvpID, 1, GL_FALSE, glm::value_ptr(mvp));

				// render glyph texture over quad
				glBindTexture(GL_TEXTURE_2D, ch.TextureID);
				
				// render quad
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

				// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
				charPos.position.x += (ch.Size.x/2.f); //(ch.Advance >> 6) // bitshift by 6 to get value in pixels (2^6 = 64). Unused for now
				charPos.position.y = transform.position.y;
			}
			else
			{
				charPos.position.x += m_text.m_whiteSpaceSize/3.f;
			}
		}

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//Update an MVP matrix, with the MVP generated in the function and returned.
	glm::mat4 Renderer::updateMVP(TransformComponent transform, glm::mat4 projection)
	{
		//Setup model view matrix
		glm::mat4 mvm = glm::mat4(1.f);
		glm::vec3 toCenter(transform.scale.x / 2.f, transform.scale.y / 2.f, 0.f);
		mvm = glm::translate(mvm, transform.position);
		mvm = glm::translate(mvm, toCenter);
		mvm = glm::rotate(mvm, transform.rotation.x, glm::vec3(1, 0, 0));
		mvm = glm::rotate(mvm, transform.rotation.y, glm::vec3(0, 1, 0));
		mvm = glm::rotate(mvm, transform.rotation.z, glm::vec3(0, 0, 1));
		mvm = glm::translate(mvm, -toCenter);
		mvm = glm::scale(mvm, transform.scale);

		//Calculate MVP
		glm::mat4 mvp = projection * mvm;

		return mvp;
	}
#pragma endregion
	
}