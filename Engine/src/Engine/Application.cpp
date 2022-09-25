#include "Application.h"

namespace Engine
{
	void Application::Run()
	{
		std::cout << "Hello";
		
		nlohmann::json saveData = m_importResources.returnJson("SaveData_Test.json");
		//saveData = nlohmann::json::parse(saveData);

		auto fName = saveData.find("Width");
		
		if (fName != saveData.end())
			std::cout << fName.value() << '\n';
		
		while (true);
	}
}