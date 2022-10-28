#pragma once

extern Engine::Application* Engine::CreateApplication();

int main(int argc, char** argv)
{
	Engine::Log::Init();

	auto app = Engine::CreateApplication();
	if (app)
	{
		GE_CORE_INFO("Starting Engine");
		app->Run();

		delete app;
	}
}