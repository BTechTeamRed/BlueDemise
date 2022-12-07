#include "Serializer.h"

#include <iostream>
#include <future>

#include "Engine/SceneBuilder/Entity.h"
#include "Engine/Utilities/Log.h"
#include "Engine/SceneBuilder/Scene.h"
#include "Engine/Rendering/Renderer.h"
#include "Engine/Rendering/GeometryFactory.h"
#include "Engine/ResourceManagement/ResourceManager.h"
#include "Engine/ResourceManagement/ScriptSerializer.h"
#include "Engine/Scripts/ScriptableBehavior.h"
#include "Engine/Utilities/Multithreading/ThreadJob.h"
#include "Engine/Utilities/Multithreading/JobQueue.h"

namespace glm
{
#pragma region Json Serialization
	//templates from nlohmann. Serializes/deserializes custom types
	void to_json(nlohmann::json& j, const vec2& vec)
	{
		j = { { "x", vec.x }, { "y", vec.y } };
	}

	void from_json(const nlohmann::json& j, vec2& vec)
	{
		vec.x = j.at("x").get<float>();
		vec.y = j.at("y").get<float>();
	}

	void to_json(nlohmann::json& j, const vec<2, int>& vec)
	{
		j = { { "x", vec.x }, { "y", vec.y } };
	}

	void from_json(const nlohmann::json& j, vec<2, int>& vec)
	{
		vec.x = j.at("x").get<int>();
		vec.y = j.at("y").get<int>();
	}

	void to_json(nlohmann::json& j, const vec3& vec)
	{
		j = { { "x", vec.x }, { "y", vec.y }, { "z", vec.z } };
	}

	void from_json(const nlohmann::json& j, vec3& vec)
	{
		vec.x = j.at("x").get<float>();
		vec.y = j.at("y").get<float>();
		vec.z = j.at("z").get<float>();
	}

	void to_json(nlohmann::json& j, const vec4& vec)
	{
		j = { { "r", vec.r }, { "g", vec.g }, { "b", vec.b }, { "a", vec.a } };
	}

	void from_json(const nlohmann::json& j, vec4& vec)
	{
		vec.r = j.at("r").get<float>();
		vec.g = j.at("g").get<float>();
		vec.b = j.at("b").get<float>();
		vec.a = j.at("a").get<float>();
	}

	//setup only for 1 parameter matrices.
	void to_json(nlohmann::json& j, const mat4& mat)
	{
		j = { { "matrix", mat[0][0]} };
	}

	void from_json(const nlohmann::json& j, mat4& mat)
	{
		mat = mat4(j.at("matrix").get<float>());
	}
#pragma endregion
}

namespace Engine
{
	bool Serializer::tryDeserializeScene(Scene& out, const std::string& sceneFile)
	{

		if (!Renderer::getInstance())
		{
			GE_CORE_FATAL("[Serializer] Failed to initialize opengl");
			return false;
		}

		auto data = ResourceManager::getInstance()->getJsonData(sceneFile);
		if (!(data.find("scene") != data.end()))
		{
			GE_CORE_FATAL("[Serializer] Unable to deserialize scene {0}", sceneFile);
			GE_CORE_FATAL("[Serializer] No scene found.");
			return false;
		}

		if (!(data["scene"].find("name") != data["scene"].end()))
		{
			GE_CORE_FATAL("[Serializer] Unable to deserialize scene {0}", sceneFile);
			GE_CORE_FATAL("[Serializer] Scene does not contain the \"name\" attribute.");
			return false;
		}

		out.m_name = data["scene"]["name"];

		//Loop through all entities and deserialize each
		for (const auto& item : data["scene"]["entities"])
		{
			if (!(item.find("tag") != item.end()))
			{
				GE_CORE_FATAL("[Serializer] Unable to deserialize scene {0}", sceneFile);
				GE_CORE_FATAL("[Serializer] An entity does not contain the \"tag\" attribute");
				return false;
			}

			Entity entity = out.createEntity(item["tag"]);

			if (!tryDeserializeEntity(entity, item, out))
			{

				GE_CORE_FATAL("[Serializer] Unable to deserialize scene {0}", sceneFile);
				GE_CORE_FATAL("[Serializer] The entity {0} has failed to serialize", item["tag"]);
				return false;

			}
		}

		return true;
	}

