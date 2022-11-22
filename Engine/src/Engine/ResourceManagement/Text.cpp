#pragma once
#include "glad/glad.h"
#include "Text.h"

#include "Engine/ResourceManagement/ResourceManager.h"
#include "Engine/Rendering/GeometryFactory.h"

#include <Engine/Utilities/Log.h>
#include <freetype/freetype.h>
#include <Engine/ResourceManagement/ShaderNorms.h>

namespace Engine 
{
	
	//Iterate through 128 ASCII characters and generate textures for each to be rendered.
	bool Text::initializeText()
	{
		//Freetype resources used to generate character textures.
		FT_Face face;
		FT_Library ft;

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
		for (unsigned char ch = 0; ch < 128; ch++)
		{
			//load character glyph
			if (FT_Load_Char(face, ch, FT_LOAD_RENDER))
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

			Characters.insert(std::pair<unsigned char, Character>(ch, character));
		}

		m_textShaderProgram = ShaderNorms::getInstance()->getShader(ShaderNorms::SN_TEXT_FILL);
		m_textVertices = GeometryFactory::getInstance()->getVerticesComponent(GeometryFactory::RT_Text);

		//clean up freetype resource
		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		return true;
	}
}