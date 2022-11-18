#include "AnimationSystem.h"

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