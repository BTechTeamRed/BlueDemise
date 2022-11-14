#include "Window.h"
#include "Engine/Utilities/Log.h"

namespace Engine {
	Window::Window()
	{
		m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, "BlueDemise", nullptr, nullptr); //switch to unique ptr with deleter for RAII?
		if (m_window == nullptr)
		{
			GE_CORE_ERROR("Failed to create GLFW window");
			glfwTerminate();
		}

		m_UIwindow = glfwCreateWindow(m_windowWidth, m_windowHeight, "User Interface", nullptr, nullptr); //switch to unique ptr with deleter for RAII?
		if (m_UIwindow == nullptr)
		{
			GE_CORE_ERROR("Failed to create GLFW window (UI)");
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
		float frustumWidth = m_camera->getComponent<CameraComponent>().frustumWidth;
		float aspectRatio = m_camera->getComponent<CameraComponent>().aspectRatio;
		return glm::vec3(screenSpaceVector.x / m_windowWidth * frustumWidth, screenSpaceVector.x / m_windowWidth * frustumWidth * aspectRatio, 1);
	}
	glm::vec2 Window::worldSpaceToScreenSpace(const glm::vec3& worldSpaceVector)
	{
		float frustumWidth = m_camera->getComponent<CameraComponent>().frustumWidth;
		float aspectRatio = m_camera->getComponent<CameraComponent>().aspectRatio;
		return glm::vec2(worldSpaceVector.x / m_windowWidth * frustumWidth, worldSpaceVector.x / m_windowWidth * frustumWidth * aspectRatio);
	}

	glm::mat4 Window::getProjectionMatrix() const
	{
		glm::vec3 cameraPos = m_camera->getComponent<TransformComponent>().position;
		return m_camera->getComponent<CameraComponent>().projection * glm::translate(glm::mat4(1), cameraPos);
	}
	float Window::getAspectRatio() const
	{
		return m_camera->getComponent<CameraComponent>().aspectRatio;
	}
}