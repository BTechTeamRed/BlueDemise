#pragma once
#include "UserInterface.h"

namespace Engine
{
	class EntitiesPanel : public UserInterface
	{

	public:

		virtual void show(unsigned int fbo = 0) override;

	private:

	};
}