#pragma once

#include "UserInterface.h"

namespace Engine
{
	class ExplorerPanel : public UserInterface
	{

	public:

		virtual ~ExplorerPanel() {}

		bool isAddButtonClicked();

		const std::string& getSelectedScript() const;

		virtual void show();

	private:

		bool m_isAddButtonClicked{ false };

		std::string m_selectedScript;

	};
}