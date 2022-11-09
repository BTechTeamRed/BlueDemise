#include "Engine/Scripts/ScriptableBehavior.h"

namespace Game
{
	class ENGINE_API MoveRightScript : public Engine::ScriptableBehavior
	{
		virtual void onUpdate(const Engine::DeltaTime& dt) override;

		virtual std::string getScriptName() override { return "MoveRightScript"; }
	};
}
