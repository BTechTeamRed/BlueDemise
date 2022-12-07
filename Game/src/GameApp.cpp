#include <Engine.h>


class GameApp : public Engine::Application
{
public:
	GameApp()
	{
		Engine::Scene scene1;
		if (Engine::Serializer::tryDeserializeScene(scene1, "level1.bda"))
		{
			// lvl1 Won't actually be swapped to, just here to make the vector be correct size
			scene1.m_sceneList.push_back("level1.bda"); 
			scene1.m_sceneList.push_back("level2.bda");
			scene1.m_sceneList.push_back("level3.bda");
			scene1.m_sceneList.push_back("level4.bda");
			scene1.m_sceneList.push_back("level5.bda");
			scene1.onRuntimeStart();
		}
		
	}
	~GameApp() {}
};


Engine::Application* Engine::CreateApplication()
{
	return new GameApp;
}