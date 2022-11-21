#include "Engine/Core.h"
#include "glm/gtc/type_ptr.hpp"

#include <freetype/freetype.h>

#include <map>
#include <string>

namespace Engine
{
	typedef unsigned int GLuint;

	class ENGINE_API Text
	{
	public:
		Text();

	private:
		//Iterate through 128 ASCII characters and generate textures for each to be rendered.
		bool initializeText();

		//Freetype resources used to generate character textures.
		FT_Face face;
		FT_Library ft;

		std::string defaultFont = "MyriadPro.otf";

		//Struct to store character render data.
		struct Character
		{
			GLuint			TextureID; //ID handle of the glyph texture
			glm::ivec2		Size;      //Size of glyph
			glm::ivec2		Bearing;   //offset from baseline to left/top of glyph	
			unsigned int	Advance;   //Offset to advance to next glyph
		};

		//initialize the structure
		void init(const char* fname, unsigned int h);

		//clean the resource
		void clean();
		
		//generate a texture and store its relevant data into a Character struct 
		//that we add to the Characters map, all data require to render for later use
		std::map<unsigned char, Character> Characters;
	};
}