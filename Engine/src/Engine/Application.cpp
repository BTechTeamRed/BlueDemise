#include "Application.h"
#include "Scene.h"
#include "ResourceManager.h"

namespace Engine
{
	void Application::Run()
	{	
		nlohmann::json data = {
			{"name", "John"},
			{"age", 30},
			{"city", "New York"}
		};
		nlohmann::json data2 = {
			{"num", 2},
			{"root", {
				{"name", "root"},
				{"children", {
					{
						{"name", "child1"},
						{"children", {
							{
								{"name", "child1.1"}
							},
							{
								{"name", "child1.2"},
							}
						}}
					},
					{
						{"name", "child2"},
						{"num", 3}
					}
				}}
			}}
		};
		
		ResourceManager::getInstance()->saveJsonFile(data, "SaveData_Test");
		
		ResourceManager::getInstance()->saveJsonFile(data, "test1");
		//ResourceManager::getInstance()->saveJsonFile(data2, "test1");
		//ResourceManager::getInstance()->saveJsonFile(data2, "test2", "Data/test/");
		
		Scene scene;
		scene.onRuntimeStart();
	}
}