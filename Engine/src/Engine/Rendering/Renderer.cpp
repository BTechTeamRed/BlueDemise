#include "Renderer.h"
#include "Engine/SceneBuilder/Scene.h"
#include "Engine/SceneBuilder/Components.h"
#include "Engine/SceneBuilder/Entity.h"
#include <Engine/SceneBuilder/InputSystem.h>
#include <glm/gtc/type_ptr.hpp>
#include "glad/glad.h"
#include "Engine/Utilities/Log.h"


namespace Engine
{

	//When constructing a renderer, initialize the window and the renderer
	//When a scene is swapped, we need to change the camera. Renderer should be treated as a singleton
	Renderer::Renderer()
	{
		//Adds the callback to the inputsystem for when the window is resized
		InputSystem::getInstance()->setResizeCallback([&](int x, int y) {m_window.resize(x, y); });

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_window.initialize();
		
		//Setting the icon
		//ResourceManager::getInstance()->setAppIcon(*m_window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			GE_CORE_ERROR("Failed to initialize GLAD");
			glfwTerminate();

			throw std::runtime_error("Failed to initialize GLAD");
		}
		
		glfwSwapInterval(1);

		//Sets the color of the 'clear' command. This is a dark grey
		glClearColor(0.1f, 0.1f, 0.1f, 1);
	}

	//Initialize the Scene. Assigns camera to window for 
	void Renderer::initializeScene(Scene& scene)
	{
		auto cameraView = scene.getEntities<CameraComponent>();
		const auto camera = scene.m_registry.get<CameraComponent>(cameraView.back());

		m_window.m_camera = camera;
	}

	//From scene, we should grab all the entities within that scene object and render each one.
	void Renderer::renderScene(const DeltaTime& dt, Scene& scene)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		
		//Render all entities with vertices, and associated components.
		drawEntities(scene);
	}

	void Renderer::drawEntities(Scene& scene)
	{
		
		//Get entities that contain transform & vertices & color components,
		const auto renderables = scene.getEntities<const VerticesComponent>();

		int currentBoundTextures = 0;
		
		for (auto [entity, vertices] : renderables.each())
		{
			TransformComponent transform = TransformComponent();
			GLuint shaderProgram;
			glm::vec4 color{0.f,0.f,0.f,0.f};

			//Bind Texture
			if (scene.m_registry.all_of<TransformComponent>(entity))
			{
				const auto transform = scene.m_registry.get<const TransformComponent>(entity);
				
			}
			
			//Obtain MVP from Window class
			const glm::mat4 mvp = updateMVP(transform, m_window.getViewMatrix(), m_window.getProjectionMatrix());
			
			if (scene.m_registry.all_of<MaterialComponent>(entity))
			{
				const auto material = scene.m_registry.get<const MaterialComponent>(entity);
				
				if (material.texID != 0)
				{
					//glActiveTexture(GL_TEXTURE0 + currentBoundTextures); // Texture unit X
					//GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS is max number of binded texts(?)
					glBindTexture(GL_TEXTURE_2D, material.texID);
					currentBoundTextures++;
				}
			
				//Change color and shaderProgram to material components color and shader.
				color = material.color;
				shaderProgram = material.shaderID;
			}
			
			//Set the color of the object
			setColor(mvp, color);

			//Bind shader
			glUseProgram(shaderProgram);


			glBindBuffer(GL_ARRAY_BUFFER, vertices.vboID);
			
			glBindVertexArray(vertices.vaoID);

			
			glDrawElements(GL_TRIANGLES, vertices.numIndices, GL_UNSIGNED_INT, nullptr);
		}
	}

	//Set the color of the current drawable object. This would need to be run per entity/renderable.
	void Renderer::setColor(glm::mat4 mvp, glm::vec4 color)
	{
		GLuint colorUniformID = glGetUniformLocation(m_programId, "col");
		GLuint mvpID = glGetUniformLocation(m_programId, "mvp");

		//Sets color of shader
		glUniform4fv(colorUniformID, 1, glm::value_ptr(color));
		glUniformMatrix4fv(mvpID, 1, GL_FALSE, glm::value_ptr(mvp));
	}
	
	//Update an MVP matrix, with the MVP generated in the function and returned.
	glm::mat4 Renderer::updateMVP(TransformComponent transform, glm::mat4 view, glm::mat4 projection)
	{
		//Setup model view matrix
		glm::mat4 mvm = glm::mat4(1.f);
		mvm = glm::translate(mvm, transform.position);
		mvm = glm::rotate(mvm, transform.rotation.x, glm::vec3(1, 0, 0));
		mvm = glm::rotate(mvm, transform.rotation.y, glm::vec3(0, 1, 0));
		mvm = glm::rotate(mvm, transform.rotation.z, glm::vec3(0, 0, 1));
		mvm = glm::scale(mvm, transform.scale);

		//Calculate MVP
		glm::mat4 mvp = projection * view * mvm;

		return mvp;
	}

	//clears the window and renders all entities that need to be rendered (those with transform, vertices, color).
	/*
	//Define vertex attributes
	void Scene::setupVertexAttribPtr(VerticesComponent& vc)
	{
		for (int i = 0; i < vc.vertexAttributes.size(); i++)
		{
			const auto attribute = vc.vertexAttributes[i];
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(attribute.index, attribute.size, attribute.type, attribute.normalized, vc.stride, (const void*)attribute.pointer);
		}

		glBindVertexArray(0);
	}*/

	#pragma endregion
	
}