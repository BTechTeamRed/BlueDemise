#include "AnimationSystem.h"
#include "Engine/ResourceManagement/ResourceManager.h"
#include <glm/gtx/matrix_transform_2d.hpp>

namespace Engine
{
	void AnimationSystem::updateAnimation(const DeltaTime& dt, AnimationComponent& component)
	{
		component.deltaTime += dt.getSeconds();
		if (component.deltaTime > component.frameRate) //enough time has elapsed since last frame update
		{ 
			component.deltaTime -= component.frameRate;
			if (++component.currentIndex >= component.animationClip.size()) //Loop back to the start if we are at the end of the clip
			{ 
				//TODO: add animation mode enum to dictate whether we loop here, stop, or reverse direction
				component.currentIndex = 0; 
			} 
			changeFrame(component.currentIndex, component); //update the current frame of the animation to the next one in the clip
		}
	}

	bool AnimationSystem::changeFrame(int frameIndex, AnimationComponent& component)
	{
		if (component.currentIndex >= component.animationClip.size()) return false; //Cannot change to frame that is outside of clip
		component.currentIndex = frameIndex;

		//Calculate the row and column
		int row = std::floor(component.animationClip[frameIndex] / component.numPerRow);
		int col = component.animationClip[frameIndex] % component.numPerRow;

		glm::vec2 scale = glm::vec2(component.spriteSize.x / (float)component.spriteSheetSize.x, component.spriteSize.y / (float)component.spriteSheetSize.y);

		//Apply Translate
		component.uvTransformMatrix = glm::translate(glm::mat3(1.f), glm::vec2(col * scale.x, row * scale.y));

		//Apply Scale
		component.uvTransformMatrix = glm::scale(component.uvTransformMatrix, scale);

		return true;
	}

	std::vector<int> AnimationSystem::createAnimationClip(AnimationType type, const AnimationComponent& component) const
	{
		std::vector<int> result;
		switch (type) 
		{
			case RT_LoopRow:
				result = std::vector<int>(component.numPerRow);
				//Fills result with values 0 to numPerRow - 1
				std::iota(result.begin(), result.end(), 0); 
				return result;

			case RT_LoopColumn:
				//Fills result with the first index on each row;
				for (int i = 0; i < std::floor(component.spritesOnSheet / component.numPerRow); i++) 
				{
					result.push_back(component.numPerRow * i);
				}
				return result;

			case RT_LoopAll:
				result = std::vector<int>(component.spritesOnSheet);
				//Fills result with values 0 to spritesOnSheet - 1
				std::iota(result.begin(), result.end(), 0);
				return result;
		}
	}

	bool AnimationSystem::updatePositionLerps(const DeltaTime& dt, PositionLerpComponent& lerp, TransformComponent& transform)
	{
		glm::vec3 dist = transform.position - lerp.target;
		glm::vec3 step = glm::normalize(dist);
		step *= lerp.speed;
		step *= (dt.getSeconds() == 0) ? 1.f : dt.getSeconds();

		//If the step is greater than the distance, we are at our destination
		if (glm::length(dist) < glm::length(step)) 
		{ 
			transform.position = lerp.target;
			return true;
		}
		else 
		{
			transform.position -= step;
			return false;
		}
	}
}