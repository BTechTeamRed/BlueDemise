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

		virtual void show() override;

	private:

		bool m_isSaveItemClicked{ false };

	};
}