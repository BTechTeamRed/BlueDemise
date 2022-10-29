#include <Engine.h>


class GameApp : public Engine::Application
{
public:
	GameApp()
	{
		Engine::Scene scene;
		scene.onRuntimeStart();
	}
	~GameApp() {}
};


Engine::Application* Engine::CreateApplication()
{
	return new GameApp;
}