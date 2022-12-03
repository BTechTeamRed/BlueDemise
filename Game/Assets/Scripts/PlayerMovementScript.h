#include "Engine/Scripts/ScriptableBehavior.h"
#include <vector>

namespace Engine
{
	class PlayerMovementScript : public Engine::ScriptableBehavior
	{
	protected:
		virtual void onUpdate(const Engine::DeltaTime& dt) override;

	public:
		virtual std::string getScriptName() override { return "PlayerMovementScript"; }
		bool tileInRange(Entity tile);
	private:
		std::vector<int> m_clipDown = { 0, 1, 2 };
		std::vector<int> m_clipWalkUp = { 3, 4, 5 };
		std::vector<int> m_clipSide = { 6, 7, 8 };
	};
}
