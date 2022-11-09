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
		static void serializeScene(Scene* scene, const std::string& sceneFile);
	private:

		enum Components
		{
			CO_Invalid,
			CO_CameraComponent,
			CO_TransformComponent,
			CO_ColorComponent,
			CO_TextureComponent,
			CO_AnimationComponent,
			CO_VerticesComponent,
			CO_ScriptComponent
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
			if (component == "ColorComponent") return CO_ColorComponent;
			if (component == "TextureComponent") return CO_TextureComponent;
			if (component == "AnimationComponent") return CO_AnimationComponent;
			if (component == "VerticesComponent") return CO_VerticesComponent;
			if (component == "ScriptComponent") return CO_ScriptComponent;
			return CO_Invalid;
		}

		[[nodiscard]] static std::string parseComponentToString(const Components component)
		{
			if (component == CO_CameraComponent) return"CameraComponent";
			if (component == CO_TransformComponent) return"TransformComponent";
			if (component == CO_ColorComponent) return"ColorComponent";
			if (component == CO_TextureComponent) return"TextureComponent";
			if (component == CO_AnimationComponent) return"AnimationComponent";
			if (component == CO_VerticesComponent) return"VerticesComponent";
			if (component == CO_ScriptComponent) return"ScriptComponent";

			return "";
		}
	};

}