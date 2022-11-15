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
	private:
		/// The singleton instance
		static GeometryFactory* m_instance;

		//Buffers specialized for the sprites
		VertexBuffer m_spriteVBO;
		IndexBuffer m_spriteIBO;
		VertexArray m_spriteVAO;
	public:
		//Gets the singleton instance
		static GeometryFactory* getInstance();
		//Gets a reference to the sprite Vertex Buffer
		VertexBuffer& getSpriteVBO();
		//Gets a reference to the sprite Index Buffer
		IndexBuffer& getSpriteIBO();
		//Gets a reference to the sprite Vertex Array
		VertexArray& getSpriteVAO();
	private:
		GeometryFactory();

		//Initializes the sprite vao, vbo, and ibo
		void initSpriteGeometry();
	};
}


