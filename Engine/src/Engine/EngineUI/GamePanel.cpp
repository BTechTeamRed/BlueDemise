#include "Engine/EngineUI/GamePanel.h"

#include "Engine/Rendering/Window.h"

#include "Engine/Utilities/Log.h"

#include <math.h>

namespace Engine
{
	void GamePanel::setInitialPosition()
	{
		//defines the position and size of the entities UI element
		ImGui::SetWindowPos("Game", ImVec2(m_position.x, m_position.y));
		ImGui::SetWindowSize("GamePanel", ImVec2(m_dimension.x, m_dimension.y));
	}

	void GamePanel::show(Window& window)
	{
		
		ImGui::Begin("GamePanel");
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

	glm::vec2 GamePanel::getPosition()
	{
		return m_position;
	}
}
