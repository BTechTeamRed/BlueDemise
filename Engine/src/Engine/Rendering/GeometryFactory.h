#pragma once
#include "glad/glad.h"
#include <vector>
#include <Engine/SceneBuilder/Components.h>
namespace Engine {

	//Temp, delete when snow implements theese classes
	class VertexBuffer;
	class IndexBuffer;
	class VertexArray;

	class GeometryFactory
	{
	public:
		enum Geometry
		{
			RT_Sprite
		};
	private:
		/// The singleton instance
		static GeometryFactory* m_instance;
		VerticesComponent m_sprite;

		//Buffers specialized for the sprites. These would eventually be changed into the classes per buffer (VertexBuffer,IndexBuffer,VertexArray)
		GLuint m_spriteVBO;
		GLuint m_spriteIBO;
		GLuint m_spriteVAO;
	public:
		//Gets the singleton instance
		static GeometryFactory* getInstance();
		VerticesComponent getVerticesComponent(Geometry geometry);
		//Gets a reference to the sprite Vertex Buffer
		GLuint& getSpriteVBO();
		//Gets a reference to the sprite Index Buffer
		GLuint& getSpriteIBO();
		//Gets a reference to the sprite Vertex Array
		GLuint& getSpriteVAO();

		GLuint getVBO(float* vertices, GLsizei stride, GLuint m_spriteVAO, std::vector<VertexAttribute> vertexAttributes);
		GLuint getVAO();
		GLuint getIBO(unsigned int* indices, GLuint& vao);
	private:
		GeometryFactory();

		//Initializes the sprite vao, vbo, and ibo
		void initSpriteGeometry();
	};
}


