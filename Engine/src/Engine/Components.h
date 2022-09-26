#pragma once
#include "glm/glm.hpp"
#include <GLFW/glfw3.h>	
#include <vector>

namespace Engine 
{
	struct TagComponent
	{
		std::string tag;
	};
	
	struct CameraComponent 
	{
	float fov;
	glm::mat4 projection;
	glm::vec2 viewport;
	float farZ;
	float nearZ;
	};

struct TransformComponent 
{
	TransformComponent(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation) : position(position), scale(scale), rotation(rotation) 
	{

	}
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
};

struct ColorComponent 
{
	ColorComponent(glm::vec4 color) : color(color) 
	{

	}
	glm::vec4 color;
};


	//Not component, just container for vertex attribute data format
	struct VertexAttribute 
	{
		VertexAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei pointer) 
			: index(index), size(size), type(type), normalized(normalized), pointer(pointer)
		{}

		GLuint index;
		GLint size;
		GLenum type;
		GLboolean normalized;
		GLsizei pointer;
	};

	//under this definition, vertex data is only ever stored on the gpu in the vao. It doesn't exist in the ECS. Not sure if this is optimal.
	struct VerticesComponent 
	{
		std::vector<VertexAttribute> vertexAttributes;
		GLuint vaoID;
		GLuint iboID;
		GLuint programID;
		GLsizei stride;
		unsigned long numIndices;
	};
}