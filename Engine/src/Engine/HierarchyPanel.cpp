#include "HierarchyPanel.h"

namespace Engine
{
	bool HierarchyPanel::isAddButtonClicked() const
	{
		return m_isAddButtonClicked;
	}

	const std::string& HierarchyPanel::getSelectedEntity() const
	{
		return m_selectedEntity;
	}

	void HierarchyPanel::addEntity(const std::string entity)
	{
		m_entities.push_back(entity);
	}

	void HierarchyPanel::show(unsigned int fbo)
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
		partition("MyriadPro_Bold_16", "Entities", grey);

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
				if (ImGui::TreeNode(entity.c_str()))
				{
					m_selectedEntity = entity;
					ImGui::TreePop();
				}
			}
		}

		ImGui::End();

	}
}
