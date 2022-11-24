#pragma once
#include "UserInterface.h"

namespace Engine
{
	class Window;
	class GamePanel : public UserInterface
	{
	public:
		void setInitialPosition(int position_x, int position_y);
		void setInitialDimension(int dimension_x, int dimension_y);
		void show(Window& window);
		glm::vec2 getPosition() { return m_position; };
	private:

	};
}