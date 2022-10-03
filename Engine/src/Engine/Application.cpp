#include "Application.h"
#include "Scene.h"

namespace Engine
{
	void Application::Run()
	{
		nlohmann::json saveData = m_importResources1.returnJson("SaveData_Test.json");
		

		auto fName = saveData.find("Height");
		
		if (fName != saveData.end())
			std::cout << fName.key() << '\n';
		else
			std::cout << "Json entry not found within [" << saveData.dump() << "]";

		Scene scene;
		scene.onRuntimeStart();
	}
}