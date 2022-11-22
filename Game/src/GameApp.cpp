#include <Engine.h>


class GameApp : public Engine::Application
{
public:
	GameApp()
	{
		Engine::Scene scene1;
		Engine::Scene scene2;
		if (Engine::Serializer::tryDeserializeScene(scene1, "level1.bda"))
		{
			scene1.m_nextScene = "level2.bda";
			scene1.onRuntimeStart();
		}
		
	}
	~GameApp() {}
};


Engine::Application* Engine::CreateApplication()
{
	return new GameApp;
}