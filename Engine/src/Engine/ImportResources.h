#pragma once

#include "Core.h"
#include "json.h"
#include <iostream>
#include <string>
#include <map>

namespace Engine
{

	class ENGINE_API ImportResources
	{
		
		public:
			ImportResources()
			{
				std::cout << "ImportResources created";

			}
			~ImportResources()
			{


			}
			//json returnJson(string Name);
		
			//This would be used like the return json function, where the name of the image is provided and will be returned. 
			//The return type needs to be deteremined.
			//image returnSprite();

		private:
		
		
			//For image files. Not sure what's imported yet. @Cole, you can modify this. STB is the library currently being used.
			//std::map<std::string, int> m_images{ {"Enemy", 10}, {"Player", 15}, {"Tile", 20}, };

			//For Json files, utilizing the json library.
			//std::map<std::string, json> m_jsons{ {"SaveData", nullptr}, {"Settings", nullptr},};
	};
}