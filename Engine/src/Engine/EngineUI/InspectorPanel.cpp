#include "InspectorPanel.h"
#include "Engine/SceneBuilder/Entity.h"

namespace Engine
{
	void InspectorPanel::setRegistry(entt::registry* registry)
	{
		m_registry = registry;
	}

	void InspectorPanel::setSelectedEntity(entt::entity entityHandle)
	{
		m_entityHandle = entityHandle;
	}

	void InspectorPanel::setFont(const std::string& font)
	{
		m_font = font;
	}

	void InspectorPanel::show()
	{
		ImGui::Begin("InspectorPanel", nullptr,
			ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);

		static bool isPositionSet = false;
		if (!isPositionSet)
		{
			ImGui::SetWindowPos("InspectorPanel", ImVec2(m_position.x, m_position.y));
			isPositionSet = true;
		}

		ImGui::SetWindowSize("InspectorPanel", ImVec2(m_dimension.x, m_dimension.y));

		//Adjust this to add more/less space between UI partitions
		const int spacing = 3;

		if (m_registry && m_registry->alive() > 0)
		{
			if (m_registry->any_of<TagComponent>(m_entityHandle))
			{
				auto* tag = &(m_registry->get<TagComponent>(m_entityHandle));
				
				//For now we use "font" as a font tag, which is ignored anyway
				partition("MyriadPro_bold_18", "Inspector - " + tag->tag, DARK_CYAN);

				//We must set color back to default
				s_style->Colors[ImGuiCol_Text] = LIGHT_GREY;

				setSpacing(spacing);
			}

			if (m_registry->any_of<TransformComponent>(m_entityHandle))
			{
				auto* transform = &(m_registry->get<TransformComponent>(m_entityHandle));
				
				partition("MyriadPro_bold_14", "Transform", LIGHT_CYAN);

				ImGui::PushFont(s_fonts["MyriadPro_14"]);
				s_style->Colors[ImGuiCol_Text] = OFF_WHITE;
				
				ImGui::SliderFloat3("Position", &transform->position.x, -25.0f, 25.0f, "%.2f");
				ImGui::SliderFloat3("Scale", &transform->scale.x, -25.0f, 25.0f, "%.2f");
				ImGui::SliderFloat3("Rotation", &transform->rotation.x, -25.0f, 25.0f, "%.2f");

				setSpacing(spacing);

				ImGui::PopFont();
			}

			if (m_registry->any_of<CameraComponent>(m_entityHandle))
			{
				auto* camera = &(m_registry->get<CameraComponent>(m_entityHandle));

				partition("MyriadPro_bold_14", "Camera", LIGHT_CYAN);

				ImGui::PushFont(s_fonts["MyriadPro_14"]);
				s_style->Colors[ImGuiCol_Text] = OFF_WHITE;

				//Not sure what the min/max values should be
				ImGui::SliderFloat("Far Z", &camera->farZ, 100.0f, 1000.0f, "%.2f");
				ImGui::SliderFloat("Near Z", &camera->nearZ, 0.01f, 0.1f, "%.2f");

				setSpacing(spacing);

				ImGui::PopFont();
			}
			
			if (m_registry->any_of<MaterialComponent>(m_entityHandle))
			{
				auto* material = &(m_registry->get<MaterialComponent>(m_entityHandle));

				partition("MyriadPro_bold_14", "Material", LIGHT_CYAN);

				ImGui::PushFont(s_fonts["MyriadPro_14"]);
				s_style->Colors[ImGuiCol_Text] = OFF_WHITE;

				std::string id = "Texture ID: " + std::to_string(material->texID);
				std::string name = "Texture Name: " + material->texName;
				ImGui::SliderFloat4("Color", &material->color.r, 0.0f, 1.0f, "%.2f");

				ImGui::Text(id.c_str());
				ImGui::Text(name.c_str());

				setSpacing(spacing);

				ImGui::PopFont();
			}

			if (m_registry->any_of<AnimationComponent>(m_entityHandle))
			{
				auto* animation = &(m_registry->get<AnimationComponent>(m_entityHandle));

				partition("MyriadPro_bold_14", "Animation", LIGHT_CYAN);

				ImGui::PushFont(s_fonts["MyriadPro_14"]);
				s_style->Colors[ImGuiCol_Text] = OFF_WHITE;

				std::string id = "ID: " + std::to_string(animation->texID);
				std::string name = "Name: " + animation->texName;

				ImGui::Text(id.c_str());
				ImGui::Text(name.c_str());

				std::string index = "Index: " + std::to_string(animation->currentIndex);
				std::string numPerRow = "Number per row: " + std::to_string(animation->numPerRow);

				ImGui::Text(index.c_str());
				ImGui::Text(numPerRow.c_str());

				ImGui::SliderFloat("Frame rate", &animation->frameRate, 0.0f, 1.0f, "%.2f");
				ImGui::SliderFloat("Texture width", &animation->texWidthFraction, 0.0f, 1.0f, "%.2f");
				ImGui::SliderFloat("Texture height", &animation->texHeightFraction, 0.0f, 1.0f, "%.2f");
				
				setSpacing(spacing);

				ImGui::PopFont();
			}

			if (m_registry->any_of<SerializableComponent>(m_entityHandle))
			{
				ImGui::PushFont(s_fonts["MyriadPro_14"]);
				s_style->Colors[ImGuiCol_Text] = OFF_WHITE;

				auto* serializable = &(m_registry->get<SerializableComponent>(m_entityHandle));
				ImGui::Checkbox("Is serializable", &serializable->serializable);

				setSpacing(spacing);

				ImGui::PopFont();
			}

			if (m_registry->any_of<ScriptUI>(m_entityHandle))
			{
				auto* script = &(m_registry->get<ScriptUI>(m_entityHandle));
				partition("MyriadPro_bold_14", "Script - " + script->sourceFileName, LIGHT_CYAN);
			}
		}

		ImGui::End();
	}
}
