#pragma once
#include "Engine/EngineUI/UserInterface.h"
#include "entt.h"

namespace Engine
{
	class HierarchyPanel : public UserInterface
	{

	public:

		virtual ~HierarchyPanel() {}

		bool isAddButtonClicked() const;

		entt::entity getSelectedEntity() const;

		void setFont(const std::string& font);

		void addEntity(const std::string& entityTag, entt::entity entity);

		virtual void show();

	private:

		bool m_isAddButtonClicked{ false };

		std::string m_font;

		entt::entity m_selectedEntity{ 0 };

		std::unordered_map<std::string, entt::entity> m_entities;

	};
}