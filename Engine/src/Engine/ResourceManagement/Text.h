#pragma once
#include "Engine/Core.h"
#include "glm/gtc/type_ptr.hpp"

#include <Engine/SceneBuilder/Components.h>
#include <map>
#include <string>

namespace Engine
{
	typedef unsigned int GLuint;
	
	class ENGINE_API Text
	{
	public:
		enum Font
		{
			RT_Arial,
			RT_Daubmark,
			RT_Freedom,
			RT_JohnHenry,
			RT_MyriadPro,
			RT_times,
			RT_VioletWasteland
		};
		//Iterate through 128 ASCII characters and generate textures for each to be rendered.
		bool initializeText();

		//Contains Vertices used for each character to be rendered. Utilized in Renderer, when a VBO/VAO is required.
		VerticesComponent m_textVertices;

		//Identifier for the shader program of text components. Used when rendering text.
		GLuint m_textShaderProgram;

		//White space size
		float m_whiteSpaceSize;

		//Struct to store character render data.
		struct Character
		{
			GLuint TextureID; //ID handle of the glyph texture
			glm::ivec2 Size;      //Size of glyph
			glm::ivec2 Bearing;   //offset from baseline to left/top of glyph	
			unsigned int Advance;   //Offset to advance to next glyph
			
			int maxAscent;
			int maxDescent;
			int bitmap_top;
		};

		//generate a texture and store its relevant data into a Character struct 
		//that we add to the Characters map, all data require to render for later use
		std::map<char, Character> Characters;
	private:
		Font defaultFont = RT_VioletWasteland;

		static std::string parseFontToString(Font fontType)
		{
			if (fontType == RT_Arial) return"arial.ttf";
			if (fontType == RT_Daubmark) return"Daubmark.ttf";
			if (fontType == RT_Freedom) return"Freedom.ttf";
			if (fontType == RT_JohnHenry) return"JohnHenry.ttf";
			if (fontType == RT_MyriadPro) return"MyriadPro.otf";
			if (fontType == RT_times) return"times.ttf";
			if (fontType == RT_VioletWasteland) return"VioletWasteland.ttf";

			return "";
		}
	};
}
