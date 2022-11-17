//Needed to define stbi class, for stb related use in this file. Essentially defines code to be used in this file.
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "glad/glad.h"

#include "ResourceManager.h"
#include "stb_image.h"
#include "GLFW/glfw3.h"
#include "Engine/Rendering/Renderer.h"
#include <fstream>
#include <iostream>
#include "Engine/Utilities/Log.h"

namespace Engine
{
#pragma region Singleton Instance Management
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
	
	//Delete all texture GLuints in the texture map.
	ResourceManager::~ResourceManager() 
	{
		//Delete all textures
		for (auto& texture : m_textures)
		{
			glDeleteTextures(1, &texture.second.texID);
		}
	}
#pragma endregion
	
#pragma region Set Functions
	
	//Save all file paths from the provided path into m_filePaths, including subdirectories.
	void ResourceManager::saveFilePaths(const std::string& path)
	{
		//Lock function to prevent other threads from saving files concurrently.
		std::lock_guard<std::mutex> lock(m_functionLock);

		//create an error handler in case the path is invalid.
		std::error_code err;

		//Using a recursive iterator, check each path under the root paths (m_sourcePaths) and add the file path to m_filePaths.
		for (auto entry : std::filesystem::recursive_directory_iterator(path, err))
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

	//Save provided json object to a provided filename. Defaults file directory to m_appAssetsPath
	//If the provided file name does not exist, it will be created. Otherwise, the existing file will be overwritten.
	void ResourceManager::saveJsonFile(nlohmann::json data, std::string fileName, std::string path, std::string extension)
	{
		//Create a file path from the provided path, file name, and file extension.
		std::string fileWithPath = path + fileName + "." + extension;

		std::ofstream fileStream(fileWithPath);	// Create a file stream and open the file
		if (fileStream.is_open())
		{
			fileStream << data;					// Replace existing file content with the updated prevData
			fileStream.close();					// Close the file stream so it doesn't hang open
		}
		else
		{
			// This should never happen but is left in for debugging purposes.
			GE_CORE_ERROR("[ResourceManager] Could not open file stream to save JSON at {0}", fileWithPath);
		}
	}
	
	//Obtain icon at filepath stored in this class, then set the icon for the provided window.
	void ResourceManager::setAppIcon(std::string appIcon, GLFWwindow* window)
	{
		ImageData img;
		
		//Create an iterator to check if the file exists in the map (done since using m_filePaths[name] will create a new entry if it doesn't exist).
		auto imagePath = m_filePaths.find(appIcon);

		//If image path is found (should be loaded into m_filePaths, currently upon initialization), load, then set app icon.
		if (imagePath == m_filePaths.end())
		{
			GE_CORE_WARN("[ResourceManager] Could not find icon of name {0}.", appIcon);
			return;
		}

		std::string path = m_filePaths[appIcon];
		std::string extension = path.substr(path.find_last_of('.') + 1);

		//Ensure the file is an image file
		if (std::find(m_imageFileExts.begin(), m_imageFileExts.end(), extension) != m_imageFileExts.end())
		{

			GE_CORE_INFO("[ResourceManager] Icon being set to {0}", appIcon);

			GLFWimage images[1];

			//Create a GLFW image and load the icon into it.
			images[0].pixels = stbi_load(path.c_str(), &images[0].width, &images[0].height, 0, 4);

			glfwSetWindowIcon(window, 1, images);
			stbi_image_free(images[0].pixels);
		}
		else 
		{
			GE_CORE_WARN("[ResourceManager] Icon extension not supported for {0}", appIcon);
		}
		
	}
#pragma endregion
	
#pragma region Get Functions

	//Based on the provided filename, return the json file from a map, or load it from the system.
	nlohmann::json ResourceManager::getJsonData(const std::string& name)
	{
		std::lock_guard<std::mutex> lock(m_functionLock);

		//Initilaze the return value, and create a map iterator to check if the json is already loaded.
		nlohmann::json data = nullptr;
		auto it = m_jsons.find(name);
		
		//If loaded, return the json. Else, load and store the json, then return it.
		if (it != m_jsons.end())
		{
			//Get value based on map key "name"
			data = m_jsons.find(name)->second;
			
			GE_CORE_WARN("[ResourceManager] Previously loaded " + name + " found.");
			return data;
		}
		
		//Create an iterator to check if the file exists in the map (done since using m_filePaths[name] will create a new entry if it doesn't exist).
		auto jsonPath = m_filePaths.find(name);
			
		//If json path is found (should be loaded into m_filePaths, currently upon initialization), load, store, then return the json.
		if (jsonPath != m_filePaths.end())
		{
			std::string path = m_filePaths[name];
			std::string extension = path.substr(path.find_last_of('.') + 1);
			
			//Ensure the file is a json file
			if (m_jsonFileExts.find(extension) != m_jsonFileExts.end())
			{
				GE_CORE_INFO("[ResourceManager] " + name + " found.");
				
				data = nlohmann::json::parse(readSource(path));

				m_jsons.insert(std::pair<std::string, nlohmann::json>(name, data));
				return data;
			}
		}

		GE_CORE_INFO("[ResourceManager] " + name + " not found.");
		return nullptr;
	}

	//A function to get font file path
	std::string ResourceManager::getFont(const std::string& name)
	{
		std::string path;

		//Create an iterator to check if the file exists in the map (done since using m_filePaths[name] will create a new entry if it doesn't exist).
		auto fontPath = m_filePaths.find(name);

		//If json path is found (should be loaded into m_filePaths, currently upon initialization), load, store, then return the json.
		if (fontPath != m_filePaths.end())
		{
			path = m_filePaths[name];
		}

		return path;
	}

	//Based on the provided filename, return the ImageData for the texture stored within a map, or load it from the system.
	//Currently only supports 2D Textures, but can be changed using texType.
	ResourceManager::ImageData ResourceManager::getTexture(const std::string& name)
	{
		//Initilaze the return value, and create a map iterator to check if the data is already loaded.
		GLuint data = NULL;
		ImageData img;
		auto it = m_textures.find(name);
	
		//If loaded, return the image. Else, load and store the texture, then return it.
		if (it != m_textures.end())
		{
			//Get value from map based on "name"
			img = m_textures.find(name)->second;

			GE_CORE_WARN("[ResourceManager] Previously loaded texture " + name + " found.");
			return img;
		}
		
		img = readImageData(name);
		
		//If image data isn't nullptr, store and return data.
		if (img.image != nullptr)
		{
			img.texID = generateTexture(img);

			//Store texture for later use.
			m_textures.insert(std::pair<std::string, ImageData>(name, img));
			
			return img;
		}
		else 
		{
			GE_CORE_ERROR("[ResourceManager] " + name + " texture could not be created. Image = nullptr");
		}
		
		return img;
	
	}

	//Based on the provided spritesheet filename, return the vector of GLuints for the sprites stored within a map, or load it from the system.
	ResourceManager::SpriteSheet ResourceManager::getSpritesheet(const std::string& name, float spriteWidth, float spriteHeight)
	{
		//Initilaze the return value, and create a map iterator to check if the data is already loaded.
		SpriteSheet data;

		auto it = m_spritesheetsTex.find(name);

		//If loaded, return the spritesheet. Else, load and store the image, then return it.
		if (it != m_spritesheetsTex.end())
		{
			//Get value based on map key "name"
			SpriteSheet data = m_spritesheetsTex.find(name)->second;

			GE_CORE_WARN("[ResourceManager] Previously loaded spritesheet " + name + " found.");
			return data;
		}

		ImageData img = readImageData(name);

		//If image data isn't nullptr, store and return data.
		if (img.image != nullptr)
		{
			ImageData spriteSheet = getTexture(name);

			if (spriteWidth == 0.f || spriteHeight == 0.f)
			{
				spriteWidth = spriteSheet.width / data.spritesPerRow;
				spriteHeight = spriteSheet.height / data.spritesPerColumn;
			}
			
			//Save sprite data to spriteSheet struct
			data.spritesPerRow = spriteSheet.width / spriteWidth;
			data.spritesPerColumn = spriteSheet.height / spriteHeight;
			data.spriteWidth = spriteWidth;
			data.spriteHeight = spriteHeight;
			data.texWidthFraction = float(spriteWidth) / spriteSheet.width;
			data.texHeightFraction = float(spriteHeight) / spriteSheet.height;
			data.numSprites = data.spritesPerRow * data.spritesPerColumn;
			data.spriteSheetHeight = spriteSheet.height;
			data.spriteSheetWidth = spriteSheet.width;
			data.texID = spriteSheet.texID;

			return data;
		}

		GE_CORE_ERROR("[ResourceManager] " + name + " texture could not be created. Image = nullptr");

		return data;

	}

	//Generate 2D texture and return GLuint for new texture, based on provided image data. Will release STBImage from memeory before return.
	//This function assumes the texture is new, and not a duplicate of an already loaded texture.
	GLuint ResourceManager::generateTexture(ImageData& img)
	{
		GLuint data;

		//Determine the kind of texture being loaded. Currently only using GL_TEXTURE_2D
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
		if (img.numComponents == m_RGB)
			glTexImage2D(texType, 0, GL_RGB, img.width, img.height, 0, GL_RGB, GL_UNSIGNED_BYTE, img.image);
		else if (img.numComponents == m_RGBA)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width, img.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.image);
		
		//Free the image from memory
		stbi_image_free(img.image);

		//Generate Mimaps (different sizes of the image)
		glGenerateMipmap(texType);

		//Unbind the texture so it isn't modified anymore.
		glBindTexture(texType, 0);

		return data;
	}

