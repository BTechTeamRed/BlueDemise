#include "Text.h"
#include <Engine/Utilities/Log.h>

#include FT_FREETYPE_H

namespace Engine 
{
	void Text::IterateText()
	{
		if (FT_Init_FreeType(&ft))
		{
			GE_CORE_ERROR("Could not init FreeType Library");
		}

		if (FT_New_Face(ft, "Cousine-Regular.ttf", 0, &face))
		{
			GE_CORE_ERROR("Failed to load font");
		}
		//disable byte-alignment restriction from opengl
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		//In this for loop, iterate through all 128 characters in the ASCll set
		for (unsight char i = 0; i < 128; i++)
		{
			//load character glyph
			if (FT_Load_Char(face, charID,FT_LOAD_RENDER))
			{
				GE_CORE_INFO("Error: failed to load Glyph");
				continue;
			}	
		}

		//Generate texture
		unsigned int texture;
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
		Characters.insert(std::pair<char, Character>(c, character));
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}
