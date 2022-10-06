#include "Explorer.h"

void Explorer::show()
{

	ImGui::Begin("Properties", nullptr,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);

	ImGui::SetWindowPos("Properties", ImVec2(m_position.x, m_position.y));
	ImGui::SetWindowSize("Properties", ImVec2(m_dimension.x, m_dimension.y));

	//Need some .otf/.ttf font files
	//partition("MyriadPro_Bold_16", "Font", green);

	ImGui::End();
}
