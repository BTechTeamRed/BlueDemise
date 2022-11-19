#include "Engine/Scripts/ScriptableBehavior.h"

namespace Engine
{
	class MoveRightScript : public Engine::ScriptableBehavior
	{
	protected:
		virtual void onUpdate(const Engine::DeltaTime& dt) override;

	public:
		virtual std::string getScriptName() override { return "MoveRightScript"; }
	};
}