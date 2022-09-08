#pragma once

extern Engine::Application* Engine::CreateApplication();

int main(int argc, char** argv)
{
	Engine::Log::Init();
	GE_CORE_INFO("Starting Engine");

	auto app = Engine::CreateApplication();
	app->Run();

	delete app;
}