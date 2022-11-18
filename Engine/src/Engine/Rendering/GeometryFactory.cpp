
#include "glad/glad.h"
#include "GeometryFactory.h"
#include "Engine/Utilities/Log.h"

namespace Engine 
{
#pragma region Singleton Instance Management
	GeometryFactory* GeometryFactory::m_instance{ nullptr };
	
	GeometryFactory* GeometryFactory::getInstance()
	{
		if (!m_instance)
		{
			m_instance = new GeometryFactory();
		}

		return m_instance;
	}

	//Constructor that instantiates every piece of geometry this class would generate.
	GeometryFactory::GeometryFactory() 
	{
		initSpriteGeometry();
	}
#pragma endregion

#pragma region Buffer Generation
	//Return geometry based on provided geometry type.
	VerticesComponent GeometryFactory::getVerticesComponent(Geometry geometry)
	{
		return m_defaultGeometry[geometry];
	}

	//Return the VBO for sprites. If it doesn't exist, create it.
	GLuint GeometryFactory::getVBO(float vertices[], int verticesSize, GLsizei stride, std::vector<VertexAttribute>& vertexAttributes)
	{
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		
		glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_DYNAMIC_DRAW);

		for (int i = 0; i < vertexAttributes.size(); i++)
		{
			const auto attribute = vertexAttributes[i];
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(attribute.index, attribute.size, attribute.type, attribute.normalized, stride * sizeof(float), (const void*)attribute.pointer);
		}

		return vbo;
	}

	//Return the VAO for sprites. If it doesn't exist, create it.
	GLuint GeometryFactory::getVAO()
	{
		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		return vao;
	}

	//Return the IBO for sprites. If it doesn't exist, create it.
	GLuint GeometryFactory::getIBO(unsigned int* indices, int indicesSize)
	{
		GLuint ibo;
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_DYNAMIC_DRAW);
		return ibo;
	}
#pragma endregion

#pragma region Geometry Generation
	void GeometryFactory::initSpriteGeometry()
	{
		VerticesComponent m_sprite;

		//Stride is the number of coords per row of vertices (in our case, XYZ position and XY UV coords)
		int stride = 5;

		//Definition of dimensions of sprite.
		float vertices[] =
		{
			// positions  // texture coords (UV coords)
			0.f, 0.f, 0.f,  0.f, 0.f,  // top left
			1.f, 0.f, 0.f,  1.f, 0.f,  // top right
			1.f, 1.f, 0.f,  1.f, 1.f,  // bottom right
			0.f, 1.f, 0.f,  0.f, 1.f,  // bottom left
		};

		//Definition of draw order for vertices.
		unsigned int indices[6] =
		{
			0, 1, 2,  //first triangle
			2, 3, 0,  //second triangle
		};

		//Based on the stride, we can determine the number of attributes.
		m_sprite.vertexAttributes.push_back(VertexAttribute(0, 3, GL_FLOAT, GL_FALSE, 0));
		m_sprite.vertexAttributes.push_back(VertexAttribute(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 3));

		m_sprite.stride = sizeof(float) * stride;
		m_sprite.numIndices = sizeof(indices) / sizeof(indices[0]);

		m_sprite.vaoID = getVAO();//VertexArray();
		m_sprite.vboID = getVBO(vertices, sizeof(vertices), (GLsizei)stride, m_sprite.vertexAttributes);
		m_sprite.iboID = getIBO(indices, sizeof(indices));

		m_defaultGeometry.insert(std::pair(RT_Sprite, m_sprite));
	}
#pragma endregion
}