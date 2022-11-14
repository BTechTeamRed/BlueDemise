#pragma once
#include <Engine/Utilities/DeltaTime.h>
#include <Engine/Core.h>
#include <Engine/Rendering/Window.h>

namespace Engine
{
	class Scene;

	class ENGINE_API Renderer
	{
	public:
		void renderScene(const DeltaTime& dt, Scene scene);
	private:
		Window m_Window;
		
	};
}