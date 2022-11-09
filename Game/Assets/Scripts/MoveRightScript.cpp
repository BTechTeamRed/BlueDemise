#include "MoveRightScript.h"


namespace Game
{
	void MoveRightScript::onUpdate(const Engine::DeltaTime& dt)
	{
		auto& c = getComponent<Engine::TransformComponent>();
		c.position.x += 5 * dt;
	}
}
