#pragma once

#include "Engine/SceneBuilder/Entity.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace Engine {

	class Renderer; //forward declaration

	class Window
	{
	private:
		int m_windowWidth{ 1920 };
		int m_windowHeight{ 1080 };
		struct GLFWwindow* m_window;
		struct GLFWwindow* m_UIwindow;

	public:
		Window();

		void initialize();

		void resize(int width, int height);

		GLFWwindow* getWindow() const { return m_window; }
		
		//Converts a vector in screenspace pixel units to worldspace units
		glm::vec3 screenSpaceToWorldSpace(const glm::vec2& screenSpaceVector);

		//Converts a worldspace vector to screenspace pixel units
		glm::vec2 worldSpaceToScreenSpace(const glm::vec3& worldSpaceVector);

		//Get's the projection matrix of the camera
		glm::mat4 getProjectionMatrix() const;

		//Get's the projection matrix of the camera
		float getAspectRatio() const;

		CameraComponent& m_camera;
		
		friend Renderer;
	};
}

