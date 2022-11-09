#pragma once
#include "Engine/Core.h"

namespace Engine
{
	class ENGINE_API Application
	{
	public:
		void Run();
	};

	// Defined in CLIENT
	Application* CreateApplication();

}