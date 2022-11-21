#include "Text.h"
#include "glad/glad.h"

#include "Engine/ResourceManagement/ResourceManager.h"

#include <Engine/Utilities/Log.h>

#include FT_FREETYPE_H

namespace Engine 
{
	//Construct the text class by initializing the texture generation of ASCII characters.
	Text::Text()
	{
		if (initializeText())
			GE_CORE_INFO("[Text] Text initialized.");
		else
			GE_CORE_ERROR("[Text] Text unsuccessfully initialized. Please check Text.cpp");
			
	}

	//Iterate through 128 ASCII characters and generate textures for each to be rendered.
	bool Text::initializeText()
	{
		if (FT_Init_FreeType(&ft))
		{
			GE_CORE_ERROR("[Text] Could not init FreeType Library");
			return false;
		}

		if (FT_New_Face(ft, ResourceManager::getInstance()->getFont(defaultFont).c_str(), 0, &face))
		{
			GE_CORE_ERROR("[Text] Failed to load {0}", defaultFont);
			return false;
		}
		//disable byte-alignment restriction from opengl
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		//In this for loop, iterate through all 128 characters in the ASCll set and generate textures for each to be rendered.
		for (unsigned char i = 0; i < 128; i++)
		{
			//load character glyph
			if (FT_Load_Char(face, i, FT_LOAD_RENDER))
			{
				GE_CORE_ERROR("[Text] Error: failed to load Glyph");
				continue;
			}	

			//Generate texture
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);

			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_GREEN,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_BLUE,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);

			//setting up the texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
			//store character for later use
			Character character = 
			{
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				face->glyph->advance.x
			};

			Characters.insert(std::pair<unsigned char, Character>(i, character));
		}

		//clean up freetype resource
		clean();

		return true;
	}

	//This function for cleaning up the freetype resource
	void Text::clean()
	{
		FT_Done_Face(face);
		FT_Done_FreeType(ft);
	}

}
