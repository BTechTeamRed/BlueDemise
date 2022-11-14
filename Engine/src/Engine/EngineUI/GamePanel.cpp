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
	}
}
