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
#include "Engine/Rendering/Renderer.h"

namespace Engine
{

#pragma region Scene Management
	
	//Initialize inputsystem, renderer and loop update until the window should be closed. 
	//I feel a new loop method should be used rather than 'when the window closes'
	void Scene::onRuntimeStart()
	{
		Renderer::getInstance()->initializeScene(*this);

		while (!m_closeScene)
		{
			m_deltaTime.updateDeltaTime();
			onRuntimeUpdate(m_deltaTime);
		}

		onRuntimeStop();
	}
	
	//Upon the scenes conclusion, terminate vertices objects, and call stopScene.
	void Scene::onRuntimeStop()
	{
		const auto view = getEntities<const VerticesComponent>();
		for (auto [entity, vertices] : view.each())
		{
			glDeleteVertexArrays(1, &vertices.vaoID);
			glDeleteBuffers(1, &vertices.vboID);
		}

		Renderer::getInstance()->stopScene(*this);
	}

	//Per scene update loop, add scripts to entities if enabled and render the scene.
	void Scene::onRuntimeUpdate(const DeltaTime& dt)
	{	
		//get a view on entities with a script Component, and execute their onUpdate.
		const auto entities = getEntities<ScriptComponent>();
		for (auto [entity, script] : entities.each())
		{
			if (script.m_instance->m_enabled) script.m_instance->onUpdate(dt);//don't update if entity is disabled
		}

		//Main window
		//glfwMakeContextCurrent(m_window);
		
		Renderer::getInstance()->renderScene(dt, *this);
				
		//glfwSwapBuffers(m_window);
		glfwPollEvents();

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
