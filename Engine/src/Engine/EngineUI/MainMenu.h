#pragma once
#include "UserInterface.h"

namespace Engine
{
	class MainMenu : public UserInterface
	{

	public:

		bool getIsSaveItemClicked()
		{
			return m_isSaveItemClicked;
		}

		virtual void show(unsigned int fbo = 0) override;

	private:

		bool m_isSaveItemClicked{ false };

	};
}