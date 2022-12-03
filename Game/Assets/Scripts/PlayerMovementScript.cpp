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
		if (InputSystem::getInstance()->isButtonPressed(0))
		{
			float mouseX = InputSystem::getInstance()->getCursorPos().x;
			float mouseY = InputSystem::getInstance()->getCursorPos().y;

			// Use picking to get entities (with physics component) player clicked on
			std::list<Entity*> entities = m_entity.getScene()->pick(mouseX, mouseY);
			//GE_TRACE("Pick @ {0} {1}", mouseX, mouseY);
			for (auto entity : entities)
			{
				//GE_TRACE("Picked this: {0}", entity->getComponent<TagComponent>().tag);
				if (entity->hasComponent<TransformComponent>() && !m_entity.hasComponent<PositionLerpComponent>())
				{
					auto transform = entity->getComponent<TransformComponent>();
					if (tileInRange(*entity))
					{
						static_cast<ColorSwap*>(entity->getComponent<ScriptComponent>().m_instance)->swapMyColor();

						// Move to the tile the player clicked on
						auto player = m_entity;
						int x = transform.position.x + 36;
						int y = transform.position.y - 104;
						player.addComponent<PositionLerpComponent>(glm::vec3(x, y, 0), 50.f);

						auto& anim = player.getComponent<AnimationComponent>();
						//Change animations
						auto& playerTransform = player.getComponent<TransformComponent>();
						if (playerTransform.position.x >= x && playerTransform.position.y >= y) { //Moving Right
							anim.animationClip = m_clipSide;
							anim.currentIndex = 0;
							playerTransform.rotation.y = 3.14f;
						}
						else if (playerTransform.position.x <= x && playerTransform.position.y >= y) { //Moving Left
							anim.animationClip = m_clipSide;
							anim.currentIndex = 0;
							playerTransform.rotation.y = 0.f;
						}
						else if (playerTransform.position.x <= x && playerTransform.position.y <= y) { //Moving Down
							anim.animationClip = m_clipDown;
							anim.currentIndex = 0;
						}
						else { //Moving up
							anim.animationClip = m_clipDown;
							anim.currentIndex = 0;
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

		if (tilePos.y > upperMinY && tilePos.y < upperMaxY)
		{
			if (tilePos.x > minX && tilePos.x < maxX)
			{
				return true;
			}
		}
		else if (tilePos.y > lowerMinY && tilePos.y < lowerMaxY)
		{
			if (tilePos.x > minX && tilePos.x < maxX)
			{
				return true;
			}
		}
		return false;
	}
}