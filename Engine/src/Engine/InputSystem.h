#pragma once
#include <map>
#include <mutex>
#include "GLFW/glfw3.h"
#include "EventSystem.h"

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
		enum MouseButton
		{
			MOUSE1, MOUSE2, MOUSE3, MOUSE4, MOUSE5
		};

		//Delete copy c'tor and assignment opertor for singleton pattern
		InputSystem(InputSystem& other) = delete;
		void operator=(InputSystem& other) = delete;

		/// <summary>
		/// Updates the values of the maps to false once per frame
		/// </summary>
		void update();
		/// <summary>
		/// Checks for a mouse button press
		/// </summary>
		/// <param name="button">Mouse button to check</param>
		/// <returns>If the button has been pressed</returns>
		bool isButtonPressed(MouseButton button);
		/// <summary>
		/// Checks for a key press
		/// </summary>
		/// <param name="key">Key to check</param>
		/// <returns>If the key has been pressed</returns>
		bool isKeyPressed(char key);
		/// <summary>
		/// Set the current window for context
		/// </summary>
		/// <param name="window">The active window</param>
		void setWindow(GLFWwindow* window);

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
		std::map<MouseButton, bool> m_mouseMap;
		/// <summary>
		/// Current active window context
		/// </summary>
		GLFWwindow* m_window;
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