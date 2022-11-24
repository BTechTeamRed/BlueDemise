#pragma once
#include "GLFW/glfw3.h"
#include "Engine/Core.h"

namespace Engine
{
	//Container for the DeltaTime used in game.
	class ENGINE_API DeltaTime
	{
	public:
		DeltaTime(double dt = 0)
		: m_Time(dt)
		{
		}

		operator double() const { return m_Time; }

		//returns the deltaTime in milliseconds.
		double getMilliseconds() const { return m_Time * 1000; }

		//returns the deltaTime in seconds.
		double getSeconds() const { return m_Time; }

		//Updates the value of deltaTime. Has to be called every frame.
		void updateDeltaTime()
		{
			const double time = glfwGetTime();
			m_Time = time - m_previousTime;
			m_previousTime = time;
		}

	private:
		double m_Time;
		double m_previousTime;
	};
}