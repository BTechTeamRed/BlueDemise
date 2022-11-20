#include "ColorSwap.h"

namespace Engine
{
	void ColorSwap::onCreate()
	{
		// Get the current color of the tile
		glm::vec4 currentColor = m_entity.getComponent<MaterialComponent>().color;

		// Get the index of the current color
		for (auto& [index, color] : colors)
		{
			if (color == currentColor)
			{
				currentColorIndex = index;
			}
		}
	}
	
	void ColorSwap::swapMyColor() {
		GE_CORE_INFO("Swapping Color!");

		// Increment index to the next color
		currentColorIndex++;
		if (currentColorIndex > difficultyLevel) {
			currentColorIndex = 0;
		}

		// Update the color of the tile
		m_entity.getComponent<MaterialComponent>().color = colors[currentColorIndex];
	}
	
	void ColorSwap::onUpdate(const Engine::DeltaTime& dt)
	{
		
	}
}