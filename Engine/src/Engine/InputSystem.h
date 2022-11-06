#pragma once
#include <map>
#include <mutex>
#include "GLFW/glfw3.h"
#include "glm/vec2.hpp"

namespace Engine {
	/// <summary>
	/// Provides a wrapper for GLFW input
	/// </summary>
	class InputSystem
	{
	public:
		//Delete copy c'tor and assignment opertor for singleton pattern
		InputSystem(InputSystem& other) = delete;
		void operator=(InputSystem& other) = delete;

#pragma region Public Methods
		/// <summary>
		/// Initialize the input system
		/// </summary>
		/// <param name="window">Window context</param>
		void init(GLFWwindow* window);
		/// <summary>
		/// Checks for a mouse button press
		/// </summary>
		/// <param name="button">Mouse button to check</param>
		/// <returns>If the button has been pressed</returns>
		bool isButtonPressed(int button);
		/// <summary>
		/// Checks for a key press
		/// </summary>
		/// <param name="key">Key to check</param>
		/// <returns>If the key has been pressed</returns>
		bool isKeyPressed(int key);
		/// <summary>
		/// Set the current window for context, resets initialization to false
		/// </summary>
		/// <param name="window">The active window</param>
		void setWindow(GLFWwindow* window);
		/// <summary>
		/// Whether the InputSystem is initialized
		/// </summary>
		/// <returns>State of input system</returns>
		bool isInit();
		/// <summary>
		/// Getter for current position of the cursor
		/// </summary>
		/// <returns>The current position of the cursor if initialized</returns>
		glm::vec2& getCursorPos();
		/// <summary>
		/// Grabs the singleton instance
		/// </summary>
		/// <returns>The instance</returns>
		static InputSystem* getInstance();
#pragma endregion
#pragma region Callbacks
		/// <summary>
		/// Callback function registered with glfw
		/// </summary>
		/// <param name="window">Window context</param>
		/// <param name="button">Active button</param>
		/// <param name="action">Action of button</param>
		/// <param name="mods">Modifier bits (shift,ctrl,etc.)</param>
		void mouseCallback(GLFWwindow* window, int button, int action, int mods);
		/// <summary>
		/// Callback function registered with glfw
		/// </summary>
		/// <param name="window">Window context</param>
		/// <param name="key">Active key</param>
		/// <param name="scancode">Platform specific scancode</param>
		/// <param name="action">Action of key</param>
		/// <param name="mods">Modifier bits (shift,ctrl,etc.)</param>
		void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		/// <summary>
		/// Callback function registered with glfw
		/// </summary>
		/// <param name="window">Window context</param>
		/// <param name="xpos">X-position of the cursor</param>
		/// <param name="ypos">Y-position of the cursor</param>
		void cursorCallback(GLFWwindow* window, double xpos, double ypos);
#pragma endregion
	private:
		//Private c'tor for singleton
		InputSystem();

#pragma region Member Variables
		/// <summary>
		/// Whether the Input System is initialized
		/// </summary>
		bool m_isInit;
		/// <summary>
		/// Map of keys and whether they have been pressed in the frame
		/// </summary>
		std::map<int, bool> m_keyMap;
		/// <summary>
		/// Map of mouse buttons and whether they have been pressed
		/// </summary>
		std::map<int, bool> m_mouseMap;
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
		/// <summary>
		/// Current position of the cursor
		/// </summary>
		glm::vec2 m_cursorPos;
#pragma endregion
	};
}