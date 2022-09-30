#include "Application.h"
#include "ThreadPool.h"

using namespace engine_concurrent;

namespace Engine
{
	void Application::Run()
	{
		ThreadPool* thePool = ThreadPool::getInstance();
		while (true);
	}
}