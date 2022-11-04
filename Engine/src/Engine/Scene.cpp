#include "glad/glad.h"
#include "Scene.h"

#include <glm/gtc/type_ptr.hpp>

#include "Log.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

#include "Entity.h"
#include "ShaderGenerator.h"
#include "Components.h"
#include "DeltaTime.h"
#include "Scripts/ScriptableBehavior.h"
#include "ResourceManager.h"

namespace Engine
{
	
#pragma region Runtime Functions
	void Scene::onRuntimeStart()
	{
		if (!initializeGL()) return;
		loadShaders();

		glfwSwapInterval(1);
		glClearColor(0.1f, 0.1f, 0.1f, 1);

		//initialize the window for UI
		if (!initializeUI()) return;

		createEntities();

		while (!glfwWindowShouldClose(m_window))
		{
			m_deltaTime.updateDeltaTime();
			onRuntimeUpdate(m_deltaTime);
		}

		onRuntimeStop();
	}

	void Scene::onRuntimeStop()
	{
		glfwTerminate();
	
		const auto view = getEntities<const VerticesComponent>();
		for (auto [entity, vertices] : view.each())
		{
			glDeleteVertexArrays(1, &vertices.vaoID);
			glDeleteBuffers(1, &vertices.vboID);
		}
		glDeleteProgram(m_programId);
	}

	void Scene::onRuntimeUpdate(const DeltaTime& dt)
	{
		//get a view on entities with a script Component, and execute their onUpdate.
		const auto entities = getEntities<ScriptComponent>();
		for (auto [entity, script] : entities.each())
		{
			if (script.m_instance->m_enabled) script.m_instance->onUpdate(dt);//don't update if entity is disabled
		}		
		
		//We have two distinct windows to manage now - one for the main scene and the UI window
		//For each window we change the GL context, render to the window, then swap buffers

		//Main window
		glfwMakeContextCurrent(m_window);
		renderScene(dt);
		glfwSwapBuffers(m_window);
		glfwPollEvents();

		//UI window
		glfwMakeContextCurrent(m_UIwindow);
		renderUI();
		glfwSwapBuffers(m_UIwindow);

		//Execute onLateUpdate().
		for (auto [entity, script] : entities.each())
		{
			if (script.m_instance->m_enabled) script.m_instance->onLateUpdate(dt);//don't update if entity is disabled
		}

		glfwPollEvents();
	}
#pragma endregion

#pragma region OpenGL Scene management
		
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

		//Setting the icon
		ResourceManager::getInstance()->setAppIcon(*m_window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			GE_CORE_ERROR("Failed to initialize GLAD");
			glfwTerminate();
			return false;
		}

