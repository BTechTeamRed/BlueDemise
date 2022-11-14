#pragma once
#include <string>
#include <vector>

#include "Engine/EngineUI/UserInterface.h"

#include "entt.h"

namespace Engine
{
	class Entity;

	class InspectorPanel : public UserInterface
	{

	public:

		virtual ~InspectorPanel() {}

		void setRegistry(entt::registry* registry);

		void setSelectedEntity(entt::entity entityHandle);

		virtual void show(unsigned int fbo = 0) override;

	private:

		entt::entity m_entityHandle {0};
		entt::registry* m_registry {nullptr};

	};
}
