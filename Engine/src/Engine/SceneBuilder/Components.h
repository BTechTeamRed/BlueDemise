#pragma once
#include "glm/glm.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "Engine/ResourceManagement/ShaderNorms.h"
#include "GLFW/glfw3.h"
#include <vector>
#include <string>
#include <functional>

/// Container file for all components.
///	As per the Entt specification, components are structs with data.
///	Components shouldn't have any methods that define any kind of behavior.
namespace Engine 
{
	class ScriptableBehavior;

	struct TagComponent
	{
		std::string tag;
	};
	
	//A component for storing the matrices of a camera, and distance/fov.
	struct CameraComponent 
	{
		CameraComponent() = default;
		CameraComponent(float frustumWidth, float aspectRatio, float farZcoordinate, float nearZcoordinate)
			: frustumWidth(frustumWidth), aspectRatio(aspectRatio), farZ(farZcoordinate), nearZ(nearZcoordinate) 
		{	
			projection = glm::ortho(0.f, frustumWidth, frustumWidth/aspectRatio, 0.f, nearZ, farZ);
		}

		glm::mat4 projection;
		float frustumWidth;
		float aspectRatio;
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

	struct FixedScreenTransformComponent {
		FixedScreenTransformComponent(glm::vec3 position, glm::vec3 scale)
			: position(position), scale(scale)
		{
		}
		glm::vec2 position;
		glm::vec2 scale;
	};

	//A component containing a vec4 of color data (RGBA), and a GLuint for the texture and shader used for the material.
	struct MaterialComponent
	{
		MaterialComponent() = default;
		//MaterialComponent(glm::vec4 color, GLuint texID, std::string texName, GLuint shaderId)
			//: color(color), texID(texID), texName(texName){}
		MaterialComponent(glm::vec4 color, GLuint texID, std::string texName, std::string shaderName)
			: color(color), texID(texID), texName(texName), shaderName(shaderName)
		{
			bind = shadersInstantiated++;
			ShaderNorms::getInstance()->addAdvancedShader(bind, shaderName);
		}
		
		glm::vec4 color{ 1.f,1.f,1.f,1.f };
		std::string texName;
		GLuint texID;
		std::string shaderName;
		inline static int shadersInstantiated{ 0 };
		int bind;
	};

	//A component containing animation data
	struct AnimationComponent
	{
		AnimationComponent() = default;
		AnimationComponent(GLuint texID, float frameRate, float texWidthFraction, float texHeightFraction, int numPerRow, int spritesOnSheet)
			: texID(texID), frameRate(frameRate), texWidthFraction(texWidthFraction), texHeightFraction(texHeightFraction), numPerRow(numPerRow), spritesOnSheet(spritesOnSheet) { }
		
		std::vector<glm::vec2> texCoords;

		GLuint texID;
		std::string texName;

		int currentIndex = 0;
		int numPerRow;
		int spritesOnSheet;
		
		double animationSpeed = 0.3;

		float deltaTime;
		float frameRate;
		float texWidthFraction;
		float texHeightFraction;
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

		//Vertex array object, which acts as a wrapper for VBO data
		GLuint vaoID;

		//Indices buffer object that reference specific vertices in the VBO. 'Draw everything in the VBO using IBO'.
		GLuint iboID;
		
		//Vertex buffer object ID: ID for the buffer containing the verts on the GPU
		GLuint vboID;
		
		//Size of a single vertex in bytes
		GLsizei stride;

		//Num of vertices provided to GPU
		unsigned long numIndices;

		bool isSprite;
	};

	//Entities with this component will be serialized by Serializer.cpp
	struct SerializableComponent
	{
		//struct can't be empty because of how entt is built.
		//Added smallest type as a workaround.
		bool serializable;
	};

	//Defines a component to create custom script actions using ScriptableBehavior
	struct ScriptComponent
	{
		ScriptableBehavior* m_instance{ nullptr };

		std::function<ScriptableBehavior*()> instantiateScript;
		std::function<void()> destroyScript;

		//Links the Script Component to a specific ScriptableBehavior
		template<typename T>
		void linkBehavior()
		{
			instantiateScript = [] { return static_cast<ScriptableBehavior*>(new T()); };
			destroyScript = [this] { delete m_instance; m_instance = nullptr; };
		}
	};
}