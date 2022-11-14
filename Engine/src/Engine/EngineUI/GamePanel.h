#pragma once
#include "UserInterface.h"

namespace Engine
{
	class GamePanel : public UserInterface
	{

	public:

		virtual void show(unsigned int fbo = 0) override;

	private:

	};
}