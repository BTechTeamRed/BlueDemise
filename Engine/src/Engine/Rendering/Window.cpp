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
		float newAspectRatio = width / height;
		if()
	}

	glm::mat4 Window::getProjectionMatrix() const
	{
		return m_camera->getComponent<CameraComponent>().projection;
	}
}