//THREADING TESTING APPLICATION.CPP CODE
#include "Application.h"
#include "Scene.h"
#include "ThreadPool.h"
#include "JobQueue.h"
#include "ThreadJob.h"
#include "Log.h"
#include <tuple>
#include <sstream>

namespace Engine
{
	void Application::Run()
	{
		Scene scene;
		scene.onRuntimeStart();
	}
}