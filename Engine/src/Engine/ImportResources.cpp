#include "ImportResources.h"

namespace Engine
{
	
	nlohmann::json ImportResources::returnJson(std::string Name)
	{
		//std::cout << "Searching for Json...";
				
		nlohmann::json searchResult = m_jsons[Name];
			
		if (searchResult != nullptr)
		{
			std::cout << "Json of " << Name << " found.";
			return searchResult;
		}
		else
		{
			std::cout << "Json of " << Name << " not found.";
			return nullptr;
		}

		//std::cout << m_jsons[Name] << '\n';
	}
}