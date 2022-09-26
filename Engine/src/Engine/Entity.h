#pragma once

#include "entt.h"
#include "Scene.h"

namespace Engine
{
	// Wrapper class for ENTT entity
	// Credit to the following video for most of the code: https://youtu.be/8LbVpkEqKuY
	class Entity
	{
	private:
		entt::entity m_entityHandle = entt::null;
		Scene* m_scene = nullptr;
	public:
		Entity() = default;
		Entity(entt::entity entityHandle, Scene* scene);
		Entity(const Entity& other) = default;
		inline entt::entity getHandle() { return m_entityHandle; }

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
