#pragma once
#include <Engine/Utilities/DeltaTime.h>
#include "Engine/Core.h"

namespace Engine
{
	class Scene;

	class ENGINE_API Renderer
	{
	public:
		void renderScene(const DeltaTime& dt, Scene scene);
	//private:
		
	};
}