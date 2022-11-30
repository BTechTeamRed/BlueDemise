#include "Engine/EngineUI/GamePanel.h"

#include "Engine/Rendering/Window.h"

#include "Engine/Utilities/Log.h"

#include <math.h>

#include "GamePanel.h"

namespace Engine
{

	void GamePanel::show(Window& window)
	{
		
		ImGui::Begin("GamePanel", nullptr,
			ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);
		
		static bool isPositionSet = false;
		if (!isPositionSet) 
		{
			ImGui::SetWindowPos("GamePanel", ImVec2(m_position.x, m_position.y));
			ImGui::SetWindowSize("GamePanel", ImVec2(m_dimension.x, m_dimension.y));
			isPositionSet = true;
		}

		ImVec2 wsize = ImGui::GetWindowSize();
		if (std::round(wsize.x) != window.getWidth() || std::round(wsize.y) != window.getHeight()) 
		{
			window.resize(std::round(wsize.x), std::round(wsize.y));
		}
		ImGui::Image((ImTextureID)window.getFboId(), wsize, ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();
	}
}