		return true;
    }

	bool Scene::initializeUI()
	{
		//Initialize the UI using ImGui OpenGL v3.3
		if (!UserInterface::initialize(m_window))
		{
			return false;
		}

		const int menuHeight = 18;

		m_explorerPanel.setPosition(glm::uvec2(0, menuHeight));
		m_explorerPanel.setDimension(glm::uvec2(m_windowWidth * 0.2f, m_windowHeight - menuHeight));

		m_entitiesPanel.setPosition(glm::uvec2(m_windowWidth - (m_windowWidth * 0.2f), menuHeight));
		m_entitiesPanel.setDimension(glm::uvec2(m_windowWidth * 0.2f, 0.5f * (m_windowHeight - menuHeight)));

		for (int i = 0; i < m_componentsPanels.size(); i++)
		{
			m_componentsPanels[i].setPosition(glm::uvec2(m_windowWidth * 0.2f + (i * m_windowWidth * 0.2f), m_windowHeight - (m_windowHeight * 0.25f)));
			m_componentsPanels[i].setDimension(glm::uvec2(m_windowWidth * 0.2f, m_windowHeight * 0.25f));
		}

		return true;
	}

	void Scene::shutdownUI()
	{
		UserInterface::shutdown();
	}

	//clears the window and renders all entities that need to be rendered (those with transform, vertices, color).
	void Scene::renderScene(const DeltaTime& dt)
	{
		//Added to put triangle view in top/middle of screen
		glClearColor(0.1f, 0.1f, 0.1f, 1);
		glScissor(m_windowWidth * 0.2f, m_windowHeight * 0.25f, m_windowWidth * 0.6f, (m_windowHeight * 0.75f) - 18);
		glViewport(m_windowWidth * 0.2f, m_windowHeight * 0.25f, m_windowWidth * 0.6f, (m_windowHeight * 0.75f) - 18);

		glClear(GL_COLOR_BUFFER_BIT);
		
		
		auto cameraView = getEntities<const CameraComponent>();
		const auto camera = m_registry.get<CameraComponent>(cameraView.back());
		glm::mat4 pm = glm::ortho(-camera.viewport.x / 2, camera.viewport.x / 2,
			-camera.viewport.y / 2, camera.viewport.y / 2, camera.nearZ, camera.farZ);
		glm::mat4 vm = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -10.f)); //position of camera in world-space


		//Render all entities
		//Get entities that contain transform & vertices & color components,
		auto solidObj = getEntities<const TransformComponent, const VerticesComponent, const ColorComponent>();

		//For each updatable solidObj entity (with transform, vertices, and color components), draw them.
		for (auto [entity, transform, vertices, color] : solidObj.each())
		{
			//Update the MVP
			glm::mat4 mvp = updateMVP(transform, vm, pm);

			//Set the color of the object
			setColor(mvp, color.color);

			glDrawElements(GL_TRIANGLES, vertices.numIndices, GL_UNSIGNED_INT, nullptr);
		}


		//Get entities that contain transform & vertices & texture components,
		auto sprites = getEntities<const TransformComponent, const VerticesComponent, const TextureComponent, const ColorComponent>();
		
		//For each updatable sprite entity (with transform, vertices, and color components), draw them.
		for (auto [entity, transform, vertices, texture, color] : sprites.each())
		{
			//Get GLuint for texture, and bind texture for rendering
			glBindTexture(GL_TEXTURE_2D, texture.texID);

			//Update the mvp
			glm::mat4 mvp = updateMVP(transform, vm, pm);

			//Set the color of the sprite
			setColor(mvp, color.color);
			
			glDrawElements(GL_TRIANGLES, vertices.numIndices, GL_UNSIGNED_INT, nullptr);

		}
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

	void Scene::renderUI()
	{
		UserInterface::startUI();

		m_mainMenu.show();

		m_explorerPanel.show();
		m_entitiesPanel.show();

		for (auto& panel : m_componentsPanels)
		{
			panel.show();
		}

		UserInterface::endUI();

		//Check which entity was selected (WIP)
		//const entt::entity id = m_entityHandles[m_entitiesPanel.getSelectedEntity()];
		//m_componentsPanels[2].setText(std::to_string((int)id));
		/*
		if (m_entitiesPanel.isAddButtonClicked())
		{
			auto totalEntities = Entity::getTotalEntities();
			std::string tag = "Entity_" + std::to_string(Entity::getTotalEntities() + 1);
			Entity cameraEntity = createEntity(tag);
			m_entitiesPanel.addEntity(tag);
		}
		*/
	}

	//loads and generates shaders to be used in scene. Replace with shader wrappers as per the .h todo.
	void Scene::loadShaders()
	{
		
		std::string vertexData = ResourceManager::getInstance()->getShaderData("Fill.vs");
		std::string fragmentData = ResourceManager::getInstance()->getShaderData("Fill.fs");

		ShaderGenerator shaderGenerator(vertexData.c_str(), fragmentData.c_str());
		
		m_programId = shaderGenerator.getProgramId();
		glUseProgram(m_programId);
	}
#pragma endregion
	
#pragma region Entity Creation
	//Create an entity from the m_registry with the provided tag component, and return the entity.
	Entity Scene::createEntity(std::string tag)
	{
		Entity entity(m_registry.create(), this);
		entity.addComponent<TagComponent>(tag);
		return entity;
	}

	// Creates entities that are to be used in the scene. Replace with serialized entities as per the .h todo.
	void Scene::createEntities()
    {
		GLuint image = ResourceManager::getInstance()->getTexture("Texture_Test.jpg");
		GLuint image2 = ResourceManager::getInstance()->getTexture("Texture_Test.png");

		//Camera
		Entity cameraEntity = createEntity("camera");
		cameraEntity.addComponent<CameraComponent>(
			90.f,
			glm::mat4(1.f),
			glm::vec2(4.8, 4.8),
			100.0f,
			0.1f
			);

		Entity triangle = createEntity("triangle");
		triangle.addComponent<TransformComponent>(
			glm::vec3(0, 0, 0),
			glm::vec3(1, 1, 1),
			glm::vec3(0, 0, 0)
			);
		triangle.addComponent<TextureComponent>(image);
		triangle.addComponent<VerticesComponent>(createSprite());
		triangle.addComponent<ColorComponent>(glm::vec4(1, 1, 1, 1));

		Entity triangle2 = createEntity("triangle2");
		triangle2.addComponent<TransformComponent>(
			glm::vec3(2.f, 0, 0),
			glm::vec3(1, 1, 1),
			glm::vec3(0, 0, 0)
			);
		triangle2.addComponent<TextureComponent>(image2);
		triangle2.addComponent<VerticesComponent>(createSprite());
		triangle2.addComponent<ColorComponent>(glm::vec4(1, 1, 1, 1));


		//TODO: After Serialization: Bind Entities HERE ***

		//Get a view of all entities with script component, instantiate them, and run their onCreate().
		auto entities = getEntities<ScriptComponent>();
		for (auto [entity, script] : entities.each())
		{
			if (!script.m_instance)
			{
				script.m_instance = script.instantiateScript();
				script.m_instance->m_entity = Entity{ entity, this };
				script.m_instance->onCreate();
			}
		}
    }
