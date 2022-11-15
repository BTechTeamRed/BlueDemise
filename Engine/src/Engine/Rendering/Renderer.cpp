#include "Renderer.h"
#include "Engine/SceneBuilder/Scene.h"
#include "Engine/SceneBuilder/Components.h"
#include "Engine/SceneBuilder/Entity.h"
#include <glm/gtc/type_ptr.hpp>
#include "glad/glad.h"


namespace Engine
{
	//From scene, we should grab all the entities within that scene object and render each one.
	void Renderer::renderScene(const DeltaTime& dt, Scene& scene)
	{
		glClear(GL_COLOR_BUFFER_BIT);

		auto cameraView = scene.getEntities<const CameraComponent>();
		const auto camera = scene.m_registry.get<CameraComponent>(cameraView.back());
		projectionMatrix = glm::ortho(0.f, camera.viewport.x, camera.viewport.y, 0.f, camera.nearZ, camera.farZ);
		viewMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -10.f)); //position of camera in world-space

		//Render all entities

		//Render all entities with vertices, and associated components.
		prepareEntities(scene);
	}

	void prepareEntities(Scene& scene)
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
			//const glm::mat4 mvp = updateMVP(transform, m_projectionMatrix, m_viewMatrix);
			
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
	glm::mat4 Scene::updateMVP(TransformComponent transform, glm::mat4 view, glm::mat4 projection)
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

	
	
	/*
	//Insitialize OpenGL, returning true if successful. Window based on GLFW.
	bool Scene::initializeGL()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, "BlueDemise", nullptr, nullptr); //switch to unique ptr with deleter for RAII?
		if (m_window == nullptr)
		{
			GE_CORE_ERROR("Failed to create GLFW window");
			glfwTerminate();
			return false;
		}

		m_UIwindow = glfwCreateWindow(m_windowWidth, m_windowHeight, "User Interface", nullptr, nullptr); //switch to unique ptr with deleter for RAII?
		if (m_UIwindow == nullptr)
		{
			GE_CORE_ERROR("Failed to create GLFW window (UI)");
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(m_window);



		//Setup a callback to update the viewport size when the window is resized
		//glfwSetWindowUserPointer(m_window, reinterpret_cast<void*>(this));
		//glfwSetWindowSizeCallback(m_window, windowResizeCallback);

		//Setting the icon
		ResourceManager::getInstance()->setAppIcon(*m_window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			GE_CORE_ERROR("Failed to initialize GLAD");
			glfwTerminate();
			return false;
		}

		loadShaders();

		glfwSwapInterval(1);
		glClearColor(0.1f, 0.1f, 0.1f, 1);

		return true;
	}

	//Callback to update window size when it changes
	//TODO: Handle screen resizing
	void windowResizeCallback(GLFWwindow* window, int width, int height)
	{
		/*Scene* scene = reinterpret_cast<Scene*>(glfwGetWindowUserPointer(window));
		auto cameraView = scene->getEntities<const CameraComponent>();
		auto &camera = scene->m_registry.get<CameraComponent>(cameraView.back());
		camera.viewport.x = width;
		camera.viewport.y = height;
	}

	//clears the window and renders all entities that need to be rendered (those with transform, vertices, color).

	//loads and generates shaders to be used in scene. Replace with shader wrappers as per the .h todo.
	void Scene::loadShaders()
	{

		std::string vertexData = ResourceManager::getInstance()->getShaderData("Fill.vs");
		std::string fragmentData = ResourceManager::getInstance()->getShaderData("Fill.fs");

		ShaderGenerator shaderGenerator(vertexData.c_str(), fragmentData.c_str());

		m_programId = shaderGenerator.getProgramId();
		glUseProgram(m_programId);
	}


	#pragma region Renderable Entities
	

	//Placeholder functio, can be replaced by serialized objects.
	VerticesComponent Scene::createSprite()
	{
		VerticesComponent vc;
		vc.vertexAttributes.push_back(VertexAttribute(0, 3, GL_FLOAT, GL_FALSE, 0));
		vc.vertexAttributes.push_back(VertexAttribute(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 3));
		vc.stride = sizeof(float) * m_quadTexCoordinates;
		vc.numIndices = m_quadIndices;

		vc.vaoID = getVAO();
		vc.vboID = getVBO();
		vc.iboID = getIBO();

		setupVertexAttribPtr(vc);

		return vc;
	}

	//Placeholder function, can be replaced by serialized objects.
	VerticesComponent Scene::createRectangle()
	{
		VerticesComponent vc;
		vc.vertexAttributes.push_back(VertexAttribute(0, 3, GL_FLOAT, GL_FALSE, 0));
		vc.stride = sizeof(float) * m_quadTexCoordinates;
		vc.numIndices = m_quadIndices;

		vc.vaoID = getVAO();
		vc.vboID = getVBO(RT_Rectangle);
		vc.iboID = getIBO();

		setupVertexAttribPtr(vc);

		return vc;
	}

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
	}

	#pragma endregion
	*/
}