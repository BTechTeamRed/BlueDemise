#include "MovementScript.h"

#include "Engine/InputSystem.h"

namespace Engine
{
	void MovementScript::onUpdate(const DeltaTime& dt)
	{
		if (m_move)
		{
			auto& c = getComponent<TransformComponent>();
			c.position.x = m_moveX + 36;
			c.position.y = m_moveY - 103;
			m_move = false;
		}
	}

}