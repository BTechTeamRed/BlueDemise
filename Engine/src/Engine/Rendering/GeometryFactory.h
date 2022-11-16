#pragma once
#include <vector>
#include <Engine/SceneBuilder/Components.h>
namespace Engine {

	//Temp, delete when snow implements theese classes
	//class VertexBuffer;
	//class IndexBuffer;
	//class VertexArray;

	class GeometryFactory
	{
	public:
		enum Geometry
		{
			RT_Sprite
		};

		//Gets the singleton instance
		static GeometryFactory* getInstance();
		GeometryFactory(GeometryFactory& other) = delete;

		//Singletons should not be assignable, this is to prevent that.
		void operator=(const GeometryFactory&) = delete;
		
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
		/// The singleton instance
		static GeometryFactory* m_instance;
		VerticesComponent m_sprite;

		//Buffers specialized for the sprites. These would eventually be changed into the classes per buffer (VertexBuffer,IndexBuffer,VertexArray)
		GLuint m_spriteVBO;
		GLuint m_spriteIBO;
		GLuint m_spriteVAO;

		GeometryFactory();

		//Initializes the sprite vao, vbo, and ibo
		void initSpriteGeometry();
	};
}


