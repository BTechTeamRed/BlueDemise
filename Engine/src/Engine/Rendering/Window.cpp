#include "Window.h"
#include "Engine/Utilities/Log.h"

namespace Engine {
	
	//Construct window class with a default camera component, which should be adjusted later upon scene initialization
	Window::Window()
	{
		//Create window with GLFW, and name of "BlueDemise"
		m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, "BlueDemise", nullptr, nullptr); //switch to unique ptr with deleter for RAII?
		if (m_window == nullptr)
		{
			GE_CORE_ERROR("Failed to create GLFW window");
			glfwTerminate();
		}

		glfwMakeContextCurrent(m_window);
	}

	void Window::resize(int width, int height)
	{
		//Adjust height to maintain current aspect ratio
		int newHeight = height * getAspectRatio(); 
		int letterboxHeight = height - newHeight;
		glViewport(0, letterboxHeight / 2, width, newHeight);
	}

	glm::vec3 Window::screenSpaceToWorldSpace(const glm::vec2& screenSpaceVector)
	{
		float frustumWidth = m_camera.frustumWidth;
		float aspectRatio = m_camera.aspectRatio;
		return glm::vec3(screenSpaceVector.x / m_windowWidth * frustumWidth, screenSpaceVector.x / m_windowWidth * frustumWidth * aspectRatio, 1);
	}
	glm::vec2 Window::worldSpaceToScreenSpace(const glm::vec3& worldSpaceVector)
	{
		float frustumWidth = m_camera.frustumWidth;
		float aspectRatio = m_camera.aspectRatio;
		return glm::vec2(worldSpaceVector.x / m_windowWidth * frustumWidth, worldSpaceVector.x / m_windowWidth * frustumWidth * aspectRatio);
	}

	//get position of camera in world space. Returns the projection matrix
	glm::mat4 Window::getProjectionMatrix() const
	{
		//Change this to use camera's proper transform component. Perhaps we can pass it as an argument? **********************
		glm::vec3 cameraPos = glm::vec3(0.f,0.f,-10.f);
		return m_camera.projection * glm::translate(glm::mat4(1), cameraPos);
	}

	float Window::getAspectRatio() const
	{
		return m_camera.aspectRatio;
	}
	
}