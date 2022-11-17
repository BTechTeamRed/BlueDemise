#pragma once
#include <vector>
#include <Engine/SceneBuilder/Components.h>
#include <Engine/Core.h>

namespace Engine
{
	class ENGINE_API GeometryFactory
	{
	public:
		enum Geometry
		{
			RT_Sprite
		};

		#pragma region Singleton Instance Management
		//Gets the singleton instance
		static GeometryFactory* getInstance();


		GeometryFactory(GeometryFactory& other) = delete;

		//Singletons should not be assignable, this is to prevent that.
		void operator=(const GeometryFactory&) = delete;
		#pragma endregion
		
		#pragma region Geometry Generation
		//Obtain geometry based on provided geometry type. Returns a vertices component containing geometry.
		VerticesComponent getVerticesComponent(Geometry geometry);

		//Generate a new piece of geometry with unique VBO, VAO and IBOs based on provided vertices and indices.
		VerticesComponent generateVerticesComponent(float vertices[], int verticesSize, float indices[], int indicesSize, GLsizei stride);

		//Generate VBO/VAO/IBO based on provided data.
		GLuint getVBO(float vertices[], int verticesSize, GLsizei stride, std::vector<VertexAttribute> vertexAttributes);
		GLuint getVAO();
		GLuint getIBO(unsigned int* indices, int indicesSize);
		#pragma endregion
	private:

		#pragma region Singleton Management
		// The singleton instance
		static GeometryFactory* m_instance;
		GeometryFactory();
		#pragma endregion

		std::unordered_map<Geometry, VerticesComponent> defaultGeometry;

		#pragma region Default Geometry Initialization
		//Initializes the sprite vao, vbo, and ibo
		void initSpriteGeometry();
		#pragma endregion
	};
}


