#pragma once
#include "UserInterface.h"

namespace Engine
{
	class Window;
	class GamePanel : public UserInterface
	{
	public:
		void setInitialPosition();
		void setPosition(float x, float y);
		void show(const Window& window);
	private:

	};
}