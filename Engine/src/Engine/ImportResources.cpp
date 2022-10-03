#include "ImportResources.h"

namespace Engine
{
	//Finds all files under the 'data' path and stores the paths in a hashmap. 
	ImportResources::ImportResources() 
	{
		//Store all files listed under m_sourcePaths in the m_filePaths hashmap
		for (int i = 0; i < m_sourcePaths.size(); i++)
		{
			std::string path = m_sourcePaths[i];

			std::cout << std::endl << path << std::endl;

			for (const auto& entry : std::filesystem::recursive_directory_iterator(path))
			{
				m_filePaths.insert(std::pair<std::string, std::string>(entry.path().filename().string(), entry.path().string()));

				GE_INFO("[ImportResources] Path added to m_filePaths [" + entry.path().string() + "]");
			}
		}
	}
	

	//------------------------- Functions ----------------------------

	nlohmann::json ImportResources::returnJson(std::string Name)
	{
		nlohmann::json data = m_jsons[Name];

		if (data != nullptr)
		{
			GE_INFO("[ImportResources] Previously loaded " + Name + " found.");
			return data;
		}
		else
		{
			std::string searchResult = m_filePaths[Name];
			std::string extension;

			GE_INFO("[ImportResources] Search Result is " + searchResult);

			if (searchResult != "")
			{
				extension = searchResult.substr(searchResult.find_last_of(".") + 1);

				if (extension == "json")
				{
					GE_INFO("[ImportResources] " + Name + " found.");

					data = readJson(searchResult);
					
					m_jsons.insert(std::pair<std::string, nlohmann::json>(Name, data));

					return data;
				}
			}
		}

		GE_INFO("[ImportResources] " + Name + " not found.");
		return nullptr;
	}


	std::string ImportResources::returnShader(std::string Name)
	{
		std::string searchResult = m_shaders[Name];

		if (searchResult != "")
		{
			GE_INFO("[ImportResources] Previously loaded " + Name + " found.");
			return searchResult;
		}
		else 
		{
			searchResult = m_filePaths[Name];
			std::string extension;
		
			GE_INFO("[ImportResources] Search Result is " + searchResult);

			if (searchResult != "")
			{
				extension = searchResult.substr(searchResult.find_last_of(".") + 1);

				if (extension == "fs" || extension == "vs")
				{
					GE_INFO("[ImportResources] " + Name + " found.");
				
					searchResult = readSource(searchResult);

					m_shaders.insert(std::pair<std::string, std::string>(Name, searchResult));
				
					return searchResult;
				}
			}
		}

		GE_INFO("[ImportResources] " + Name + " not found.");
		return nullptr;
	}
	
	std::string ImportResources::readSource(std::string sourcePath)
	{
		std::string src = "";
		std::ifstream reader(sourcePath);
		
		if (reader.is_open())
		{
			std::string line;
			while (std::getline(reader, line))
			{
				src.append(line.append("\n"));
			}
			reader.close();
		}
		return src;
	}

	nlohmann::json ImportResources::readJson(std::string sourcePath) 
	{
		nlohmann::json data;
		std::ifstream reader(sourcePath);

		if (reader.is_open())
		{
			data = nlohmann::json::parse(reader);
			
			reader.close();
		}
		return data;
	}
}