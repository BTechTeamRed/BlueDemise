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


//Written by Kevin Vilanova, KevinAlexV.
//Class is intended to import and manage assets and resources used by the game engine.
namespace Engine
{

	class ENGINE_API ImportResources
	{
		
		public:
			
#pragma region Constructors and Destructors	
			
			//Constructor for ImportResources Class. Finds all files under the 'data' path and stores the paths in a map. 
			ImportResources();
			
			//Deconstructor for ImportResources.
			~ImportResources(){}
#pragma endregion


#pragma region Return Functions
			
			//Function to return a json from the hashmap based on a provided name. 
			nlohmann::json returnJson(std::string Name);

			//Function to return a shader from the hashmap based on a provided name. 
			std::string returnShader(std::string Name);
#pragma endregion
			
		private:

#pragma region Maps and Vectors
			
			//Every file path found under the specified resources folder, 'm_sourcePath'.
			std::map<std::string, std::string> m_filePaths{};

			//For image files. Not sure what's imported yet. @Cole, you can modify this. STB is the library currently being used.
			//std::map<std::string, int> m_images{ {"Enemy", 10}, {"Player", 15}, {"Tile", 20}, };

			//A map to store Json files, utilizing the json library.
			std::map<std::string, nlohmann::json> m_jsons{};
			
			//A map to store shader files as strings
			std::map<std::string, std::string> m_shaders{};

			//Vector containing all source paths to search for files (think of these as the 'root' asset folders). 
			//This can be added to from Application/Game code for custom paths.
			std::vector<std::string> m_sourcePaths = { std::filesystem::current_path().string() + "\\Data" };

#pragma endregion
			
			
#pragma region Load Functions
		
			//Read file found at sourcePath and return it as a string.
			std::string readSource(std::string sourcePath);
#pragma endregion
	};
}