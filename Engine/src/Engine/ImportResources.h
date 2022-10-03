#pragma once

#include "Core.h"
#include "json.h"
#include "Log.h"
#include <iostream>
#include <fstream>
#include<sstream>
#include <string>
#include <filesystem>
#include <map>

namespace Engine
{

	class ENGINE_API ImportResources
	{
		
		public:

			//______________________________Constructor & Deconstructors_____________________________________
			// 
			//Create a file reader, which is called from proper object specific functions (json, shader, etc)
			// 
			//Constructor for ImportResources Class. Finds all files under the 'data' path and stores the paths in a hashmap. 
			ImportResources();
			
			//Deconstructor for ImportResources.
			~ImportResources(){}
			
			//______________________________Return Functions_____________________________________
			
			//Function to return a json from the hashmap based on a provided name. 
			nlohmann::json returnJson(std::string Name);

			//Function to return a shader from the hashmap based on a provided name. 
			std::string returnShader(std::string Name);

		private:

			//_________________________________Variables______________________________________

			//-------------- Hash Maps --------------
			
			//Every file path found under the specified resources folder, 'm_sourcePath'.
			std::map<std::string, std::string> m_filePaths{};

			//For image files. Not sure what's imported yet. @Cole, you can modify this. STB is the library currently being used.
			//std::map<std::string, int> m_images{ {"Enemy", 10}, {"Player", 15}, {"Tile", 20}, };

			//A hashmap to store Json files, utilizing the json library.
			std::map<std::string, nlohmann::json> m_jsons{};
			
			//A hashmap to store Json files, utilizing the json library.
			std::map<std::string, std::string> m_shaders{};
		
			//---------------- Paths -----------------
			
			//Vector containing all source paths to search for files. This can be added to from Application/Game code for custom paths.
			std::vector<std::string> m_sourcePaths = { std::filesystem::current_path().string() + "\\Data" };
			

			//--------- Reading/Writing Functions ---------
			std::string readSource(std::string sourcePath);

			nlohmann::json readJson(std::string sourcePath);

	};
}