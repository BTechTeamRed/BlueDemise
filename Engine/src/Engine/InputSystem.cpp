#include "Engine/InputSystem.h"

using namespace Engine;

InputSystem* InputSystem::m_instance = nullptr;
std::mutex InputSystem::m_instanceMutex;

InputSystem::InputSystem():m_window(nullptr), m_isInit(false)
{
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
}

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
		//glfwSetKeyCallback(window, );
	}
}

bool InputSystem::isButtonPressed(int button)
{
	return m_mouseMap[button];
}

bool InputSystem::isKeyPressed(int key)
{
	return m_keyMap[key];
}

void InputSystem::setWindow(GLFWwindow* window)
{
	m_window = window;
}

void InputSystem::mouseCallback(GLFWwindow* window, int button, int action, int mod)
{
	m_mouseMap[button] = action == GLFW_PRESS;
}

void InputSystem::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	m_keyMap[key] = action == GLFW_PRESS;
}