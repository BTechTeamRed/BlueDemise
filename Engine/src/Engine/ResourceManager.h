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

	class ENGINE_API ResourceManager
	{
		
		public:

			#pragma region Singleton Instance Management

			//Singletons should not be cloneable, this is to prevent clones.
			ResourceManager(ResourceManager &other) = delete;
			
			//Singletons should not be assignable, this is to prevent that.
			void operator=(const ResourceManager &) = delete;
			
			//This retrieves a pointer to the current instance of ResourceManager. If it doesn't exist, then one will be created and returned.
			static ResourceManager* getInstance();
			
			#pragma endregion

			#pragma region Set Functions

			//Function to save all file paths into m_filePaths within the provided path, including subdirectories.
			void saveFilePaths(const std::string& path);
			#pragma endregion

			#pragma region Get Functions
			
			//Function to return a json from the hashmap based on a provided name. Returns a nullptr if no json is found.
			nlohmann::json getJsonData(const std::string& Name);

			//Function to return a shader from the hashmap based on a provided name. Returns an empty string if no shader is found.
			std::string getShaderData(const std::string& Name);

			//Function to save provided json data to a provided file. Will save to "Data/data.json" if file name/path are not specified.
			void saveJsonFile(nlohmann::json data, std::string fileName = "data", std::string path = "Data/");
			#pragma endregion

		
		protected:
			#pragma region Constructors
			
						//Constructor for ResourceManager Class. Finds all files under the 'data' path and stores the paths in a map. 
						ResourceManager();
			#pragma endregion
		
		private:

			

			//The static instance of ResourceManager that will be returned if the resource manager is requested.
			static ResourceManager* m_pinstance;
			
			//static lock m_mutex is the lock to be used for static functions. Otherwise, m_functionlock should be utilized.
			static std::mutex m_mutex;
			std::mutex m_functionLock;
			
			#pragma region File Storage Variables
			
			//Extensions for all files handled through resource manager.
			std::string m_jsonFileExt = "json";
			
			//TODO: There should be some member var holding the default file path for saving json files, but c++ doesn't allow it??
			//Default directory to save JSON data files
			//std::string m_jsonDataPath = "\\Data";

			std::vector<std::string> m_shaderFileExts = { "vs", "fs" };
			#pragma endregion

			#pragma region File Storage Variables
			
						//Every file path found under the specified resources folder, 'm_sourcePath'.
						std::unordered_map<std::string, std::string> m_filePaths{};

						//For image files. Not sure what's imported yet. @Cole, you can modify this. STB is the library currently being used.
						//std::map<std::string, int> m_images{ {"Enemy", 10}, {"Player", 15}, {"Tile", 20}, };

						//A map to store Json files, utilizing the json library.
						std::unordered_map<std::string, nlohmann::json> m_jsons{};
			
						//A map to store shader files as strings
						std::unordered_map<std::string, std::string> m_shaders{};

						//Vector containing all source paths to search for files (think of these as the 'root' asset folders). 
						//This can be added to from Application/Game code for custom paths.
						std::vector<std::string> m_sourcePaths = { std::filesystem::current_path().string() + "\\Data" };

			#pragma endregion
			
			#pragma region Set & Read/Load Functions
		
						//Add file path from provided directory_entry to the m_filePaths map.
						void saveFilePath(std::filesystem::directory_entry path);

						//Read file found at sourcePath and return it as a string. Will only read the provided string. Returns an empty string if nothing is found.
						std::string readSource(const std::string &sourcePath);
			#pragma endregion
	};
}