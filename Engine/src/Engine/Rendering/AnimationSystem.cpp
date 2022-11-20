#include "AnimationSystem.h"
#include "Engine/ResourceManagement/ResourceManager.h"

namespace Engine
{
	//Constructor for an animationSystem that is sprite based. ResourceManagers spriteData struct is passed in and converted into animationSystems spriteData struct.
	AnimationSystem::AnimationSystem(ResourceManager::SpriteSheet spriteData)
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
	}

	void AnimationSystem::playAnimation(AnimationType animType, int changeToThis)
	{
		switch (animType)
		{
			case RT_LoopRow:
			{
				if (m_spriteRowIndex < m_spriteAnimationData.spritesPerRow)
					m_spriteRowIndex++;
				else
					m_spriteRowIndex = 0;
			}
			default:
			{
				
			}
		}
	}
	

	void AnimationSystem::changeFrame(int frame)
	{
		
	}


	void AnimationSystem::updateUVCoords()
	{
		
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