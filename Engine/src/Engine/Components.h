#pragma once
#include <glm.hpp>
#include <GLFW/glfw3.h>>
#include <vector>

struct CameraComponent {
	float fov;
	glm::mat4 projection;
	glm::vec2 viewport;
	float farZ;
	float nearZ;
};

struct TransformComponent {
	glm::vec3 position;
	glm::vec3 position;
	glm::vec3 rotation;
};

struct ColorComponent {
	glm::vec4 color;
};


//Not component, just container for vertex data
struct VertexAttributes {
	const char* name;
	bool enabled;
	GLuint indx;
	GLint size;
	GLenum type;
	GLboolean normalized;
	GLsizei stride;
	GLuint vbo;
};

struct VerticesComponent {
	std::vector<VertexAttributes> vertices;
	GLuint vaoID;
	GLuint iboID;
	GLuint programID;
	unsigned long numIndices;
};

