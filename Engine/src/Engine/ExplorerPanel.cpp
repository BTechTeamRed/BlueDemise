#include "ExplorerPanel.h"

void ExplorerPanel::show()
{

	ImGui::Begin("ExplorerPanel", nullptr,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);

	//sets the position and size of the UI element
	ImGui::SetWindowPos("ExplorerPanel", ImVec2(m_position.x, m_position.y));
	ImGui::SetWindowSize("ExplorerPanel", ImVec2(m_dimension.x, m_dimension.y));

	//Need some .otf/.ttf font files
	//defines the title section above the UI element
	partition("MyriadPro_Bold_16", "Explorer", white);

	//sets the text colour to be green
	s_style->Colors[ImGuiCol_Text] = green;

	//creates a drop down where if x is true (open), display y 
	if (ImGui::TreeNode("Asset folder"))
	{
		if (ImGui::TreeNode("Models"))
		{
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Textures"))
		{
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Sprites"))
		{
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Audio"))
		{
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}

	ImGui::End();
}
