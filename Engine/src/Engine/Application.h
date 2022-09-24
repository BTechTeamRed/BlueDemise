#pragma once
#include "Core.h"
#include "ImportResources.h"

namespace Engine
{
	class ENGINE_API Application
	{
	public:
		ImportResources m_importResources;
		void Run();
	};

	// Defined in CLIENT
	Application* CreateApplication();

}