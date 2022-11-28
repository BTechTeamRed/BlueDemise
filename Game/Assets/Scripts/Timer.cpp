#include "Timer.h"
#include "Score.h"

namespace Engine
{
	void Timer::onUpdate(const Engine::DeltaTime& dt)
	{
		time += dt.getSeconds();
	}

	std::string Timer::getTime()
	{
		return std::to_string(time);
	}
}