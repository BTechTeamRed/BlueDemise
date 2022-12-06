#include "PlayerMovementScript.h"
#include "Engine/SceneBuilder/InputSystem.h"
#include "Engine/Utilities/Log.h"
#include "Engine/SceneBuilder/Scene.h"
#include "Engine/SceneBuilder/InputSystem.h"
#include "Engine/Scripts/ColorSwap.h"

namespace Engine
{
	void PlayerMovementScript::onCreate()
	{
		auto entities = m_entity.getScene()->getEntities<TransformComponent, TagComponent>();
		for (auto& [entity, transform, tag] : entities.each())
		{
			if (tag.tag.find("tile") != -1)
			{
				transform.position.x += 10;
				transform.position.y -= 11;
				transform.scale.x = 175.0;
				transform.scale.y = 87.0;
			}

			if (tag.tag == "tile1") transform.position.z = 8.0f;
			if (tag.tag == "tile2" || tag.tag == "tile6") transform.position.z = 7.9f;
			if (tag.tag == "tile3" || tag.tag == "tile7" || tag.tag == "tile11") transform.position.z = 7.8f;
			if (tag.tag == "tile4" || tag.tag == "tile8" || tag.tag == "tile12" || tag.tag == "tile16") transform.position.z = 7.7f;
			if (tag.tag == "tile5" || tag.tag == "tile9" || tag.tag == "tile13" || tag.tag == "tile17" || tag.tag == "tile21") transform.position.z = 7.6f;
			if (tag.tag == "tile10" || tag.tag == "tile14" || tag.tag == "tile18" || tag.tag == "tile22") transform.position.z = 7.5f;
			if (tag.tag == "tile15" || tag.tag == "tile19" || tag.tag == "tile23") transform.position.z = 7.4f;
			if (tag.tag == "tile20" || tag.tag == "tile24") transform.position.z = 7.3f;
			if (tag.tag == "tile25") transform.position.z = 7.2f;
		}
	}


	void PlayerMovementScript::onUpdate(const Engine::DeltaTime& dt)
	{
		if (InputSystem::getInstance()->isButtonPressed(0))
		{
			float mouseX = InputSystem::getInstance()->getCursorPos().x;
			float mouseY = InputSystem::getInstance()->getCursorPos().y;

			// Use picking to get entities (with physics component) player clicked on
			std::list<Entity*> entities = m_entity.getScene()->pick(mouseX, mouseY);
			//GE_TRACE("Pick @ {0} {1}", mouseX, mouseY);
			for (auto& entity : entities)
			{
				//GE_TRACE("Picked this: {0}", entity->getComponent<TagComponent>().tag);
				if (entity->hasComponent<TransformComponent>() && !m_entity.hasComponent<PositionLerpComponent>())
				{
					auto& transform = entity->getComponent<TransformComponent>();
					if (tileInRange(*entity))
					{
						auto* ref = static_cast<ColorSwap*>(entity->getComponent<ScriptComponent>().m_instance);
						if (ref) ref->swapMyColor();

						// Move to the tile the player clicked on
						auto& player = m_entity;
						int x = transform.position.x + 36;
						int y = transform.position.y - 104;
						auto& playerTransform = player.getComponent<TransformComponent>();
						player.addComponent<PositionLerpComponent>(glm::vec3(x, y, playerTransform.position.z), m_moveSpeed);

						auto& anim = player.getComponent<AnimationComponent>();
						//Change animations

						if (playerTransform.position.x >= x && playerTransform.position.y >= y) //Moving Right
						{ 
							anim.animationClip = m_clipSide;
							anim.currentIndex = 0;
							playerTransform.rotation.y = 3.14f;
						}
						else if (playerTransform.position.x <= x && playerTransform.position.y >= y) //Moving Left
						{ 
							anim.animationClip = m_clipSide;
							anim.currentIndex = 0;
							playerTransform.rotation.y = 0.f;
						}
						else if (playerTransform.position.x <= x && playerTransform.position.y <= y) //Moving Down
						{ 
							anim.animationClip = m_clipDown;
							anim.currentIndex = 0;
						}
						else { //Moving up
							anim.animationClip = m_clipDown;
							anim.currentIndex = 0;
						}
						if (player.hasComponent<AudioComponent>())
						{
							player.getComponent<AudioComponent>().playSound(false, false, false);
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