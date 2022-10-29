#pragma once

#include <string>
#include "Engine/Core.h"
#include "json.h"

namespace Engine
{
	class Scene;
	class Entity;

	class ENGINE_API Serializer
	{
	public:
		//Deserializes out into a scene with json data from a file in sceneFile
		//returns false if Deserialization fails.
		static bool tryDeserializeScene(Scene& out, const std::string& sceneFile);

		//Serializes scene into a json file in sceneFile
		static void serializeScene(const Scene& scene, const std::string& sceneFile);

	private:
		//Deserializes out into an entity
		//returns false if Deserialization fails.
		static bool tryDeserializeEntity(Entity& out, const nlohmann::json& entity, Scene& scene);
	};

}