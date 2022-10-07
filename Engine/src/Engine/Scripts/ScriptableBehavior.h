#pragma once
#include "Engine/Entity.h"

namespace Engine
{
	class ENGINE_API ScriptableBehavior
	{
	public:
		virtual ~ScriptableBehavior() {}

		template<typename T>
		T& getComponent()
		{
			return m_entity.getComponent<T>();
		}

	protected:
		//
		virtual void onCreate() {}

		virtual void onDestroy() {}

		//Runs on a loop before rendering
		virtual void onUpdate(DeltaTime dt) {}

	private:


	private:
		Entity m_entity;
		friend class Scene;
	};
}

