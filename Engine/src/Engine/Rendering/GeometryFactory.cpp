#include "GeometryFactory.h"

namespace Engine {
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

		std::vector<VertexAttribute> vertexAttributes;

		vertexAttributes.push_back(VertexAttribute(0, 3, GL_FLOAT, GL_FALSE, 0));
		vertexAttributes.push_back(VertexAttribute(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * stride));

		//Create VBO, IBO, VAO from the spriteVerticies and indicies
		m_spriteVAO = getVAO();//VertexArray();
		m_spriteVBO = getVBO(vertices, (GLsizei)stride, m_spriteVAO, vertexAttributes);//VertexBuffer(vertices, stride, m_spriteVAO, vertexAttributes);
		m_spriteIBO = getIBO(indices, m_spriteVAO);//IndexBuffer(indices, m_spriteVAO);


		m_sprite.stride = sizeof(float) * stride;
		m_sprite.numIndices = sizeof(indices) / sizeof(indices[0]);

		m_sprite.vaoID = GeometryFactory::getInstance()->getSpriteVAO();
		m_sprite.vboID = GeometryFactory::getInstance()->getSpriteVBO();
		m_sprite.iboID = GeometryFactory::getInstance()->getSpriteIBO();
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
	GLuint GeometryFactory::getVBO(float* vertices, GLsizei stride, GLuint m_spriteVAO, std::vector<VertexAttribute> vertexAttributes)
	{
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		
		for (int i = 0; i < vertexAttributes.size(); i++)
		{
			const auto attribute = vertexAttributes[i];
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(attribute.index, attribute.size, attribute.type, attribute.normalized, stride, (const void*)attribute.pointer);
		}
		
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

		glBindVertexArray(0);
		
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
	GLuint GeometryFactory::getIBO(unsigned int* indices, GLuint vao)
	{
		GLuint ibo;
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
		return ibo;
	}
}