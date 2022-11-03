#pragma once
#include <map>
#include <mutex>

namespace Engine {
	/// <summary>
	/// Provides a wrapper for GLFW input
	/// </summary>
	class InputSystem
	{
	public:
		/// <summary>
		/// Enumeration of mouse button keys
		/// </summary>
		enum MouseKey
		{
			MOUSE1, MOUSE2, MOUSE3, MOUSE4, MOUSE5
		};

		//Delete copy c'tor and assignment opertor for singleton pattern
		InputSystem(InputSystem& other) = delete;
		void operator=(InputSystem& other) = delete;

		/// <summary>
		/// Grabs the singleton instance
		/// </summary>
		/// <returns>The instance</returns>
		static InputSystem* getInstance();
	private:
		//Private c'tor for singleton
		InputSystem();

		/// <summary>
		/// Map of keys and whether they have been pressed in the frame
		/// </summary>
		std::map<char, bool> m_keyMap;
		/// <summary>
		/// Map of mouse buttons and whether they have been pressed
		/// </summary>
		std::map<MouseKey, bool> m_mouseMap;
		/// <summary>
		/// Mutex to protect the single instance
		/// </summary>
		static std::mutex m_instanceMutex;
		/// <summary>
		/// The singleton instance
		/// </summary>
		static InputSystem* m_instance;
	};
}