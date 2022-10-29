#include "Serializer.h"

#include "Engine/Entity.h"
#include "Engine/Log.h"
#include "Engine/ResourceManager.h"

namespace Engine
{
	bool Serializer::tryDeserializeScene(Scene& out, const std::string& scenePath)
	{
		auto data = ResourceManager::getInstance()->getJsonData(scenePath);

		//Loop through all entities and deserialize each
		for (const auto& item : data["scene"]["entities"])
		{
			if (!(item.find("tag") != item.end()))
			{
				GE_CORE_FATAL("Unable to deserialize scene {0}", scenePath);
				GE_CORE_FATAL("An entity does not contain the \"tag\" attribute");
			}

			Entity entity = out.createEntity(item["tag"]);
			if (!tryDeserializeEntity(entity, item))
			{
				GE_CORE_FATAL("Unable to deserialize scene {0}", scenePath);
				GE_CORE_FATAL("The entity {0} has failed to serialize", item["tag"]);
			}
				
		}
	}

	void Serializer::serializeScene(const Scene& scene, const std::string& scenePath)
	{
		
	}

	bool Serializer::tryDeserializeEntity(Entity& out, const nlohmann::json& entity)
	{
		//Loop through all components and deserialize each
		for (const auto& component : entity["components"])
		{
			if (!(component.find("name") != component.end()))
			{
				GE_CORE_FATAL("Unable to deserialize entity {0}", entity["tag"]);
				GE_CORE_FATAL("A component does not contain the \"name\" attribute");
			}

			auto name = component["name"];

			//check each component type individually
			if (name == "CameraComponent")
			{
				out.addComponent<CameraComponent>();
			}
		}
	}
}
