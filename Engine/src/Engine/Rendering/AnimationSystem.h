#pragma once
#include "Engine/Core.h"


namespace Engine 
{
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

			AnimationSystem();
			~AnimationSystem();
			void playAnimation(AnimationType type, int animate);

			void changeFrame(int frame);
			void updateUVCoords();
		private:
			
		
	};
}