#include "Application.h"

#include "Entity.h"
#include "Scene.h"

namespace Engine
{
	void Application::Run()
	{
		Scene scene;
		Entity entityTest = scene.createEntity("test");
	}
}