	std::string Serializer::serializeScene(Scene* scene, const std::string& sceneFile)
	{
		nlohmann::json sceneJson;
		nlohmann::json entitiesJson;
		
		scene->m_registry.each([&](entt::entity entityHandle)
		{
			Entity entity = Entity{ entityHandle, scene };
			if (!entity) return;

			if (entity.hasComponent<SerializableComponent>()) //skip entities that were generated/don't have this component
			{

				serializeEntity(entity, sceneFile, entitiesJson);
				
				//std::future handle = std::async(std::launch::async, serializeEntity, entity, sceneFile, entitiesJson);
			}
		});

		sceneJson["scene"]["entities"] = entitiesJson;
		sceneJson["scene"]["name"] = scene->m_name;

		//std::cout << sceneJson << std::endl;
		ResourceManager::getInstance()->saveJsonFile(sceneJson, sceneFile, "bda");
		return sceneJson.dump();
		//TODO: Bind serialization to GUI event once we have one.

	}

	void Serializer::serializeEntity(Entity& entity, const std::string& sceneFile, nlohmann::json& out)
	{


		if (!entity.hasComponent<TagComponent>())
		{
			GE_CORE_ERROR("[Serializer] An entity was created without a tag component and cannot be serialized.");
		}

		nlohmann::json components = nlohmann::json::array();

		if (entity.hasComponent<CameraComponent>())
		{
			auto c = entity.getComponent<CameraComponent>();
			nlohmann::json j;
			j["name"] = parseComponentToString(CO_CameraComponent);
			j["aspectRatio"] = c.aspectRatio;
			j["frustumWidth"] = c.frustumWidth;
			j["farZ"] = c.farZ;
			j["nearZ"] = c.nearZ;

			components.push_back(j);
		}

		if (entity.hasComponent<TransformComponent>())
		{
			auto c = entity.getComponent<TransformComponent>();
			nlohmann::json j;
			j["name"] = parseComponentToString(CO_TransformComponent);
			j["position"] = c.position;
			j["scale"] = c.scale;
			j["rotation"] = c.rotation;

			components.push_back(j);
		}

		if (entity.hasComponent<TextComponent>())
		{
			auto c = entity.getComponent<TextComponent>();
			nlohmann::json j;
			j["name"] = parseComponentToString(CO_TextComponent);
			j["text"] = c.text;

			components.push_back(j);
		}

		if (entity.hasComponent<MaterialComponent>())
		{
			auto c = entity.getComponent<MaterialComponent>();
			nlohmann::json j;
			j["name"] = parseComponentToString(CO_MaterialComponent);
			j["color"] = c.color;
			j["texName"] = c.texName;
			j["shaderName"] = c.shaderName;

			components.push_back(j);
		}

		if (entity.hasComponent<AnimationComponent>())
		{
			auto c = entity.getComponent<AnimationComponent>();
			nlohmann::json j;
			j["name"] = parseComponentToString(CO_AnimationComponent);
			j["numPerRow"] = c.numPerRow;
			j["frameRate"] = c.frameRate;
			j["spriteSheetSize"] = c.spriteSheetSize;
			j["spriteSize"] = c.spriteSize;
			j["numSprites"] = c.spritesOnSheet;

			components.push_back(j);
		}

		if (entity.hasComponent<PositionLerpComponent>())
		{
			auto c = entity.getComponent<PositionLerpComponent>();
			nlohmann::json j;
			j["name"] = parseComponentToString(CO_PositionLerpComponent);
			j["speed"] = c.speed;
			j["target"] = c.target;

			components.push_back(j);
		}

		if (entity.hasComponent<VerticesComponent>())
		{
			auto c = entity.getComponent<VerticesComponent>();
			nlohmann::json j;
			j["name"] = parseComponentToString(CO_VerticesComponent);
			j["type"] = c.isSprite ? "sprite" : "polygon";

			//TODO: Serialize vertex if polygon once we know what that looks like.	

			components.push_back(j);
		}

		if (entity.hasComponent<ScriptComponent>())
		{
			auto c = entity.getComponent<ScriptComponent>();
			nlohmann::json j;
			j["name"] = parseComponentToString(CO_ScriptComponent);
			j["scriptName"] = c.m_instance->getScriptName();

			components.push_back(j);
		}

		if (entity.hasComponent<PhysicsComponent>())
		{
			const auto& c = entity.getComponent<PhysicsComponent>();
			nlohmann::json j;
			j["name"] = parseComponentToString(CO_PhysicsComponent);
			j["dimensions"] = c.boundingBox->getDimensions();
			j["position"] = c.boundingBox->getPosition();

			components.push_back(j);
		}

		if (entity.hasComponent<AudioComponent>())
		{
			auto c = entity.getComponent<AudioComponent>();
			nlohmann::json j;
			j["name"] = parseComponentToString(CO_AudioComponent);
			j["soundFileName"] = c.soundFileName;

			components.push_back(j);
		}

		if (entity.hasComponent<ScriptUI>() && !entity.hasComponent<ScriptComponent>())
		{
			auto c = entity.getComponent<ScriptUI>();
			nlohmann::json j;
			j["name"] = parseComponentToString(CO_ScriptComponent);
			j["scriptName"] = c.sourceFileName;

			components.push_back(j);
		}

		//add all components and tag to json
		nlohmann::json entityJson;
		entityJson["components"] = components;
		entityJson["tag"] = entity.getComponent<TagComponent>().tag;

		GE_CORE_INFO("[Serializer] Serialized Component: {0}", entityJson.dump());

		out.push_back(entityJson);
	}

