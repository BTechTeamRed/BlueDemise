
#include "glad/glad.h"
#include "GeometryFactory.h"
#include "Engine/Utilities/Log.h"

namespace Engine 
{

	GeometryFactory* GeometryFactory::m_instance{ nullptr };
	
	GeometryFactory* GeometryFactory::getInstance()
	{
		if (!m_instance)
		{
			m_instance = new GeometryFactory();
		}

		return m_instance;
	}

	GeometryFactory::GeometryFactory() 
	{
		initSpriteGeometry();
	}

	VerticesComponent GeometryFactory::getVerticesComponent(Geometry geometry)
	{
		VerticesComponent vc;

		switch (geometry)
		{
			case RT_Sprite:
			{
				vc = m_sprite;
				break;
			}
			default:
				break;
		}
		
		return vc;
	}

	void GeometryFactory::initSpriteGeometry()
	{
		//Might be 3 ***********
		int stride = 5;

		float vertices[] =
		{
			// positions  // texture coords (UV coords)
			0.f, 0.f, 0.f,  0.f, 0.f,  // top left
			1.f, 0.f, 0.f,  1.f, 0.f,  // top right
			1.f, 1.f, 0.f,  1.f, 1.f,  // bottom right
			0.f, 1.f, 0.f,  0.f, 1.f,  // bottom left
		};

		unsigned int indices[6] =
		{
			0, 1, 2,  //first triangle
			2, 3, 0,  //second triangle
		};
		
		m_sprite.vertexAttributes.push_back(VertexAttribute(0, 3, GL_FLOAT, GL_FALSE, 0));
		m_sprite.vertexAttributes.push_back(VertexAttribute(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 3));

		m_sprite.stride = sizeof(float) * stride;
		m_sprite.numIndices = 6; // sizeof(indices) / sizeof(indices[0]);
		
		
		//Create VBO, IBO, VAO from the spriteVerticies and indicies
		//m_spriteVAO = getVAO();//VertexArray();
		//m_spriteVBO = getVBO(vertices, (GLsizei)stride, m_spriteVAO, m_sprite);//VertexBuffer(vertices, stride, m_spriteVAO, vertexAttributes);
		//m_spriteIBO = getIBO(indices, m_spriteVAO);//IndexBuffer(indices, m_spriteVAO);


		m_sprite.vaoID = getVAO();//VertexArray();
		m_sprite.vboID = getVBO(vertices, (GLsizei)stride, m_sprite.vaoID, m_sprite);
		m_sprite.iboID = getIBO(indices, m_sprite.vaoID);
	}

	GLuint& GeometryFactory::getSpriteVBO()
	{
		return m_spriteVBO;
	}
	GLuint& GeometryFactory::getSpriteIBO()
	{
		return m_spriteIBO;
	}
	GLuint& GeometryFactory::getSpriteVAO()
	{
		return m_spriteVAO;
	}

	//Return the VBO for sprites. If it doesn't exist, create it.
	GLuint GeometryFactory::getVBO(float* vertices, GLsizei stride, GLuint m_spriteVAO, VerticesComponent& sprite)
	{
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
		
		for (int i = 0; i < sprite.vertexAttributes.size(); i++)
		{
			const auto attribute = sprite.vertexAttributes[i];
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(attribute.index, attribute.size, attribute.type, attribute.normalized, stride * sizeof(float), (const void*)attribute.pointer);
		}

		float t_vertices[] =
		{
			// positions  // texture coords (UV coords)
			0.f, 0.f, 0.f,  0.f, 0.f,  // top left
			1.f, 0.f, 0.f,  1.f, 0.f,  // top right
			1.f, 1.f, 0.f,  1.f, 1.f,  // bottom right
			0.f, 1.f, 0.f,  0.f, 1.f,  // bottom left
		};
		
		glBufferData(GL_ARRAY_BUFFER, sizeof(t_vertices), t_vertices, GL_DYNAMIC_DRAW);

		//glBindVertexArray(0);
		
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
	GLuint GeometryFactory::getIBO(unsigned int* indices, GLuint& vao)
	{
		GLuint ibo;
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		unsigned int t_indices[6] =
		{
			0, 1, 2,  //first triangle
			2, 3, 0,  //second triangle
		};
		
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(t_indices), t_indices, GL_DYNAMIC_DRAW);
		return ibo;
	}
}