#pragma once

#include "Engine/SceneBuilder/Entity.h"
#include "glm/ext/matrix_clip_space.hpp"

namespace Engine
{
	//Define renderer as forward declaration to ignore include until cpp
	class Renderer;

	class ENGINE_API Window
	{
	private:
		//Dimensions of window
		int m_windowWidth{ 1920 };
		int m_windowHeight{ 1080 };

		//FBO definition to render all draw calls to a single texture, for future use.
		GLuint m_fboID;
		GLuint m_fboTextureID;
		GLuint m_rboID;

		//GLFW window pointer, our target rendering object
		struct GLFWwindow* m_window;

	public:
		Window() {};

		//Camera component is initialized with default component. This should be updated ASAP
		CameraComponent m_camera{ CameraComponent{} };
		
		#pragma region Window handling
		//Update camera component of window when camera is changed so the window can update its projection matrix.
		void updateCamera(CameraComponent newCamera);

		//Resize the window to the given dimensions
		void resize(int width, int height);

		//Initialize the window class and return the status of initialization.
		bool initialize();
		#pragma endregion

		#pragma region FBO handling
		//Frame buffer handling
		void initFrameBuffer();
		void bindFrameBuffer();
		void unBindFrameBuffer();
		#pragma endregion

		#pragma region Getters
		//Converts a vector in screenspace pixel units to worldspace units, offset defaults to zero vector (don't use offset for sizes, only positions)
		glm::vec3 screenSpaceToWorldSpace(const glm::vec2& screenSpaceVector, const glm::vec2 debugWindowOffset);

		//Converts a worldspace vector to screenspace pixel units, offset defaults to zero vector (don't use offset for sizes, only positions)
		glm::vec2 worldSpaceToScreenSpace(const glm::vec3& worldSpaceVector, const glm::vec2 debugWindowOffset);

		//Return a pointer to the current GLFW window
		GLFWwindow* getWindow() const { return m_window; }

		//Get width/height of window.
		int getHeight() const { return m_windowHeight; }
		int getWidth() const { return m_windowWidth; }
		GLuint getFboId() const { return m_fboID;  }

		//Get's the projection matrix of the camera
		glm::mat4 getProjectionMatrix() const;

		//Get's the projection matrix of the camera
		float getAspectRatio() const;
		#pragma endregion
		
		friend Renderer;
	};
}

