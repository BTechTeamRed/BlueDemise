#pragma once

#include "Engine/Scripts/ScriptableBehavior.h"

namespace Engine
{
	class Score: public Engine::ScriptableBehavior
	{
	protected:
		int score = 1;
		virtual void onCreate() override;
	public:
		void setScore(int num);
		void addOne();
		std::string getScore();
		virtual std::string getScriptName() override { return "Score"; }
	};
}
