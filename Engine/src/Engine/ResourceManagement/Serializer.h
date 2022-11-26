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
		static std::string serializeScene(Scene* scene, const std::string& sceneFile);
	private:

		enum Components
		{
			CO_Invalid,
			CO_CameraComponent,
			CO_TransformComponent,
			CO_MaterialComponent,
			CO_AnimationComponent,
			CO_VerticesComponent,
			CO_SerializableComponent,
			CO_ScriptComponent,
			CO_PhysicsComponent,
			CO_AudioComponent
		};

		//Deserializes out into an entity
		//returns false if Deserialization fails.
		static bool tryDeserializeEntity(Entity& out, const nlohmann::json& entity, Scene& scene);

		//Serializes scene into a json file in sceneFile
		static nlohmann::json serializeEntity(Entity& entity, const std::string& sceneFile);

		[[nodiscard]] static Components parseComponent(const std::string& component) 
		{
			if (component == "CameraComponent") return CO_CameraComponent;
			if (component == "TransformComponent") return CO_TransformComponent;
			if (component == "MaterialComponent") return CO_MaterialComponent;
			if (component == "AnimationComponent") return CO_AnimationComponent;
			if (component == "VerticesComponent") return CO_VerticesComponent;
			if (component == "SerializableComponent") return CO_SerializableComponent;
			if (component == "ScriptComponent") return CO_ScriptComponent;
			if (component == "PhysicsComponent") return CO_PhysicsComponent;
			if (component == "AudioComponent") return CO_AudioComponent;
			return CO_Invalid;
		}

		[[nodiscard]] static std::string parseComponentToString(const Components component)
		{
			if (component == CO_CameraComponent) return"CameraComponent";
			if (component == CO_TransformComponent) return"TransformComponent";
			if (component == CO_MaterialComponent) return"MaterialComponent";
			if (component == CO_AnimationComponent) return"AnimationComponent";
			if (component == CO_VerticesComponent) return"VerticesComponent";
			if (component == CO_SerializableComponent) return"SerializableComponent";
			if (component == CO_ScriptComponent) return"ScriptComponent";
			if (component == CO_PhysicsComponent) return"PhysicsComponent";
			if (component == CO_AudioComponent) return"AudioComponent";
			return "";
		}
	};

}