#include "Engine/EngineUI/GamePanel.h"

#include "Engine/Rendering/Window.h"

#include "Engine/Utilities/Log.h"

#include <math.h>

namespace Engine
{
	void GamePanel::setInitialPosition(int position_x, int position_y)
	{
		//defines the position and size of the entities UI element
		ImGui::SetWindowPos("Game", ImVec2(position_x, position_y));
	}

	void GamePanel::setInitialDimension(int dimension_x, int dimension_y) {
		ImGui::SetWindowSize("GamePanel", ImVec2(dimension_x, dimension_y));
	}

	void GamePanel::show(Window& window)
	{
		
		ImGui::Begin("GamePanel", nullptr,
			ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);
		{
			ImGui::BeginChild("GamePanel");
			ImVec2 wsize = ImGui::GetWindowSize();
			if (std::round(wsize.x) != window.getWidth() || std::round(wsize.y) != window.getHeight()) 
			{
				window.resize(std::round(wsize.x), std::round(wsize.y));
			}
			auto imguiWindowPos = ImGui::GetWindowPos();
			m_position.x = imguiWindowPos.x;
			m_position.y = imguiWindowPos.y;
			ImGui::Image((ImTextureID)window.getFboId(), wsize, ImVec2(0, 1), ImVec2(1, 0));
			ImGui::EndChild();
		}

		ImGui::End();
	}
}
