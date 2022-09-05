#include <Engine.h>

class GameApp : public Engine::Application
{
public:
	GameApp() {}
	~GameApp() {}

};


Engine::Application* Engine::CreateApplication()
{
	return new GameApp;
}