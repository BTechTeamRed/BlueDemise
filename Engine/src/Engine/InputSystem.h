#pragma once

namespace Engine {
	class InputSystem
	{
	public:
		//Delete copy c'tor and assignment opertor for singleton pattern
		InputSystem(InputSystem& other) = delete;
		void operator=(InputSystem& other) = delete;

		//

		//Getter for singleton instance
		static InputSystem* getInstance();
	private:
		//Private c'tor for singleton
		InputSystem();


		//Static singleton instance
		static InputSystem* m_instance;
	};
}