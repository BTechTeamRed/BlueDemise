#pragma once
#include "Engine/Core.h"
#include "Engine/Entity.h"

namespace Engine
{
	// Polymorphic class that binds to <ScriptComponent>.
	// classes inheriting from ScriptableBehavior essentially give entities different behavior during the Scene loop.
	class ENGINE_API ScriptableBehavior
	{
	public:
		virtual ~ScriptableBehavior() {}

		//Returns a component T if the entity has one, otherwise it returns nullptr.
		template<typename T>
		T& getComponent()
		{
			if (m_entity.hasComponent<T>()) return nullptr;
			return m_entity.getComponent<T>();
		}

	protected:
		//gets called once after Entities are serialized and created, in Scene::createEntities().
		virtual void onCreate() {}

		//gets called before the entity is destroyed.
		//currently unused since we are not destroying entities until the game is exited.
		virtual void onDestroy() {}

		//Runs on a loop before rendering
		virtual void onUpdate(DeltaTime dt) {}

		//Runs after all other Entity onUpdates have been called and after rendering
		virtual void onLateUpdate(DeltaTime dt) {}

		//runs when enabled/disabled
		virtual void onDisable() {}
		virtual void onEnable() {}

		void enable() { m_enabled = true; this->onEnable(); }
		void disable() { m_enabled = false; this->onDisable(); }

		const std::string& toString() { return m_entity.getName(); }
		const std::string& getName() { return m_entity.getName(); }

		//TODO add event functionality
	   
	private:
		//A disabled entity will not execute onUpdate() or onLateUpdate();
		bool m_enabled{true};

		Entity m_entity;
		friend class Scene;
	};
}

