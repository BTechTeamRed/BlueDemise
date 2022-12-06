#pragma once
#include "Engine/Core.h"
#include "glm/glm.hpp"
#include <vector>
#include <string>


namespace Engine 
{
	class ResourceManager;
	class DeltaTime;
	struct AnimationComponent;
	struct TransformComponent;
	struct PositionLerpComponent;
	typedef unsigned int GLuint;

	class ENGINE_API AnimationSystem
	{
		public:
			enum AnimationType
			{
				RT_LoopRow,
				RT_LoopColumn,
				RT_LoopAll,
			};

			#pragma region Manage Animation

			//Updates an Animation component, changing the frame if required
			void updateAnimation(const DeltaTime& dt, AnimationComponent& component);

			//updates the UV matrix to reflect the sprite at the frameIndex, if it is a valid index
			bool changeFrame(int frameIndex, AnimationComponent& component);

			//Creates a list of indicies to the component's sprites to create an animation of the specified type
			std::vector<int> createAnimationClip(AnimationType type, const AnimationComponent& component) const;
			
			//Updates a PositionLerpComponent, moving the transform towards the lerp target by step. Returns true if position == target
			bool updatePositionLerps(const DeltaTime& dt, PositionLerpComponent& lerp, TransformComponent& transform);
			#pragma endregion
	};
}
