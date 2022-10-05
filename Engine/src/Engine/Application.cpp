#include "Application.h"
#include "Scene.h"

namespace Engine
{
	void Application::Run()
	{	
		
		nlohmann::json data = m_importResources1.returnJson("SaveData_Test.json");

		//std:: cout << saveData;

		auto fName = data.find("pi");
		
		std::cout << fName.value();

		data = m_importResources1.returnJson("SaveData_Test.json");

		fName = data.find("pi");

		std::cout << fName.value();
		
		Scene scene;
		scene.onRuntimeStart();
	}
}