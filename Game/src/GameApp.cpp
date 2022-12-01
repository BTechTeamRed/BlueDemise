#include <Engine.h>


class GameApp : public Engine::Application
{
public:
	GameApp()
	{
		Engine::Scene scene1;
		if (Engine::Serializer::tryDeserializeScene(scene1, "animTest.bda"))
		{
			scene1.onRuntimeStart();
		}
		
	}
	~GameApp() {}
};


Engine::Application* Engine::CreateApplication()
{
	return new GameApp;
}