#pragma once
#include "Engine/Scripts/ScriptableBehavior.h"

namespace Engine
{
	class MovementScript : public ScriptableBehavior
	{
	protected:
		void onUpdate(const DeltaTime& dt) override;
		bool m_move{false};
		float m_moveX;
		float m_moveY;

		friend Scene;
	};
}


