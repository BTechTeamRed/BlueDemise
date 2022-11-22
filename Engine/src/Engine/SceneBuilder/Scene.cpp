#include "glad/glad.h"
#include "Engine/SceneBuilder/Scene.h"

#include "Engine/Utilities/Log.h"

#include "Entity.h"
#include "Components.h"
#include <Engine/Utilities/DeltaTime.h>
#include "Engine/Scripts/ScriptableBehavior.h"
#include "InputSystem.h"
#include "Engine/ResourceManagement/Serializer.h"
#include "Engine/Rendering/Renderer.h"

const float DT_THRESHOLD = 10;

namespace Engine
{
#pragma region Runtime Functions
	//Initialize the scene and loop update until the window should be closed. 
	//I feel a new loop method should be used rather than 'when the window closes'
	void Scene::onRuntimeStart()
	{
		Renderer::getInstance()->initializeScene(*this);

		while (!m_closeScene) //switch will be a swap condition
		{
			m_deltaTime.updateDeltaTime();
			m_deltaTime = m_deltaTime > DT_THRESHOLD ? 0 : m_deltaTime;
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
		if (m_switch)
		{
			m_switch = false;
			swapScene(m_nextScene);
		}

		//get a view on entities with a script Component, and execute their onUpdate.
		const auto entities = getEntities<ScriptComponent>();
		for (auto [entity, script] : entities.each())
		{
			//initialize the script instance and run OnCreate();
			if (!script.m_instance)
			{
				script.m_instance = script.instantiateScript();
				script.m_instance->m_entity = Entity{ entity, this };
				script.m_instance->onCreate();
			}

			if (script.m_instance->m_enabled) script.m_instance->onUpdate(dt);//don't update if entity is disabled
		}
		
		Renderer::getInstance()->renderScene(dt, *this);
		
		glfwPollEvents();
		
		//Execute onLateUpdate().
		for (auto [entity, script] : entities.each())
		{
			if (script.m_instance->m_enabled) script.m_instance->onLateUpdate(dt);//don't update if entity is disabled
		}

		glfwPollEvents();
	}

	void Scene::swapScene(const std::string& other)
	{
		m_registry = entt::registry();
		Serializer::tryDeserializeScene(*this, other);
	}
#pragma endregion


#pragma region Entity Creation
	//Create an entity from the m_registry with the provided tag component, and return the entity.
	Entity Scene::createEntity(std::string tag)
	{
		Entity entity(m_registry.create(), this);
		entity.addComponent<TagComponent>(tag);

		Renderer::getInstance()->updateUIHeiraarchy(tag, entity);
		return entity;
	}
#pragma endregion
}