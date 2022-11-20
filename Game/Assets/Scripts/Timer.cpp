#include "Timer.h"

namespace Engine
{
	void Timer::onUpdate(const Engine::DeltaTime& dt)
	{
		time += dt.getSeconds();
		GE_CORE_INFO("Timer: {0}", time);
	}

	std::string Timer::getTime()
	{
		return std::to_string(time);
	}
}