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
			CO_TextComponent,
			CO_AnimationComponent,
			CO_PositionLerpComponent,
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
		static nlohmann::json serializeEntity(const Entity& entity, const std::string& sceneFile);

		//Serializes scene into a json file in sceneFile
		static nlohmann::json serializeEntity(Entity& entity, const std::string& sceneFile);

		[[nodiscard]] static Components parseComponent(const std::string& component)
		{
			return componentMap.find(component) != componentMap.end() ? componentMap.find(component)->second : CO_Invalid;
		}

		[[nodiscard]] static std::string parseComponentToString(const Components component)
		{
			if (component == CO_CameraComponent) return"CameraComponent";
			if (component == CO_TransformComponent) return"TransformComponent";
			if (component == CO_MaterialComponent) return"MaterialComponent";
			if (component == CO_AnimationComponent) return"AnimationComponent";
			if (component == CO_VerticesComponent) return"VerticesComponent";
			if (component == CO_TextComponent) return"TextComponent";
			if (component == CO_SerializableComponent) return"SerializableComponent";
			if (component == CO_ScriptComponent) return"ScriptComponent";
			if (component == CO_PhysicsComponent) return"PhysicsComponent";
			if (component == CO_AudioComponent) return"AudioComponent";
			return "";
		}

		inline static std::unordered_map<std::string, Components> componentMap
		{
			{ "CameraComponent", CO_CameraComponent },
			{ "TransformComponent", CO_TransformComponent },
			{ "MaterialComponent", CO_MaterialComponent },
			{ "AnimationComponent", CO_AnimationComponent },
			{ "VerticesComponent", CO_VerticesComponent },
			{ "TextComponent", CO_TextComponent },
			{ "SerializableComponent", CO_SerializableComponent },
			{ "ScriptComponent", CO_ScriptComponent },
			{ "PhysicsComponent", CO_PhysicsComponent },
			{ "AudioComponent", CO_AudioComponent }
		};
	};

}