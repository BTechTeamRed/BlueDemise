#pragma once

#include "entt.h"
#include "Scene.h"
#include "Components.h"

namespace Engine
{
	// Wrapper class for ENTT entity
	class Entity
	{
	private:
		Scene* m_scene = nullptr;
		entt::entity m_entityHandle = entt::null;
	public:
		Entity() = default;
		Entity(entt::entity entityHandle, Scene* scene);
		Entity(const Entity& other) = default;
		inline entt::entity getHandle() { return m_entityHandle; }
		const std::string& getName() { return getComponent<TagComponent>().tag; }

		//Functions to help with managing entities' components
		template<typename T, typename... Args>
		T& addComponent(Args&&... args)
		{
			return m_scene->m_registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& getComponent()
		{
			return m_scene->m_registry.get<T>(m_entityHandle);
		}

		template<typename T>
		void removeComponent()
		{
			return m_scene->m_registry.remove<T>(m_entityHandle);
		}

		template<typename T>
		bool hasComponent()
		{
			return m_scene->m_registry.all_of<T>(m_entityHandle);
		}

		operator entt::entity() const { return m_entityHandle; }

		bool operator==(const Entity& other) const
		{
			return m_entityHandle == other.m_entityHandle && m_scene == other.m_scene;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}
	};
}
