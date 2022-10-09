//Needed to define stbi class, for stb related use in this file. Essentially defines code to be used in this file.
#define STB_IMAGE_IMPLEMENTATION

#include "ResourceManager.h"
#include "stb_image.h"

namespace Engine
{
	ResourceManager* ResourceManager::m_pinstance{ nullptr };
	std::mutex ResourceManager::m_mutex;
	
	//Finds all files under the 'data' path and stores the paths in a map. 
	ResourceManager::ResourceManager() 
	{
		//Store all files listed under m_sourcePaths in the m_filePaths map
		for (int e = 0; e < m_sourcePaths.size(); e++)
		{
			std::string path = m_sourcePaths[e];

			saveFilePaths(path);
		}
	}

	//Return the instance of resource manager. If one does not exist, create it, and return the pointer.
	ResourceManager* ResourceManager::getInstance() 
	{
		//To ensure this is thread safe, lock this function until it returns a value.
		std::lock_guard<std::mutex> lock(m_mutex);

		if (m_pinstance == nullptr)
		{
			m_pinstance = new ResourceManager();
		}

		return m_pinstance;
	}
	
	//Delete all image pointers in the image map, and frees images using stb library.
	ResourceManager::~ResourceManager() 
	{
		//Delete all images
		//for (auto& image : m_images)
		//{
			//stbi_image_free(image.second.image);
			//delete image.second.image;
		//}/
	}

#pragma region Set Functions
	
	//Save all file paths from the provided path into m_filePaths, including subdirectories.
	void ResourceManager::saveFilePaths(const std::string& path)
	{
		//Lock function to prevent other threads from saving files concurrently.
		std::lock_guard<std::mutex> lock(m_functionLock);

		//Using a recursive iterator, check each path under the root paths (m_sourcePaths) and add the file path to m_filePaths.
		for (const auto& entry : std::filesystem::recursive_directory_iterator(path))
		{
			//If the file is not a directory, store the path.
			if (!std::filesystem::is_directory(entry.path()))
			{
				saveFilePath(entry);
			}
		}
	}

	//Load a file from a path into the m_filePaths map, if it is not already in the map and is a file.
	void ResourceManager::saveFilePath(std::filesystem::directory_entry path)
	{
		//File path is the full path of the found folder or file. Filename is the folder/file name, and file extension should contain the file type (to differentiate between file and folder).
		std::filesystem::path filePath = path.path();
		std::string fileName = filePath.filename().string();

		//Check if file exists in map.
		auto it = m_filePaths.find(fileName);
		
		//If the file name is not already in the map add it to the map.
		if (it == m_filePaths.end())
		{
			m_filePaths.insert(std::pair<std::string, std::string>(fileName, filePath.string()));
			GE_CORE_INFO("[ResourceManager] File added to m_filePaths [{0}]", filePath.string());
		}
	}
#pragma endregion
	
#pragma region Get Functions
	//Based on the provided filename, return the json file from a map, or load it from the system.
	nlohmann::json ResourceManager::getJsonData(const std::string& Name)
	{
		std::lock_guard<std::mutex> lock(m_functionLock);

		//Initilaze the return value, and create a map iterator to check if the json is already loaded.
		nlohmann::json data = nullptr;
		std::unordered_map<std::string, nlohmann::json>::iterator it = m_jsons.find(Name);
		
		//If loaded, return the json. Else, load and store the json, then return it.
		if (it != m_jsons.end())
		{
			//Get value based on map key "Name"
			data = m_jsons.find(Name)->second;
			
			GE_CORE_WARN("[ResourceManager] Previously loaded " + Name + " found.");
			return data;
		}
		
		//Create an iterator to check if the file exists in the map (done since using m_filePaths[Name] will create a new entry if it doesn't exist).
		std::unordered_map<std::string, std::string>::iterator jsonPath = m_filePaths.find(Name);
			
		//If json path is found (should be loaded into m_filePaths, currently upon initialization), load, store, then return the json.
		if (jsonPath != m_filePaths.end())
		{
			std::string path = m_filePaths[Name];
			std::string extension = path.substr(path.find_last_of('.') + 1);
			
			//Ensure the file is a json file
			if (extension == m_jsonFileExt)
			{
				GE_CORE_INFO("[ResourceManager] " + Name + " found.");
				
				data = nlohmann::json::parse(readSource(path));

				m_jsons.insert(std::pair<std::string, nlohmann::json>(Name, data));
				return data;
			}
		}

		GE_CORE_INFO("[ResourceManager] " + Name + " not found.");
		return nullptr;
	}

