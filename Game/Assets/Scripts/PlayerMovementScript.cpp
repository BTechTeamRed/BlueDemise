#include "PlayerMovementScript.h"
#include "Engine/SceneBuilder/InputSystem.h"
#include "Engine/Utilities/Log.h"
#include "Engine/SceneBuilder/Scene.h"
#include "Engine/SceneBuilder/InputSystem.h"

namespace Engine
{
	void PlayerMovementScript::onUpdate(const Engine::DeltaTime& dt)
	{
		if (InputSystem::getInstance()->isButtonPressed(0)) {

			// TODO: REPLACE WITH PICKING //
			float mouseX = InputSystem::getInstance()->getCursorPos().x;
			float mouseY = InputSystem::getInstance()->getCursorPos().y;

			// Loop through all the entities
			const auto entities = getEntities<TransformComponent>();
			for (auto& [entity, transform] : entities.each())
			{
				// If the mouse is inside the entity:
				if (mouseX > transform.position.x && mouseY > transform.position.y
					&& mouseX < (transform.position.x + transform.scale.x)
					&& mouseY < (transform.position.y + transform.scale.y))
				{
					Entity other = { entity, m_entity.getScene() };

					// If the entity is not a player or obstacle:
					if (other.getComponent<TagComponent>().tag != "player" &&
						other.getComponent<TagComponent>().tag != "obstacle")
					{
						// Change the color of the selected entity
						other.getComponent<MaterialComponent>().color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

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