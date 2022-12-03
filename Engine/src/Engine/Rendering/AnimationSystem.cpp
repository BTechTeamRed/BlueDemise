#include "AnimationSystem.h"
#include "Engine/ResourceManagement/ResourceManager.h"
#include <glm/gtx/matrix_transform_2d.hpp>

namespace Engine
{
	//Constructor for an animationSystem that is sprite based. ResourceManagers spriteData struct is passed in and converted into animationSystems spriteData struct.
	/*AnimationSystem::AnimationSystem(ResourceManager::SpriteSheet spriteData)
	{
		m_spriteAnimationData.texID = spriteData.texID;
		m_spriteAnimationData.spritesPerRow = spriteData.spritesPerRow;
		m_spriteAnimationData.spritesPerColumn = spriteData.spritesPerColumn;
		m_spriteAnimationData.spriteWidthFraction = spriteData.texWidthFraction;
		m_spriteAnimationData.spriteHeightFraction = spriteData.texHeightFraction;
		m_spriteAnimationData.spritesOnSheet = spriteData.numSprites;
		m_spriteAnimationData.spritesheetHeight = spriteData.spriteSheetHeight;
		m_spriteAnimationData.spritesheetWidth = spriteData.spriteSheetWidth;
		m_spriteAnimationData.spriteWidth = spriteData.spriteWidth;
		m_spriteAnimationData.spriteHeight = spriteData.spriteHeight;
	}

	AnimationSystem::~AnimationSystem()
	{
	}*/

	void AnimationSystem::updateAnimation(const DeltaTime& dt, AnimationComponent& component)
	{
		component.deltaTime += dt.getSeconds();
		if (component.deltaTime > component.frameRate) { //enough time has elapsed since last frame update
			component.deltaTime -= component.frameRate;
			if (++component.currentIndex >= component.animationClip.size()) { //Loop back to the start if we are at the end of the clip
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
		switch (type) {
			case RT_LoopRow:
				result = std::vector<int>(component.numPerRow);
				//Fills result with values 0 to numPerRow - 1
				std::iota(result.begin(), result.end(), 0); 
				return result;

			case RT_LoopColumn:
				//Fills result with the first index on each row;
				for (int i = 0; i < std::floor(component.spritesOnSheet / component.numPerRow); i++) {
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


/*
	void prepareAnimations(Scene& scene)
	{

			//Buffer new data into VBO, if there is any new data. (is this needed if an animation system adjusts these buffers? No, we shouldnt, as we'll use unique VBOs per object.
			//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

		const auto animations = scene.getEntities<const AnimationComponent>();

		if (scene.m_registry.all_of<AnimationComponent>(entity))
		{
			glBindBuffer(GL_ARRAY_BUFFER, vertices.vboID);
			auto& anim = m_registry.get<AnimationComponent>(entity);
			anim.deltaTime += dt;

			if (anim.deltaTime > anim.animationSpeed)
			{
				anim.deltaTime = 0;
				anim.currentIndex++;
				if (anim.currentIndex > anim.spritesOnSheet) anim.currentIndex = 0;
			}

			//Calculation for finding the sprite in a texture.
			const float tx = (anim.currentIndex % anim.numPerRow) * anim.texWidthFraction;
			const float ty = (anim.currentIndex / anim.numPerRow + 1) * anim.texHeightFraction;

			//bind VBO
			float vertices[] =
			{
				// positions  // texture coords (UV coords)
				0.f, 0.f, 0.f,  tx, ty,														// top left
				1.f, 0.f, 0.f,  tx + anim.texWidthFraction, ty,								// top right
				1.f, 1.f, 0.f,  tx + anim.texWidthFraction, ty + anim.texHeightFraction,	// bottom right
				0.f, 1.f, 0.f,  tx, ty + anim.texHeightFraction								// bottom left
			};

			//Buffer new data into VBO
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

			glBindTexture(GL_TEXTURE_2D, anim.texID);
		}

	}*/
}