#include <Engine.h>


class GameApp : public Engine::Application
{
public:
	GameApp()
	{
		Engine::Scene scene;
		if (Engine::Serializer::tryDeserializeScene(scene, "level1.bda"))
		{
			scene.onRuntimeStart();
		}
		
	}
	~GameApp() {}
};


Engine::Application* Engine::CreateApplication()
{
	return new GameApp;
}