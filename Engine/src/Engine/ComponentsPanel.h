#pragma once

#include "UserInterface.h"

class ComponentsPanel : public UserInterface
{

public:

	ComponentsPanel(const std::string& name);

	virtual void show() override;

private:

	std::string m_name;

};