	bool Serializer::tryDeserializeEntity(Entity& out, const nlohmann::json& entity, Scene& scene)
	{

		//Loop through all components and deserialize each
		for (const auto& component : entity["components"])
		{
			if (!(component.find("name") != component.end()))
			{
				GE_CORE_FATAL("[Serializer] Unable to deserialize entity {0}", entity["tag"]);
				GE_CORE_FATAL("[Serializer] A component does not contain the \"name\" attribute");
				return false;
			}

			auto name = parseComponent(component["name"].get<std::string>());

			//check each component type individually
			switch (name)
			{
			case CO_CameraComponent:
			{
				auto frustumWidth = component["frustumWidth"].get<float>();
				auto aspectRatio = component["aspectRatio"].get<float>();
				auto farZ = component["farZ"].get<float>();
				auto nearZ = component["nearZ"].get<float>();

				out.addComponent<CameraComponent>(frustumWidth, aspectRatio, farZ, nearZ);
				break;
			}
			case CO_TransformComponent:
			{
				auto position = component["position"].get<glm::vec3>();
				auto scale = component["scale"].get<glm::vec3>();
				auto rotation = component["rotation"].get<glm::vec3>();

				out.addComponent<TransformComponent>(position, scale, rotation);
				break;
			}
			case CO_MaterialComponent:
			{

				std::string texture = component["texName"];
				std::string shader = component["shaderName"];
				auto image = ResourceManager::getInstance()->getTexture(texture);

				out.addComponent <MaterialComponent>(component["color"].get<glm::vec4>(), image.texID, texture, shader);
				break;
			}
			case CO_TextComponent:
			{
				std::string text = component["text"];

				out.addComponent<TextComponent>(text);
				break;
			}
			case CO_VerticesComponent:
			{
				//Check if vertices is of type sprite
				if (!(component.find("type") != component.end()))
				{
					GE_CORE_FATAL("[Serializer] Unable to deserialize entity {0}", entity["tag"]);
					GE_CORE_FATAL("[Serializer] A VerticesComponent does not contain the \"type\" attribute");
					return false;
				}

				if (component["type"] == "sprite")
				{
					out.addComponent<VerticesComponent>(GeometryFactory::getInstance()->getVerticesComponent(GeometryFactory::RT_Sprite));
				}

				break;
			}
			case CO_AnimationComponent:
			{
				auto numPerRow = component["numPerRow"].get<int>();
				auto frameRate = component["frameRate"].get<float>();
				auto spriteSheetSize = component["spriteSheetSize"].get<glm::vec<2, int>>();
				auto spriteSize = component["spriteSize"].get<glm::vec<2, int>>();
				auto spritesOnSheet = component["numSprites"].get<int>();

				out.addComponent<AnimationComponent>(spriteSheetSize, spriteSize, numPerRow, spritesOnSheet, frameRate);
				break;
			}
			case CO_PositionLerpComponent:
			{
				auto speed = component["speed"].get<float>();
				auto target = component["target"].get<glm::vec3>();

				out.addComponent<PositionLerpComponent>(target, speed);
				break;
			}
			case CO_ScriptComponent:
			{
				std::string scriptName = component["scriptName"];
				out.addComponent<ScriptUI>(scriptName);
				ScriptSerializer::linkAndDeserializeScript(out, scriptName);
				break;
			}
			case CO_PhysicsComponent:
			{
				glm::vec3 dimensions = component["dimensions"].get<glm::vec3>();
				glm::vec3 position = component["position"].get<glm::vec3>();

				out.addComponent<PhysicsComponent>(dimensions, position);
				break;
			}
			case CO_AudioComponent:
			{
				std::string soundFileName = component["soundFileName"];
				out.addComponent<AudioComponent>(soundFileName);
				break;
			}
			default:
			{
				GE_CORE_WARN("[Serializer] Could not deserialize component {0}", component["name"]);
				break;
			}
			}
		}

		out.addComponent<SerializableComponent>();

		return true;
	}
}