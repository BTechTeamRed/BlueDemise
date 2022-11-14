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

	GeometryFactory::GeometryFactory() {
		initSpriteGeometry();
	}

	void GeometryFactory::initSpriteGeometry()
	{
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
		vertexAttributes.push_back(VertexAttribute(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 3));

		//Create VBO, IBO, VAO from the spriteVerticies and indicies
		m_spriteVAO = VertexArray();
		m_spriteVBO = VertexBuffer(vertices, m_spriteVAO, vertexAttributes);
		m_spriteIBO = IndexBuffer(indices, m_spriteVAO);
	}

	VertexBuffer& GeometryFactory::getSpriteVBO()
	{
		return m_spriteVBO;
	}
	IndexBuffer& GeometryFactory::getSpriteIBO()
	{
		return m_spriteIBO;
	}
	VertexArray& GeometryFactory::getSpriteVAO()
	{
		return m_spriteVAO;
	}
}