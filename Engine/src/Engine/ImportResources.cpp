#include "ImportResources.h"

namespace Engine
{
	
	//Finds all files under the 'data' path and stores the paths in a map. 
	ImportResources::ImportResources() 
	{
		//Store all files listed under m_sourcePaths in the m_filePaths map
		for (int i = 0; i < m_sourcePaths.size(); i++)
		{
			std::string path = m_sourcePaths[i];

			//Using a recursive iterator, check each path under the root paths (m_sourcePaths) and add the file path to m_filePaths.
			for (std::filesystem::recursive_directory_iterator i(path), end; i != end; ++i)
			{
				//File path is the full path of the found folder or file. Filename is the folder/file name, and file extension should contain the file type (to differentiate between file and folder).
				std::filesystem::path filePath = i->path();
				std::string fileName = filePath.filename().string();
				std::string fileExtension = "";
				
				//Check if file exists in map.
				auto it = m_filePaths.find(fileName);

				//If the filename contains a '.' (file extension), save the extension.
				if (fileName.find(".") != std::string::npos)
				{
					fileExtension = fileName.substr(fileName.find_last_of(".") + 1);
				}
			
				//If the file name is not already in the map, and an extension exists, add it to the map.
				if ((it == m_filePaths.end()) && (fileExtension != ""))
				{	
					m_filePaths.insert(std::pair<std::string, std::string>(fileName, filePath.string()));
					GE_INFO("[ImportResources] File added to m_filePaths [" + filePath.string() + "]");
				}
			}
		}
	}

#pragma region Load and Return Functions
	
	//Based on the provided filename, return the json file from a map, or load it from the system.
	nlohmann::json ImportResources::returnJson(std::string Name)
	{
		//Initilaze the return value, and create a map iterator to check if the json is already loaded.
		nlohmann::json data = nullptr;
		std::map<std::string, nlohmann::json>::iterator it = m_jsons.find(Name);
		
		//If loaded, return the json. Else, load and store the json, then return it.
		if (it != m_jsons.end())
		{
			//Get value based on map key "Name"
			data = m_jsons.find(Name)->second;
			
			GE_INFO("[ImportResources] Previously loaded " + Name + " found.");
			return data;
		}
		else
		{
			std::string jsonPath = m_filePaths[Name];
			std::string extension;

			//GE_INFO("[ImportResources] Path is " + jsonPath);

			//If json path is found (should be loaded into m_filePaths, currently upon initialization), load, store, then return the json.
			if (jsonPath != "")
			{
				extension = jsonPath.substr(jsonPath.find_last_of(".") + 1);

				//Ensure the file is a json file
				if (extension == "json")
				{
					GE_INFO("[ImportResources] " + Name + " found.");

					data = nlohmann::json::parse(readSource(jsonPath));

					m_jsons.insert(std::pair<std::string, nlohmann::json>(Name, data));
					return data;
				}
			}
		}

		GE_INFO("[ImportResources] " + Name + " not found.");
		return nullptr;
	}

	//Based on the provided filename, return the shader source from a map, or load it from the system.
	std::string ImportResources::returnShader(std::string Name)
	{
		//Initilaze the return value, and create a map iterator to check if the shader is already loaded.
		std::string data = "";
		std::map<std::string, std::string>::iterator it = m_shaders.find(Name);
		
		//If loaded, return the shader. Else, load and store the shader, then return it.
		if (it != m_shaders.end())
		{
			//Get value based on map key "Name"
			data = m_shaders.find(Name)->second;

			GE_INFO("[ImportResources] Previously loaded " + Name + " found.");
			return data;
		}
		else 
		{
			std::string searchResult = m_filePaths[Name];
			std::string extension;
		
			//GE_INFO("[ImportResources] Search Result is " + searchResult);

			//If shader path is found (should be loaded into m_filePaths, currently upon initialization), load, store, then return the shader.
			if (searchResult != "")
			{
				//Ensure the file is a shader file, based on the extension. 
				extension = searchResult.substr(searchResult.find_last_of(".") + 1);

				if (extension == "fs" || extension == "vs")
				{
					GE_INFO("[ImportResources] " + Name + " found.");
					data = readSource(searchResult);

					m_shaders.insert(std::pair<std::string, std::string>(Name, data));
				
					return data;
				}
			}
		}

		GE_INFO("[ImportResources] " + Name + " not found.");
		return "";
	}
	
	//Read the source of a file and return it as a string.
	std::string ImportResources::readSource(std::string sourcePath)
	{
		std::string src = "";
		std::ifstream reader(sourcePath);
		
		//If sourcePath is found, read the file and return it as a string.
		if (reader.is_open())
		{
			std::string line;
			
			//Upon the end of each line, append a \n for a new line.
			while (std::getline(reader, line))
			{
				src.append(line.append("\n"));
			}
			reader.close();
		}
		return src;
	}
	
#pragma endregion
}