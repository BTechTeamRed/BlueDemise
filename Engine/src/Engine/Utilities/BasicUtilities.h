#pragma once

#include <string>

namespace Engine::Utilities
{
	//returns true if the string contains a substring value
	bool strContains(std::string parent, std::string substr)
	{
		return parent.find(substr) != std::string::npos;
	}
}