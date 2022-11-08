#include "Engine/EngineUI/ComponentsPanel.h"

namespace Engine
{
	ComponentsPanel::ComponentsPanel(const std::string& name) : m_name(name) {}

	void ComponentsPanel::show()
	{

		ImGui::Begin(m_name.c_str(), nullptr,
			ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_::ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);

		//define bounds of the window
		ImGui::SetWindowPos(m_name.c_str(), ImVec2(m_position.x, m_position.y));
		ImGui::SetWindowSize(m_name.c_str(), ImVec2(m_dimension.x, m_dimension.y));

		//Need some .otf/.ttf font files
		//defines the title section above the UI element
		partition("MyriadPro_Bold_16", m_name, white);

		//sets the colour of the text to blue, uses blue defined in UserInterface
		s_style->Colors[ImGuiCol_Text] = blue;

		//displays some text in the window
		ImGui::Text("Hello");


		ImGui::End();

	}
}