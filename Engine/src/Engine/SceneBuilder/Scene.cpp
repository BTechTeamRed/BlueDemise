#include "glad/glad.h"
#include "Engine/SceneBuilder/Scene.h"

#include <glm/gtc/type_ptr.hpp>

#include "Engine/Utilities/Log.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

#include "Entity.h"
#include <Engine/ResourceManagement/ShaderGenerator.h>
#include "Components.h"
#include <Engine/Utilities/DeltaTime.h>
#include "Engine/Scripts/ScriptableBehavior.h"
#include "Engine/ResourceManagement/ResourceManager.h"
#include "InputSystem.h"

namespace Engine
{

#pragma region Runtime Functions
	void Scene::onRuntimeStart()
	{

		//initialize the window for UI
		if (!initializeUI()) return;

		createEntities();
		InputSystem::getInstance()->init(m_window);

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
	
#pragma region Entity Creation
	//Create an entity from the m_registry with the provided tag component, and return the entity.
	Entity Scene::createEntity(std::string tag)
	{
		Entity entity(m_registry.create(), this);
		entity.addComponent<TagComponent>(tag);
		return entity;
	}
#pragma endregion
}
