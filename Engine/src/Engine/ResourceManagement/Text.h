#include <Engine/Utilities/Log.h>

//Freetype Header file
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#include FT_FREETYPE_H

//OpenGl Header file
#include <gl/GL.h>
#include <gl/GLU.h>

//STL Header file
#include <vector>
#include <map>
#include <string>

GLuint textureID[Max_NO_TEXTURES];

namespace Engine
{
	struct Character
	{
		//GLuint			TextureID; //ID handle of the glyph texture
		//glm::ivec2		Size;      //Size of glyph
		//glm::ivec2		Bearing;   //offset from baseline to left/top of glyph	
		//GLuint			Advance;   //Offset to advance to next glyph

		float		height;	  // the height of the text
		GLuint*		textures; // used textures
		GLuint		list_base // the value of the list
	};

	//initialize the structure
	void init(const char* fname, unsigned int h);

	//clean the resource
	void clean();

	//draw the texture to the screen
	//void print(const Character& ft_text, float x, float y, const char* fmt);
	/*std::map<GLchar, Character> Characters;*/
}