	//Based on the provided filename, return the shader source from a map, or load it from the system.
	std::string ResourceManager::getShaderData(const std::string& name)
	{
		std::lock_guard<std::mutex> lock(m_functionLock);

		//Initilaze the return value, and create a map iterator to check if the shader is already loaded.
		std::string data;
		auto it = m_shaders.find(name);
		
		//If loaded, return the shader. Else, load and store the shader, then return it.
		if (it != m_shaders.end())
		{
			//Get value based on map key "name"
			data = m_shaders.find(name)->second;

			GE_CORE_WARN("[ResourceManager] Previously loaded " + name + " found.");
			return data;
		}
		
		//Create an iterator to check if the file exists in the map (done since using m_filePaths[name] will create a new entry if it doesn't exist).
		auto shaderPath = m_filePaths.find(name);
			
		//If shader path is found (should be loaded into m_filePaths, currently upon initialization), load, store, then return the shader.
		if (shaderPath != m_filePaths.end())
		{
			
			std::string searchResult = m_filePaths[name];
			//Ensure the file is a shader file, based on the extension. 
			std::string extension = searchResult.substr(searchResult.find_last_of('.') + 1);

			//Check if the extension is apart of the acceptable shader extensions stored in m_shaderFileExts.
			if (std::find(m_shaderFileExts.begin(), m_shaderFileExts.end(), extension) != m_shaderFileExts.end())
			{
				GE_CORE_INFO("[ResourceManager] " + name + " found.");
				data = readSource(searchResult);
				
				m_shaders.insert(std::pair<std::string, std::string>(name, data));
				
				return data;
			}
		}

		GE_CORE_INFO("[ResourceManager] " + name + " not found.");
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
	ResourceManager::ImageData ResourceManager::readImageData(const std::string& name)
	{
		//Initilaze the return value, and create a map iterator to check if the data is already loaded.
		ResourceManager::ImageData data;

		//Create an iterator to check if the file exists in the map (done since using m_filePaths[name] will create a new entry if it doesn't exist).
		auto imagePath = m_filePaths.find(name);

		//If image path is found (should be loaded into m_filePaths, currently upon initialization), load, store, then return the data.
		if (imagePath != m_filePaths.end())
		{
			std::string path = m_filePaths[name];
			std::string extension = path.substr(path.find_last_of('.') + 1);

			//Ensure the file is an image file
			if (std::find(m_imageFileExts.begin(), m_imageFileExts.end(), extension) != m_imageFileExts.end())
			{
				GE_CORE_INFO("[ResourceManager] " + name + " found.");

				//Last digit can be 1-4, and forces that many components per pixel (or STBI_rgb_alpha), see https://github.com/nothings/stb/blob/master/stb_image.h. Otherwise, 0 will force the number of components to be determined by the image.
				data.image = stbi_load(path.c_str(), &data.width, &data.height, &data.numComponents, 0);

				//If image data isn't NULL, store and return data.
				if (data.image != nullptr)
				{
					return data;
				}

				GE_CORE_ERROR("[ResourceManager] Failed to load image " + name);
			}
			else
			{
				GE_CORE_ERROR("[ResourceManager] " + name + " is not an image file. Verify it is supported by STB, and the extension is stored within m_imageFileExts");
			}
		}

		GE_CORE_INFO("[ResourceManager] " + name + " not found.");
		return data;
	}
#pragma endregion
}
#endif