#pragma endregion

#pragma region Renderable Entities
	
	//Return the VBO for sprites. If it doesn't exist, create it.
	GLuint Scene::getSpriteVBO() 
	{
		if(!createdVBO);
		{
			createdVBO = true;

			float vertices[] = 
			{
				// positions  // texture coords (UV coords)
				-1.f, -1.f, 0.f,  0.f, 1.f,  // top right
				1.f, 1.f, 0.f,    1.f, 0.f,  // bottom right
				1.f, -1.f, 0.f,   1.f, 1.f,  // bottom left
				-1.f, 1.f, 0.f,   0.f, 0.f,  // top left 
			};

			unsigned int indices[6] = 
			{ 
				0, 1, 2, //first triangle
				0, 1, 3  //second triangle
			};
			
			
			glGenBuffers(1, &m_spriteVBO);
			glBindBuffer(GL_ARRAY_BUFFER, m_spriteVBO);

			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		}

		return m_spriteVBO;
	}
	
	//Return the VAO for sprites. If it doesn't exist, create it.
	GLuint Scene::getSpriteVAO()
	{
		if (!createdVAO);
		{
			createdVAO = true;

			glGenVertexArrays(1, &m_spriteVAO);
			glBindVertexArray(m_spriteVAO);
		}
		
		return m_spriteVAO;
	}

	//Return the IBO for sprites. If it doesn't exist, create it.
	GLuint Scene::getSpriteIBO()
	{
		if (!createdIBO);
		{
			createdIBO = true;
			
			unsigned int indices[6] =
			{
				0, 1, 2, //first triangle
				0, 1, 3  //second triangle
			};

			glGenBuffers(1, &m_spriteIBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_spriteIBO);

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		}

		return m_spriteIBO;
	}

	//Set the color of the current drawable object. This would need to be run per entity/renderable.
	void Scene::setColor(glm::mat4 mvp, glm::vec4 color) 
	{
		GLuint colorUniformID = glGetUniformLocation(m_programId, "col");
		GLuint mvpID = glGetUniformLocation(m_programId, "mvp");

		//Sets color of shader
		glUniform4fv(colorUniformID, 1, glm::value_ptr(color));
		glUniformMatrix4fv(mvpID, 1, GL_FALSE, glm::value_ptr(mvp));
	}

	//Placeholder function since we don't have serialized objects. This just creates a triangle VerticesComponents to be rendered in the scene.
	VerticesComponent Scene::createSprite()
	{
		VerticesComponent vc;
		//Each vertex has one attribute, which is 2 floats to represent position
		vc.vertexAttributes.push_back(VertexAttribute(0, 3, GL_FLOAT, GL_FALSE, 0));
		vc.vertexAttributes.push_back(VertexAttribute(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 3));
		
		//TODO: Update vertexAttributes for UV
		vc.stride = sizeof(float) * 5;
		vc.numIndices = 6;

		vc.vaoID = getSpriteVAO();
		vc.vboID = getSpriteVBO();
		vc.iboID = getSpriteIBO();

		//Define vertex attributes
		for (int i = 0; i < vc.vertexAttributes.size(); i++) 
		{
			const auto attribute = vc.vertexAttributes[i];
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(attribute.index, attribute.size, attribute.type, attribute.normalized, vc.stride, (const void*)attribute.pointer);
		}

		return vc;
	}
	
#pragma endregion
	
}
