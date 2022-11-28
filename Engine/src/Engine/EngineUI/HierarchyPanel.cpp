#include "HierarchyPanel.h"

namespace Engine
{
	bool HierarchyPanel::isAddButtonClicked() const
	{
		return m_isAddButtonClicked;
	}

	entt::entity HierarchyPanel::getSelectedEntity() const
	{
		return m_selectedEntity;
	}

	void HierarchyPanel::setFont(const std::string& font)
	{
		m_font = font;
	}

	void HierarchyPanel::addEntity(const std::string& entityTag, entt::entity entity)
	{
		m_entities[entityTag] = entity;
	}

	//struct for entity list radio buttons
	struct EntityButton 
	{
		bool state; // true if selected
		std::string tag; // entity name
		entt::entity entity; // actual entt entity 
	};


	void HierarchyPanel::show()
	{
		ImGui::Begin("HierarchyPanel", nullptr,
			ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);

		//defines the position and size of the entities UI element
		static bool isPositionSet = false;
		if (!isPositionSet)
		{
			ImGui::SetWindowPos("HierarchyPanel", ImVec2(m_position.x, m_position.y));
			isPositionSet = true;
		}

		ImGui::SetWindowSize("HierarchyPanel", ImVec2(m_dimension.x, m_dimension.y));

		//Need some .otf/.ttf font files
		//defines the title section above the UI element
		partition(m_font, "Entities", grey);

		//sets the text colour to be red
		s_style->Colors[ImGuiCol_Text] = red;

		m_isAddButtonClicked = false;

		if (ImGui::Button("Add new entity...", ImVec2(150, 25)))
		{
			m_isAddButtonClicked = true;
		}

		if (m_entities.empty())
		{
			ImGui::Text("No entities.");
		}

		else
		{
			for (const auto& entity : m_entities)
			{
				// Initializes EntityButton struct for each entity
				EntityButton Entity;
				Entity.tag = entity.first.c_str();
				Entity.entity = entity.second;
				Entity.state = false;

				// if this entity is selected, fill in the radio button
				if (Entity.entity == m_selectedEntity) 
				{
					Entity.state = true;
				}

				if (ImGui::RadioButton(Entity.tag.c_str(), Entity.state))
				{
					m_selectedEntity = entity.second;
				}
			}
		}

		ImGui::End();

	}
}
