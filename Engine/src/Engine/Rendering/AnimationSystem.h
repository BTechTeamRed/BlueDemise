#pragma once
#include "Engine/Core.h"
#include "glm/glm.hpp"
#include <vector>
#include <string>

#include "Engine/ResourceManagement/ResourceManager.h"
#include "Engine/SceneBuilder/Components.h"
#include "Engine/Utilities/DeltaTime.h"


namespace Engine 
{
	class ResourceManager;
	typedef unsigned int GLuint;

	class ENGINE_API AnimationSystem
	{
		public:
			enum AnimationType
			{
				RT_LoopRow,
				RT_LoopColumn,
				RT_LoopAll,
				RT_Manual
			};

			//Constructor for spriteSheet-based animations. Takes in a ResourceManager::SpriteSheet struct and converts it into an AnimationSystem::SpriteData struct.
			/*AnimationSystem(ResourceManager::SpriteSheet spriteData);
			~AnimationSystem();*/

			#pragma region Manage Animation
			//Updates an Animation component, changing the frame if required
			void updateAnimation(const DeltaTime& dt, AnimationComponent& component);
			//updates the UV matrix to reflect the sprite at the frameIndex, if it is a valid index
			bool changeFrame(int frameIndex, AnimationComponent& component);
			//Creates a list of indicies to the component's sprites to create an animation of the specified type
			std::vector<int> createAnimationClip(AnimationType type, const AnimationComponent& component) const;
			#pragma endregion

		private:
			#pragma region Sprite Data
			//Struct to contain spriteData that is used for animating a spritesheet
			/*
			struct SpriteData
			{
				SpriteData() = default;

				//Generated texture ID for the spritesheet.
				GLuint texID;

				//Currently rendered sprite from the sprite sheet, starting from top-left corner, going by row.
				int currentIndex = 0;

				//Height and width of a sprite as a percentage, in the context of UV coordinates.
				float spriteWidthFraction;
				float spriteHeightFraction;

				//Height and width of a spritesheet image.
				int spritesheetWidth;
				int spritesheetHeight;

				//Individual sprite height/width
				float spriteWidth;
				float spriteHeight;

				//Number of sprites per row, and sprites on a spritesheet/texture.
				int spritesPerRow;
				int spritesPerColumn;
				int spritesOnSheet;

				//Texture/UV coordinates for the sprite, in the form of a 4x2 matrix.
				std::vector<glm::vec2> UVCoords;
			};

			int m_spriteRowIndex = 0;
			int m_spriteColumnIndex = 0;
			SpriteData m_spriteAnimationData;
			#pragma endregion

			#pragma region Animation Data
			
			float m_deltaTime;
			float m_frameRate;

			float m_animationSpeed = 0.1f;
			#pragma endregion
			*/
	};
}
