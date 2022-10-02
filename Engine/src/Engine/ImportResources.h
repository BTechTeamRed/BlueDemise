#pragma once

#include "Core.h"
#include "json.h"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <map>

namespace Engine
{

	class ENGINE_API ImportResources
	{
		
		public:
			//Constructor for ImportResources Class. Finds all files under the 'data' path and stores them in a hashmap. 
			//Currently only stores jsons.
			ImportResources()
			{
				std::string m_jsonPath = std::filesystem::current_path().string() + "\\Data";
				std::cout << std::endl << m_jsonPath << std::endl;

				for (const auto& entry : std::filesystem::directory_iterator(m_jsonPath))
				{
					//std::cout << entry.path() << std::endl;
					std::ifstream f(entry.path());

					if (f.is_open())
					{
						nlohmann::json data = nlohmann::json::parse(f);
					
						m_jsons.insert(std::pair<std::string, nlohmann::json>(entry.path().filename().string(), data));
						
						f.close();
					}
				}
			}
			
			//Deconstructor for ImportResources.
			~ImportResources()
			{
				
			}
			
			//Function to return a json from the hashmap based on a provided name. 
			nlohmann::json returnJson(std::string Name);
		
			//This would be used like the return json function, where the name of the image is provided and will be returned. 
			//The return type needs to be deteremined.
			//image returnSprite();

		private:
			//For image files. Not sure what's imported yet. @Cole, you can modify this. STB is the library currently being used.
			//std::map<std::string, int> m_images{ {"Enemy", 10}, {"Player", 15}, {"Tile", 20}, };

			//A hashmap to store Json files, utilizing the json library.
			std::map<std::string, nlohmann::json> m_jsons{};
	};
}