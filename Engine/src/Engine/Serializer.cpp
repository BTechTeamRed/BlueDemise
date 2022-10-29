#include "Serializer.h"

#include "Engine/Entity.h"
#include "Engine/Log.h"
#include "Engine/ResourceManager.h"

namespace glm
{
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
		j = { { "r", vec.x }, { "g", vec.y }, { "b", vec.y }, { "a", vec.y } };
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
		mat = mat4(j["matrix"]);
	}
}

namespace Engine
{
	bool Serializer::tryDeserializeScene(Scene& out, const std::string& scenePath)
	{
		auto data = ResourceManager::getInstance()->getJsonData(scenePath);
		if (!(data.find("scene") != data.end()))
		{
			GE_CORE_FATAL("Unable to deserialize scene {0}", scenePath);
			GE_CORE_FATAL("No scene found.");
			return false;
		}

		if (!(data["scene"].find("name") != data["scene"].end()))
		{
			GE_CORE_FATAL("Unable to deserialize scene {0}", scenePath);
			GE_CORE_FATAL("Scene does not contain the \"name\" attribute.");
			return false;
		}

		out.m_name = data["scene"]["name"];

		//Loop through all entities and deserialize each
		for (const auto& item : data["scene"]["entities"])
		{
			if (!(item.find("tag") != item.end()))
			{
				GE_CORE_FATAL("Unable to deserialize scene {0}", scenePath);
				GE_CORE_FATAL("An entity does not contain the \"tag\" attribute");
				return false;
			}

			Entity entity = out.createEntity(item["tag"]);
			if (!tryDeserializeEntity(entity, item, out))
			{
				GE_CORE_FATAL("Unable to deserialize scene {0}", scenePath);
				GE_CORE_FATAL("The entity {0} has failed to serialize", item["tag"]);
				return false;
			}

		}

		return true;
	}

	void Serializer::serializeScene(const Scene& scene, const std::string& scenePath)
	{

	}

	bool Serializer::tryDeserializeEntity(Entity& out, const nlohmann::json& entity, Scene& scene)
	{
		//Loop through all components and deserialize each
		for (const auto& component : entity["components"])
		{
			if (!(component.find("name") != component.end()))
			{
				GE_CORE_FATAL("Unable to deserialize entity {0}", entity["tag"]);
				GE_CORE_FATAL("A component does not contain the \"name\" attribute");
				return false;
			}

			auto name = component["name"].get<std::string>();

			//check each component type individually
			if (name == "CameraComponent")
			{
				auto& camera = out.addComponent<CameraComponent>();
				camera.fov = component["fov"].get<float>();
				camera.projection = component["projection"].get<glm::mat4>();
				camera.viewport = component["viewport"].get<glm::vec2>();
				camera.farZ = component["farZ"].get<float>();
				camera.nearZ = component["nearZ"].get<float>();
			}

			else if (name == "TransformComponent")
			{
				auto& transform = out.addComponent<TransformComponent>();
				transform.position = component["position"].get<glm::vec3>();
				transform.scale = component["scale"].get<glm::vec3>();
				transform.rotation = component["rotation"].get<glm::vec3>();
			}

			else if (name == "ColorComponent")
			{
				auto& color = out.addComponent<ColorComponent>();
				color.color = component["color"].get<glm::vec4>();
			}

			else if (name == "TextureComponent")
			{
				std::string texture = component["texName"];
				auto image = ResourceManager::getInstance()->getTexture(texture);
				out.addComponent<TextureComponent>(image.texID, texture);
			}

			else if (name == "VerticesComponent")
			{
				//Check if vertices is of type sprite
				if (component.find("type") != component.end())
				{
					GE_CORE_FATAL("Unable to deserialize entity {0}", entity["tag"]);
					GE_CORE_FATAL("A VerticesComponent does not contain the \"type\" attribute");
					return false;
				}

				if (component["type"] == "sprite")
				{
					out.addComponent<VerticesComponent>(scene.createSprite());
				}
			}
		}

		return true;
	}
}