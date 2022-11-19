#pragma once
#include "Engine/Core.h"
#include "glm/glm.hpp"
#include <vector>
#include <string>

#include "Engine/ResourceManagement/ResourceManager.h"


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

			AnimationSystem(ResourceManager::ImageData textureData, float spritesheetWidth, float spritesheetHeight, int numPerRow, int spritesOnSheet);
			~AnimationSystem();

			#pragma region Manage Animation
			void playAnimation(AnimationType type, int animate);
			void changeFrame(int frame);
			void updateUVCoords();
			#pragma endregion

		private:
			#pragma region Sprite Data
			//Generated texture ID for the spritesheet.
			GLuint m_textureID;

			//Texture/UV coordinates for the sprite, in the form of a 4x2 matrix.
			std::vector<glm::vec2> m_UVCoords;

			//Currently rendered sprite from the sprite sheet, starting from top-left corner, going by row.
			int m_currentIndex = 0;

			//Height and width of a sprite as a percentage, in the context of UV coordinates.
			float m_spriteWidthFraction;
			float m_spriteHeightFraction;

			//Number of sprites per row, and sprites on a spritesheet/texture.
			int m_spritesPerRow;
			int m_spritesOnSheet;
			#pragma endregion

			#pragma region Animation Data
			
			float m_deltaTime;
			float m_frameRate;

			float m_animationSpeed = 0.1f;
			#pragma endregion
	};
}
