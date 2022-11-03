#include "Engine/InputSystem.h"

using namespace Engine;

InputSystem* InputSystem::m_instance = nullptr;
std::mutex InputSystem::m_instanceMutex;

InputSystem::InputSystem():m_window(nullptr)
{
	//Initialize letter keys
	for (char key = 'a'; key <= 'z'; key++)
	{
		m_keyMap[key] = false;
	}
	//Initialize number keys
	for (char key = '0'; key <= '9'; key++)
	{
		m_keyMap[key] = false;
	}
	//Initialize mouse buttons
	m_mouseMap[MOUSE1] = false;
	m_mouseMap[MOUSE2] = false;
	m_mouseMap[MOUSE3] = false;
	m_mouseMap[MOUSE4] = false;
	m_mouseMap[MOUSE5] = false;
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

bool InputSystem::isButtonPressed(MouseButton button)
{
	return m_mouseMap[button];
}

bool InputSystem::isKeyPressed(char key)
{
	return m_keyMap[key];
}

void InputSystem::setWindow(GLFWwindow* window)
{
	m_window = window;
}