#include "Engine/InputSystem.h"

using namespace Engine;

InputSystem* InputSystem::m_instance = nullptr;
std::mutex InputSystem::m_instanceMutex;

InputSystem::InputSystem() {}

InputSystem* InputSystem::getInstance()
{
	std::unique_lock<std::mutex> lock;

	if (!m_instance)
	{
		m_instance = new InputSystem();
	}

	return m_instance;
}
