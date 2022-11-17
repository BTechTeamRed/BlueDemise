#include <glad/glad.h>
#include "Window.h"
#include "Engine/Utilities/Log.h"

namespace Engine 
{
	bool Window::initialize() 
	{
		//Create window with GLFW, and name of "BlueDemise"
		m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, "BlueDemise", nullptr, nullptr); //switch to unique ptr with deleter for RAII?
		if (m_window == nullptr)
		{
			GE_CORE_ERROR("Failed to create GLFW window");
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(m_window);
		return true;
	}

	void Window::initFrameBuffer()
	{
		glGenFramebuffers(1, &m_fboID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
		glGenTextures(1, &m_fboTextureID);
		glBindTexture(GL_TEXTURE_2D, m_fboTextureID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_windowWidth, m_windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fboTextureID, 0);

		glGenRenderbuffers(1, &m_rboID);
		glBindRenderbuffer(GL_RENDERBUFFER, m_rboID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_windowWidth, m_windowHeight);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rboID);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			GE_CORE_ERROR("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	void Window::updateCamera(CameraComponent newCamera)
	{
		m_camera = newCamera;
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
		return m_camera.projection * glm::translate(glm::mat4(1.f), cameraPos);
	}

	float Window::getAspectRatio() const
	{
		return m_camera.aspectRatio;
	}

	void Window::bindFrameBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
	}

	void Window::unBindFrameBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glDisable(GL_DEPTH_TEST);
	}
	
}