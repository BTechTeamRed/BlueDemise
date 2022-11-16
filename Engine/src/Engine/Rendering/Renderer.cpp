#include "glad/glad.h"

#include <glm/gtc/type_ptr.hpp>

#include "Renderer.h"
#include "Engine/SceneBuilder/Scene.h"
#include "Engine/SceneBuilder/Entity.h"
#include "Engine/SceneBuilder/InputSystem.h"

#include "Engine/ResourceManagement/ResourceManager.h"
#include "Engine/ResourceManagement/ShaderGenerator.h"

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
		
		
		//Initialize the window and associated functions, and make the window the current context
		if(!m_window.initialize())
		{
			GE_CORE_ERROR("Failed to initialize window");
			glfwTerminate();
			exit(0);
		}

		//Adds the callback to the inputsystem for when the window is resized
		InputSystem::getInstance()->setResizeCallback([&](int x, int y) {m_window.resize(x, y); });
		
		//Setting the icon
		//ResourceManager::getInstance()->setAppIcon(*m_window);

		
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			GE_CORE_ERROR("Failed to initialize GLAD");
			glfwTerminate();

			throw std::runtime_error("Failed to initialize GLAD");
			
			exit(0);
		}
		
		glfwSwapInterval(1);

		//Sets the color of the 'clear' command. This is a dark grey
		glClearColor(0.1f, 0.1f, 0.1f, 1);
	}

	//Return the instance of resource manager. If one does not exist, create it, and return the pointer.
	Renderer* Renderer::getInstance()
	{
		//To ensure this is thread safe, lock this function until it returns a value.
		//std::lock_guard<std::mutex> lock(m_mutex);

		if (m_pinstance == nullptr)
		{
			m_pinstance = new Renderer();
		}

		return m_pinstance;
	}

	//Initialize the Scene. Assigns camera to window for 
	void Renderer::initializeScene(Scene& scene)
	{
		InputSystem::getInstance()->init(m_window.getWindow());
		
		auto cameraView = scene.getEntities<CameraComponent>();
		const auto camera = scene.m_registry.get<CameraComponent>(cameraView.back());

		m_window.m_camera = camera;
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
		glClear(GL_COLOR_BUFFER_BIT);
		
		//Render all entities with vertices, and associated components.
		drawEntities(scene);

		if (glfwWindowShouldClose(m_window.getWindow())) 
		{
			scene.m_closeScene = false;
		}
	}

	void Renderer::drawEntities(Scene& scene)
	{
		
		//Get entities that contain transform & vertices & color components,
		const auto renderables = scene.getEntities<const VerticesComponent>();

		int currentBoundTextures = 0;
		
		for (auto [entity, vertices] : renderables.each())
		{
			TransformComponent transform{ glm::vec3 {0.f,0.f,0.f}, glm::vec3 {0.f,0.f,0.f} , glm::vec3 {0.f,0.f,0.f} };
			GLuint shaderProgram = loadShaders();
			glm::vec4 color{0.f,0.f,0.f,0.f};

			//Bind Texture
			if (scene.m_registry.all_of<TransformComponent>(entity))
			{
				const auto transform = scene.m_registry.get<const TransformComponent>(entity);
			}
			
			//Obtain MVP from Window class
			const glm::mat4 mvp = updateMVP(transform, m_window.getProjectionMatrix());
			
			if (scene.m_registry.all_of<MaterialComponent>(entity))
			{
				const auto material = scene.m_registry.get<const MaterialComponent>(entity);
				
				if (setTexture(material.texID, currentBoundTextures)){currentBoundTextures++;}
			
				//Change color and shaderProgram to material components color and shader.
				color = material.color;
				shaderProgram = material.shaderID;
			}
			
			//Set the color of the object
			setColor(mvp, color);

			//Bind shader
			glUseProgram(shaderProgram);

			glBindBuffer(GL_ARRAY_BUFFER, vertices.vboID);
			
			//glBindVertexArray(vertices.vaoID);
			
			glDrawElements(GL_TRIANGLES, vertices.numIndices, GL_UNSIGNED_INT, nullptr);
		}
	}

	//Bind the texture to texture ID, and perform any other checks and binding 
	bool Renderer::setTexture(GLuint textureID, int currentBoundTextures)
	{
		if (textureID == 0){ return false; }
		
		glActiveTexture(GL_TEXTURE0 + currentBoundTextures);
		glBindTexture(GL_TEXTURE_2D, textureID);
		return true;
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
	
	GLuint Renderer::loadShaders()
	{

		std::string vertexData = ResourceManager::getInstance()->getShaderData("Fill.vs");
		std::string fragmentData = ResourceManager::getInstance()->getShaderData("Fill.fs");

		ShaderGenerator shaderGenerator(vertexData.c_str(), fragmentData.c_str());

		return shaderGenerator.getProgramId();
	}


	
	//Update an MVP matrix, with the MVP generated in the function and returned.
	glm::mat4 Renderer::updateMVP(TransformComponent transform, glm::mat4 projection)
	{
		//Setup model view matrix
		glm::mat4 mvm = glm::mat4(1.f);
		mvm = glm::translate(mvm, transform.position);
		mvm = glm::rotate(mvm, transform.rotation.x, glm::vec3(1, 0, 0));
		mvm = glm::rotate(mvm, transform.rotation.y, glm::vec3(0, 1, 0));
		mvm = glm::rotate(mvm, transform.rotation.z, glm::vec3(0, 0, 1));
		mvm = glm::scale(mvm, transform.scale);

		//Calculate MVP
		glm::mat4 mvp = projection * mvm;

		return mvp;
	}
	#pragma endregion
	
}