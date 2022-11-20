#include "Timer.h"
#include "Score.h"

namespace Engine
{
	void Timer::onUpdate(const Engine::DeltaTime& dt)
	{
		time += dt.getSeconds();
		GE_CORE_INFO("Timer: {0}", time);
		if (doesEntityExist("score"))
		{
			GE_CORE_INFO("Score: {0}", static_cast<Score*>(getEntity("score").getComponent<ScriptComponent>().m_instance)->getScore());
		}
	}

	std::string Timer::getTime()
	{
		return std::to_string(time);
	}
}