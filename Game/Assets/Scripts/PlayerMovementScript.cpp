#include "PlayerMovementScript.h"
#include "Engine/SceneBuilder/InputSystem.h"
#include "Engine/Utilities/Log.h"
#include "Engine/SceneBuilder/Scene.h"
#include "Engine/SceneBuilder/InputSystem.h"
#include "Engine/Scripts/ColorSwap.h"

namespace Engine
{
	void PlayerMovementScript::onUpdate(const Engine::DeltaTime& dt)
	{
		if (InputSystem::getInstance()->isButtonPressed(0)) {

			// TODO: REPLACE WITH PICKING //
			float mouseX = InputSystem::getInstance()->getCursorPos().x;
			float mouseY = InputSystem::getInstance()->getCursorPos().y;

			std::list<Entity*> entities = m_entity.getScene()->pick(mouseX, mouseY);
			for (auto entity : entities)
			{
				if (entity->hasComponent<TransformComponent>())
				{
					auto transform = entity->getComponent<TransformComponent>();
					std::string tag = entity->getComponent<TagComponent>().tag;
					if (mouseX > transform.position.x && mouseY > transform.position.y
						&& mouseX < (transform.position.x + transform.scale.x)
						&& mouseY < (transform.position.y + transform.scale.y))
					{
						if (tag != "player" && tag != "obstacle")
						{
							if (tileInRange(*entity))
							{
								static_cast<ColorSwap*>(entity->getComponent<ScriptComponent>().m_instance)->swapMyColor();

								// Move to the tile the player clicked on
								auto player = m_entity;
								int x = transform.position.x + 36;
								int y = transform.position.y - 104;
								player.getComponent<TransformComponent>().position.x = x;
								player.getComponent<TransformComponent>().position.y = y;
							}
						}
					}
				}
			}
		}
	}

	bool PlayerMovementScript::tileInRange(Entity tile)
	{
		// Get the position of the player
		glm::vec3 playerPos = m_entity.getComponent<TransformComponent>().position;
		int minX = playerPos.x - 136; // -36 for offset, -100 for the tile
		int maxX = minX + 400;

		int upperMinY = playerPos.y + 54; // +104 for the offset, -50 for the tile
		int upperMaxY = playerPos.y + 104;
		int lowerMinY = upperMaxY + 50;
		int lowerMaxY = lowerMinY + 50;

		// Get the position of the tile
		auto tilePos = tile.getComponent<TransformComponent>().position;
		tilePos.x += 10;
		tilePos.y += 10;

		if (tilePos.y > upperMinY && tilePos.y < upperMaxY) {
			if (tilePos.x > minX && tilePos.x < maxX) {
				return true;
			}
		}
		else if (tilePos.y > lowerMinY && tilePos.y < lowerMaxY) {
			if (tilePos.x > minX && tilePos.x < maxX) {
				return true;
			}
		}
		return false;
	}
}