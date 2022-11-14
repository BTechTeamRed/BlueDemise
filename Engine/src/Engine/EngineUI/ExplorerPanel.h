#pragma once

#include "UserInterface.h"

namespace Engine
{
	class ExplorerPanel : public UserInterface
	{

	public:

		virtual ~ExplorerPanel() {}

		virtual void show(unsigned int fbo = 0) override;

	private:



	};
}