	GLuint ResourceManager::getTexture(const std::string& Name)
	{
		//std::lock_guard<std::mutex> lock(m_functionLock);

		//Initilaze the return value, and create a map iterator to check if the data is already loaded.
		GLuint data;
		std::unordered_map<std::string, GLuint>::iterator it = m_textures.find(Name);

		//If loaded, return the image. Else, load and store the image, then return it.
		if (it != m_textures.end())
		{
			//Get value based on map key "Name"
			data = m_textures.find(Name)->second;

			GE_CORE_WARN("[ResourceManager] Previously loaded texture " + Name + " found.");
			return data;
		}
		
		ImageData img = readImageData(Name);
		
		//If image data isn't nullptr, store and return data.
		if (img.image != nullptr)
		{
			GLenum texType = GL_TEXTURE_2D;

			// Generates an OpenGL texture object
			glGenTextures(1, &data);
			
			// Assigns the texture to a Texture Unit
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(texType, data);

			
			// Configures the type of algorithm that is used to make the image smaller or bigger
			glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			//Configures the way the texture repeasts
			glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			
			// Extra lines in case you choose to use GL_CLAMP_TO_BORDER
			// float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
			// glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);
			
			// Assigns the image to the OpenGL Texture object based on the number of RGB components (if 4, then alpha value is present).
			if (img.numComponents == 3)
				glTexImage2D(texType, 0, GL_RGB, img.width, img.height, 0, GL_RGB, GL_UNSIGNED_BYTE, img.image);
			else if (img.numComponents == 4)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width, img.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.image);
			
			//Free the image from memory
			stbi_image_free(img.image);
			
			//Generate Mimaps (different sizes of the image)
			glGenerateMipmap(texType);

			//Store texture for later use.
			//m_textures.insert(std::pair<std::string, GLuint>(Name, data));
			
			
			//Unbind the texture so it isn't modified anymore.
			glBindTexture(texType, 0);
			
			return data;
		}
		else 
		{
			GE_CORE_ERROR("[ResourceManager] " + Name + " texture could not be created. Image = nullptr");
		}
		
		return NULL;
	
	}

	
	//Based on the provided filename, return the shader source from a map, or load it from the system.
	std::string ResourceManager::getShaderData(const std::string& Name)
	{
		std::lock_guard<std::mutex> lock(m_functionLock);

		//Initilaze the return value, and create a map iterator to check if the shader is already loaded.
		std::string data;
		std::unordered_map<std::string, std::string>::iterator it = m_shaders.find(Name);
		
		//If loaded, return the shader. Else, load and store the shader, then return it.
		if (it != m_shaders.end())
		{
			//Get value based on map key "Name"
			data = m_shaders.find(Name)->second;

			GE_CORE_WARN("[ResourceManager] Previously loaded " + Name + " found.");
			return data;
		}
		
		//Create an iterator to check if the file exists in the map (done since using m_filePaths[Name] will create a new entry if it doesn't exist).
		std::unordered_map<std::string, std::string>::iterator shaderPath = m_filePaths.find(Name);
			
		//If shader path is found (should be loaded into m_filePaths, currently upon initialization), load, store, then return the shader.
		if (shaderPath != m_filePaths.end())
		{
			
			std::string searchResult = m_filePaths[Name];
			//Ensure the file is a shader file, based on the extension. 
			std::string extension = searchResult.substr(searchResult.find_last_of('.') + 1);

			//Check if the extension is apart of the acceptable shader extensions stored in m_shaderFileExts.
			if (std::find(m_shaderFileExts.begin(), m_shaderFileExts.end(), extension) != m_shaderFileExts.end())
			{
				GE_CORE_INFO("[ResourceManager] " + Name + " found.");
				data = readSource(searchResult);
				
				m_shaders.insert(std::pair<std::string, std::string>(Name, data));
				
				return data;
			}
		}

		GE_CORE_INFO("[ResourceManager] " + Name + " not found.");
		return data;
	}
#pragma endregion
	
#pragma region Read Functions
	
	//Read the provided file path and return it as a string.
	std::string ResourceManager::readSource(const std::string &sourcePath)
	{
		std::string src;
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

	//Based on the provided filename, load the image from the system and return the imageData struct contain all data provided by stbi_load. 
	//NOTE: After using the image, you MUST use stbi_image_free(); in order to free the memory of the image.
	ResourceManager::ImageData ResourceManager::readImageData(const std::string& Name)
	{
		//Initilaze the return value, and create a map iterator to check if the data is already loaded.
		ResourceManager::ImageData data = { nullptr, 0, 0, 0 };

		//Create an iterator to check if the file exists in the map (done since using m_filePaths[Name] will create a new entry if it doesn't exist).
		std::unordered_map<std::string, std::string>::iterator imagePath = m_filePaths.find(Name);

		//If image path is found (should be loaded into m_filePaths, currently upon initialization), load, store, then return the data.
		if (imagePath != m_filePaths.end())
		{
			std::string path = m_filePaths[Name];
			std::string extension = path.substr(path.find_last_of('.') + 1);

			//Ensure the file is an image file
			if (std::find(m_imageFileExts.begin(), m_imageFileExts.end(), extension) != m_imageFileExts.end())
			{
				GE_CORE_INFO("[ResourceManager] " + Name + " found.");

				//Last digit can be 1-4, and forces that many components per pixel, see https://github.com/nothings/stb/blob/master/stb_image.h
				data.image = stbi_load(path.c_str(), &data.width, &data.height, &data.numComponents, STBI_rgb);

				//If image data isn't NULL, store and return data.
				if (data.image != nullptr)
				{
					return data;
				}

				GE_CORE_ERROR("[ResourceManager] Failed to load image " + Name);
			}
			else
			{
				GE_CORE_ERROR("[ResourceManager] " + Name + " is not an image file. Verify it is supported by STB, and the extension is stored within m_imageFileExts");
			}
		}

		GE_CORE_INFO("[ResourceManager] " + Name + " not found.");
		return data;
	}
#pragma endregion
}