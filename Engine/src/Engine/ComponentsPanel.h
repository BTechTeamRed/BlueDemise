#pragma once

#include "UserInterface.h"

namespace Engine
{
	class ComponentsPanel : public UserInterface
	{

	public:

		//constructor takes in name of panel as a parameter
		ComponentsPanel(const std::string& name);

		virtual ~ComponentsPanel() {}

		bool isAddButtonClicked() const;

		const std::string& getSelectedComponent() const;

		void addComponent(const std::string& component);

		virtual void show() override;

	private:

		bool m_isAddButtonClicked{ false };

		std::string m_name;

		std::string m_selectedComponent;

		std::vector<std::string> m_components;

	};
}