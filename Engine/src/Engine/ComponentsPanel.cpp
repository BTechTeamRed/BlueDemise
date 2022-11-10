#include "ComponentsPanel.h"

namespace Engine
{
	ComponentsPanel::ComponentsPanel(const std::string& name) : m_name(name) {}

	bool ComponentsPanel::isAddButtonClicked() const
	{
		return m_isAddButtonClicked;
	}

	const std::string& ComponentsPanel::getSelectedComponent() const
	{
		return m_selectedComponent;
	}

	void ComponentsPanel::addComponent(const std::string& component)
	{
		m_components.push_back(component);
	}

	void ComponentsPanel::show()
	{

		ImGui::Begin(m_name.c_str(), nullptr,
			ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);

		//define bounds of the window
		static bool isPositionSet = false;

		if (!isPositionSet)
		{
			ImGui::SetWindowPos(m_name.c_str(), ImVec2(m_position.x, m_position.y));
			isPositionSet = true;
		}

		ImGui::SetWindowSize(m_name.c_str(), ImVec2(m_dimension.x, m_dimension.y));

		//Need some .otf/.ttf font files
		//defines the title section above the UI element
		partition("MyriadPro_Bold_16", m_name, white);

		//sets the colour of the text to blue, uses blue defined in UserInterface
		s_style->Colors[ImGuiCol_Text] = green;

		m_isAddButtonClicked = false;

		for (const auto& component : m_components)
		{
			if (ImGui::TreeNode(component.c_str()))
			{
				m_selectedComponent = component;
				ImGui::TreePop();
			}
		}

		setSpacing(3);

		if (ImGui::Button("Add component to selected entity", ImVec2(250, 25)))
		{
			m_isAddButtonClicked = true;
		}

		ImGui::End();
	}
}