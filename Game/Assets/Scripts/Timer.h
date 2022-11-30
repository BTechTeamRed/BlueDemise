#pragma once

#include "Engine/Scripts/ScriptableBehavior.h"

namespace Engine
{
	class Timer : public Engine::ScriptableBehavior
	{
	protected:
		virtual void onUpdate(const Engine::DeltaTime& dt) override;
	public:
		double time;
		std::string getTime();
		virtual std::string getScriptName() override { return "Timer"; }
	};
}