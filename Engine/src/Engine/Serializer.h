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
		//Deserializes out into a scene with json data from a file in scenePath
		//returns false if Deserialization fails.
		bool tryDeserializeScene(Scene& out, const std::string& scenePath);

		//Serializes scene into a json file in scenePath
		void serializeScene(const Scene& scene, const std::string& scenePath);

	private:
		//Deserializes out into an entity
		//returns false if Deserialization fails.
		bool tryDeserializeEntity(Entity& out, const nlohmann::json& entity, Scene& scene);
	};

}