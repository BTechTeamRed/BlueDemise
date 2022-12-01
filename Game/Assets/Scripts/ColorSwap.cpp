#include "ColorSwap.h"
#include "Score.h"

namespace Engine
{
	void ColorSwap::onCreate()
	{
		// Get the current color of the tile
		glm::vec4 currentColor = m_entity.getComponent<MaterialComponent>().color;

		// Get the index of the current color
		for (auto& [index, color] : colors)
		{
			if (color == currentColor)
			{
				currentColorIndex = index;
			}
		}
		auto transform = m_entity.getComponent<TransformComponent>();
		glm::vec3 dimensions;
		auto entities = getEntities<CameraComponent>();
		for (auto& [entity, camera] : entities.each())
		{
			dimensions.x = camera.frustumWidth;
			dimensions.y = camera.frustumWidth / camera.aspectRatio;
			dimensions.z = camera.farZ - camera.nearZ;
		}
		dimensions /= 2.0f;
		glm::vec3 position = transform.position;
		glm::vec3 scaleOffset = transform.scale / 2.0f;
		position -= dimensions - scaleOffset;
		position.y *= -1;
		position.z += 1.0f;
		if (!m_entity.hasComponent<PhysicsComponent>())
		{
			m_entity.addComponent<PhysicsComponent>(transform.scale, position);
		}
		else
		{
			//auto comp = m_entity.getComponent<PhysicsComponent>();
			//comp.boundingBox->updateDimensions(dimensions);
			//comp.boundingBox->updatePosition(position);
		}
	}

	void ColorSwap::swapMyColor()
	{
		// Increment index to the next color
		currentColorIndex++;
		if (currentColorIndex > difficultyLevel)
		{
			currentColorIndex = 0;
		}

		// Update the color of the tile
		m_entity.getComponent<MaterialComponent>().color = colors[currentColorIndex];
		checkLevelComplete();
	}

	void ColorSwap::checkLevelComplete()
	{
		glm::vec4 color = m_entity.getComponent<MaterialComponent>().color;
		const auto entities = getEntities<TransformComponent>();
		int tileCount = 0;
		int matchingColorCount = 0;
		for (auto& [entity, transform] : entities.each())
		{
			Entity other = { entity, m_entity.getScene() };
			if (other.getComponent<TagComponent>().tag != "player" &&
				other.getComponent<TagComponent>().tag != "obstacle")
			{
				tileCount++;
				if (color == other.getComponent<MaterialComponent>().color)
				{
					matchingColorCount++;
				}
			}
		}
		if (tileCount == matchingColorCount)
		{
			// Finish the level!
			if (doesEntityExist("score"))
			{
				static_cast<Score*>(getEntity("score").getComponent<ScriptComponent>().m_instance)->addOne();
				GE_CORE_INFO("Score: {0}", static_cast<Score*>(getEntity("score").getComponent<ScriptComponent>().m_instance)->getScore());
				GE_CORE_INFO("Scene Score: {0}", m_entity.getScene()->score);
				m_entity.getScene()->m_switch = true;
			}
		}
	}

	void ColorSwap::onUpdate(const Engine::DeltaTime& dt)
	{

	}
}