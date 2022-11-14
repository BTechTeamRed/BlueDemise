#include "Engine/EngineUI/GamePanel.h"

namespace Engine
{
	void GamePanel::show(unsigned int fbo)
	{

		ImGui::Begin("GamePanel");
		{
			ImGui::BeginChild("GamePanel");
			ImVec2 wsize = ImGui::GetWindowSize();
			ImGui::Image((ImTextureID)fbo, ImVec2(1920, 1080), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::EndChild();
		}

		//defines the position and size of the entities UI element
		ImGui::SetWindowPos("Game", ImVec2(m_position.x, m_position.y));
		ImGui::SetWindowSize("GamePanel", ImVec2(m_dimension.x, m_dimension.y));

		ImGui::End();


		//ImGui::Begin("GamePanel", nullptr,
		//	ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar |
		//	ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
		//	ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar |
		//	ImGuiWindowFlags_::ImGuiWindowFlags_NoMove |
		//	ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);

		////defines the position and size of the entities UI element
		//ImGui::SetWindowPos("EntitiesPanel", ImVec2(m_position.x, m_position.y));
		//ImGui::SetWindowSize("EntitiesPanel", ImVec2(m_dimension.x, m_dimension.y));

		////Need some .otf/.ttf font files
		////defines the title section above the UI element
		//partition("MyriadPro_Bold_16", "Entities", white);

		////sets the text colour to be red
		//s_style->Colors[ImGuiCol_Text] = red;

		////displays text in the UI element
		//ImGui::Text("Player");
		//ImGui::Text("Enemy");
		//ImGui::Text("Floor");

		//ImGui::End();

	}
}
