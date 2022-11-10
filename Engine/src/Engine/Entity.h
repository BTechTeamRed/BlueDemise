#pragma once

#include "entt.h"
#include "Scene.h"
#include "Components.h"

namespace Engine
{
	// Wrapper class for ENTT entity
	class ENGINE_API Entity
	{
	private:
		static int s_totalEntities;
		Scene* m_scene = nullptr;
		entt::entity m_entityHandle = entt::null;
	public:
		static int getTotalEntities();

		Entity() = default;
		Entity(entt::entity entityHandle, Scene* scene);
		Entity(const Entity& other) = default;
		inline entt::entity getHandle() { return m_entityHandle; }
		const std::string& getName() { return getComponent<TagComponent>().tag; }
		
#pragma region Entity Component Management	
		
		//A template for adding components to an entity.
		template<typename T, typename... Args>
		T& addComponent(Args&&... args)
		{
			return m_scene->m_registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
		}

		//A template for getting components from an entity.
		template<typename T>
		T& getComponent()
		{
			return m_scene->m_registry.get<T>(m_entityHandle);
		}

		//A template to remove components from an entity.
		template<typename T>
		void removeComponent()
		{
			m_scene->m_registry.remove<T>(m_entityHandle);
		}

		//A template to check if an entity contains a component.
		template<typename T>
		bool hasComponent()
		{
			return m_scene->m_registry.all_of<T>(m_entityHandle);
		}

		operator entt::entity() const { return m_entityHandle; }

		//An operator to check if two entities are the same.
		bool operator==(const Entity& other) const
		{
			return m_entityHandle == other.m_entityHandle && m_scene == other.m_scene;
		}

		//An operator to check if an entity is not the same
		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

		//Returns true if entity is null
		operator bool() const { return m_entityHandle != entt::null; }
#pragma endregion
	};
}
