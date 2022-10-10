#pragma once
#include "glm/glm.hpp"
#include "GLFW/glfw3.h"
#include <vector>

/// Container file for all components.
///	As per the Entt specification, components are structs with data.
///	Components shouldn't have any methods that define any kind of behavior.
namespace Engine 
{
	struct TagComponent
	{
		std::string tag;
	};
	
	//A component for storing the matrices of a camera, and distance/fov.
	struct CameraComponent 
	{
		CameraComponent(float f, glm::mat4 proj, glm::vec2 view, float fZ, float nZ)
			: fov(f),  projection(proj), viewport(view), farZ(fZ), nearZ(nZ) {}

		float fov;
		glm::mat4 projection;
		glm::vec2 viewport;
		float farZ;
		float nearZ;
	};

	//A component containing the 'transform' of a component (position in the world).
	struct TransformComponent 
	{
		TransformComponent() = default;
		TransformComponent(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation)
			: position(position), scale(scale), rotation(rotation) {}

		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 rotation;
	};

	//A component containing a vec4 of color data, RGBA.
	struct ColorComponent 
	{
		ColorComponent() = default;
		ColorComponent(glm::vec4 color)
			: color(color) {}

		glm::vec4 color;
	};

	//A component containing sprite data.
	struct SpriteComponent
	{
		SpriteComponent() = default;
		SpriteComponent(GLuint texture)
			: texture(texture) {}

		GLuint texture;

		float vertices[20] =
		{
			// XYZ UV (UV is for texture mapping, to access [X, Y] part of the sheet)
			-1.f, -1.f, 0.f, 0.f, 1.f, //bottom left
			1.f, 1.f, 0.f, 1.f, 0.f, //top right
			1.f, -1.f, 0.f, 1.f, 1.f, //bottom right
			-1.f, 1.f, 0.f, 0.f, 0.f, //top left
		};

		//*NOTE: Use seperate VBO (for UV verts) to modify what is access from a spritesheet

		//Order to draw vertices.
		unsigned int indices[6] = { 0, 1, 2, 0, 1, 3 };
	};

	//A component containing texture data
	struct TextureComponent
	{
		TextureComponent() = default;
		TextureComponent(GLuint texID)
			: texID(texID) {}

		GLuint texID;
	};

	//Not component, just container for vertex attribute data format
	struct VertexAttribute 
	{
		VertexAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei pointer) 
			: index(index), size(size), type(type), normalized(normalized), pointer(pointer), vbo(-1) {}

		GLuint index;
		GLint size;
		GLenum type;
		GLboolean normalized;
		GLsizei pointer;
		GLuint vbo;
	};

	//under this definition, vertex data is only ever stored on the gpu in the vao. It doesn't exist in the ECS. Not sure if this is optimal.
	//Contains data to communicate with the GPU about what to draw (typically per entity).
	struct VerticesComponent 
	{
		VerticesComponent() = default;
		VerticesComponent(const VerticesComponent& other) = default;

		std::vector<VertexAttribute> vertexAttributes;

		//
		GLuint vaoID;

		//Indices buffer object that reference specific vertices in the VBO. 'Draw everything in the VBO using IBO'.
		GLuint iboID;
		
		//Vertex buffer object ID: ID for the buffer containing the verts on the GPU
		GLuint vboID;
		
		//Size of a single vertex in bytes
		GLsizei stride;

		//Num of vertices provided to GPU
		unsigned long numIndices;
	};
}