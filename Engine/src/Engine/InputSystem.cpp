#include "Engine/InputSystem.h"
#include "GLFW/glfw3.h"

using namespace Engine;

InputSystem* InputSystem::m_instance = nullptr;
std::mutex InputSystem::m_instanceMutex;


InputSystem* InputSystem::getInstance()
{
	std::unique_lock<std::mutex> lock;

	if (!m_instance)
	{
		m_instance = new InputSystem();
	}

	return m_instance;
}

void InputSystem::init(GLFWwindow* window)
{
	if (window != nullptr)
	{
		m_window = window;

		//Initialize cursor position as 0,0
		m_cursorPos = glm::vec2(0, 0);
		//Initialize keys
		//Number keys
		for (int key = GLFW_KEY_0; key <= GLFW_KEY_9; ++key)
		{
			m_keyMap[key];
		}
		//Letter keys
		for (int key = GLFW_KEY_A; key <= GLFW_KEY_Z; ++key)
		{
			m_keyMap[key];
		}
		//Initialize mouse buttons
		for (int button = GLFW_MOUSE_BUTTON_1; button <= GLFW_MOUSE_BUTTON_8; ++button)
		{
			m_mouseMap[button] = false;
		}

		//Set callbacks
		glfwSetWindowUserPointer(window, m_instance);
		auto mouseFunc = [](GLFWwindow* window, int button, int action, int mod)
		{
			static_cast<InputSystem*>(glfwGetWindowUserPointer(window))->mouseCallback(window, button, action, mod);
		};
		glfwSetMouseButtonCallback(m_window, mouseFunc);
		auto keyFunc = [](GLFWwindow* window, int key, int scancode, int action, int mod)
		{
			static_cast<InputSystem*>(glfwGetWindowUserPointer(window))->keyCallback(window, key, scancode, action, mod);
		};
		glfwSetKeyCallback(m_window, keyFunc);
		auto cursorFunc = [](GLFWwindow* window, double x, double y)
		{
			static_cast<InputSystem*>(glfwGetWindowUserPointer(window))->cursorCallback(window, x, y);
		};
		glfwSetCursorPosCallback(window, cursorFunc);
		m_isInit = true;
	}
}

bool InputSystem::isButtonPressed(int button)
{
	//Check for proper initialization
	return m_isInit && m_mouseMap[button];
}

bool InputSystem::isKeyPressed(int key)
{
	//Check for proper initialization
	return m_isInit && m_keyMap[key];
}

glm::vec2& InputSystem::getCursorPos()
{
	return m_isInit ? m_cursorPos : glm::vec2(0, 0);
}

void InputSystem::setWindow(GLFWwindow* window)
{
	m_window = window;
	m_isInit = false;
}

void InputSystem::mouseCallback(GLFWwindow* window, int button, int action, int mod)
{
	m_mouseMap[button] = action == GLFW_PRESS;
}

void InputSystem::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	m_keyMap[key] = action == GLFW_PRESS;
}

void InputSystem::cursorCallback(GLFWwindow* window, double xPos, double yPos)
{
	m_cursorPos.x = xPos;
	m_cursorPos.y = yPos;
}

bool InputSystem::isInit()
{
	return m_isInit;
}