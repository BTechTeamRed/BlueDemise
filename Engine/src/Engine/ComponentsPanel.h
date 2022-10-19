#pragma once

#include "UserInterface.h"

class ComponentsPanel : public UserInterface
{

public:

	//constructor takes in name of panel as a parameter
	ComponentsPanel(const std::string& name);

	virtual void show() override;

private:

	std::string m_name;

};
