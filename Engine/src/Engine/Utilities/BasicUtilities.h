#pragma once

#include <string>
#include <vector>
#include <sstream>

namespace Engine::Utilities
{
	//returns true if the string contains a substring value
	bool strContains(std::string parent, std::string substr)
	{
		return parent.find(substr) != std::string::npos;
	}

	//separates string into vector by \n
	std::vector<std::string> strSplit(std::string source)
	{
		std::stringstream ss(source);
		std::vector<std::string> list;

		std::string s;
		while (std::getline(ss, s, '\n'))
		{
			list.push_back(s);
		}

		return list;
	}
}