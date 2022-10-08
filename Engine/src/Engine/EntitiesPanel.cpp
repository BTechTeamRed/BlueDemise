#include "EntitiesPanel.h"

void EntitiesPanel::show()
{

	ImGui::Begin("EntitiesPanel", nullptr,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);

	ImGui::SetWindowPos("EntitiesPanel", ImVec2(m_position.x, m_position.y));
	ImGui::SetWindowSize("EntitiesPanel", ImVec2(m_dimension.x, m_dimension.y));

	//Need some .otf/.ttf font files
	partition("MyriadPro_Bold_16", "Entities", white);

	s_style->Colors[ImGuiCol_Text] = red;

	ImGui::Text("Player");
	ImGui::Text("Enemy");
	ImGui::Text("Floor");

	ImGui::End();

}
