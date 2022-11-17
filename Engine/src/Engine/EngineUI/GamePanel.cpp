#include "Engine/EngineUI/GamePanel.h"

#include "Engine/Rendering/Window.h"

namespace Engine
{
	void GamePanel::show(const Window& window)
	{

		ImGui::Begin("GamePanel");
		{
			ImGui::BeginChild("GamePanel");
			ImVec2 wsize = ImGui::GetWindowSize();
			ImGui::Image((ImTextureID)window.getFboId(), ImVec2(window.getWidth(), window.getHeight()), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::EndChild();
		}

		//defines the position and size of the entities UI element
		ImGui::SetWindowPos("Game", ImVec2(m_position.x, m_position.y));
		ImGui::SetWindowSize("GamePanel", ImVec2(m_dimension.x, m_dimension.y));

		ImGui::End();
	}
}
