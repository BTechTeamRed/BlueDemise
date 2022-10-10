#pragma once

#include "Core.h"
#include "json.h"
#include "Log.h"
#include "glad/glad.h"
#include <iostream>
#include <fstream>
#include<sstream>
#include <string>
#include <filesystem>
#include <map>

//Written by Kevin Vilanova, KevinAlexV.
//Class is intended to import and manage assets and resources used by the game engine.
//Documentation for the stb_image library can be found here: https://github.com/nothings/stb/blob/master/stb_image.h | https://gist.github.com/roxlu/3077861
//For texture loading, the expectation is to bind a texture then render any objects using that texture before switching to another texture, as opposed to doing a load 
//per use-case (ie; do bind text 1 -> draw object A B C, then bind text 2).
namespace Engine
{

	class ENGINE_API ResourceManager
	{
		
		public:

			//A struct to contain all data contained within an STB image. 
			struct ImageData
			{
				unsigned char* image;
				int width, height, numComponents;
			};

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
			
			//Function to return a json (formatted as jsons from nlohmanns library) from the hashmap based on a provided name. Returns a nullptr if no json is found.
			nlohmann::json getJsonData(const std::string& Name);

			GLuint getTexture(const std::string& Name);

			
			//Function to return a shader (formatted as a string with newlines to seperate GSLS code) from the hashmap based on a provided name. Returns an empty string if no shader is found.
			std::string getShaderData(const std::string& Name);
			#pragma endregion

		
		protected:
			#pragma region Constructors & Deconstructors
			
			//Constructor for ResourceManager Class. Finds all files under the 'data' path and stores the paths in a map. 
			ResourceManager();

			//Deconstructor
			~ResourceManager();
			#pragma endregion
		
		private:

			//The static instance of ResourceManager that will be returned if the resource manager is requested.
			static ResourceManager* m_pinstance;
			
			//static lock m_mutex is the lock to be used for static functions. Otherwise, m_functionlock should be utilized.
			static std::mutex m_mutex;
			std::mutex m_functionLock;
			
			#pragma region File Extension Variables
			
			//Extensions for all files handled through resource manager.
			std::string m_jsonFileExt = "json";

			std::vector<std::string> m_shaderFileExts = { "vs", "fs" };
			
			std::vector<std::string> m_imageFileExts = { "jpeg", "png", "jpg"};
			#pragma endregion

			#pragma region File Storage Variables

			//Every file path found under the specified resources folder, 'm_sourcePath'.
			std::unordered_map<std::string, std::string> m_filePaths{};

			//Map to store each processed texture containing an image imported from STBI
			std::unordered_map<std::string, GLuint> m_textures{};
			
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

			//Function to return an image (formatted as a pointer to an unsigned char) from the hashmap based on a provided name. Returns a nullptr if no image is found.
			//NOTE: After using the image, you MUST use stbi_image_free(); in order to free the memory of the image.
			ImageData readImageData(const std::string& Name);

			//Read file found at sourcePath and return it as a string. Will only read the provided string. Returns an empty string if nothing is found.
			std::string readSource(const std::string &sourcePath);
			#pragma endregion
	};
}