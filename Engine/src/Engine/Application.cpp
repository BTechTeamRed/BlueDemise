#include "Application.h"
#include "Scene.h"

namespace Engine
{
	void Application::Run()
	{
		Scene scene;
		scene.onRuntimeStart();
	}
}