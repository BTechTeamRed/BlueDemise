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
		auto render = Renderer::getInstance();
		render->initializeScene(*this);
		
		// Create physics world
		glm::vec3 dimensions;
		auto entities = getEntities<CameraComponent>();
		for (auto& [entity, camera] : entities.each())
		{
			dimensions.x = camera.frustumWidth;
			dimensions.y = camera.frustumWidth / camera.aspectRatio;
			dimensions.z = camera.farZ - camera.nearZ;
		}
		GE_CORE_TRACE("Scene::onRuntimeStart: Creating world {0} x {1} x {2}", dimensions.x, dimensions.y, dimensions.z);
		m_physics = new PhysicsSystem(dimensions);

		// Insert physics objects
		auto physicsList = getEntities<PhysicsComponent>();
		for (auto& [entity, phyObj] : physicsList.each())
		{
			Entity* obj = new Entity(entity, this);
			if (!m_physics->insert(obj))
			{
				std::string tag = obj->getComponent<TagComponent>().tag;
				GE_CORE_ERROR("Scene::onRuntimStart: Failure to insert {0} into physics world", tag);
				delete obj;
			}
		}

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
		
		m_physics->update();

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

	std::list<Entity*> Scene::pick(float x, float y)
	{
		glm::vec3 worldPos = Renderer::getInstance()->screenToWorld(glm::vec2(x,y));
		GE_CORE_TRACE("Scene::pick: {0} {1} = {2} {3} {4}", x, y, worldPos.x, worldPos.y, worldPos.z);
		return m_physics->raycast(worldPos, glm::vec3(0,0,1));
	}
}