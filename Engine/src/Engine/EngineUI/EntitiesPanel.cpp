#include "Engine/EngineUI/EntitiesPanel.h"

namespace Engine
{
	void EntitiesPanel::show()
	{
		ImGui::Begin("EntitiesPanel", nullptr,
			ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_::ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);

		//defines the position and size of the entities UI element
		ImGui::SetWindowPos("EntitiesPanel", ImVec2(m_position.x, m_position.y));
		ImGui::SetWindowSize("EntitiesPanel", ImVec2(m_dimension.x, m_dimension.y));

		//Need some .otf/.ttf font files
		//defines the title section above the UI element
		partition("MyriadPro_bold_18", "Entities", darkBlue);

		s_style->Colors[ImGuiCol_Text] = lightCyan;

		//displays text in the UI element
		ImGui::Text("Player");
		ImGui::Text("Enemy");
		ImGui::Text("Floor");

		ImGui::End();

	}
}
