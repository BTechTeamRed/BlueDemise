#include "InspectorPanel.h"
#include "Entity.h"

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
				partition("Freedom_18", "Inspector - " + tag->tag, green);

				//We must set color back to default
				s_style->Colors[ImGuiCol_Text] = grey;

				setSpacing(spacing);
			}

			if (m_registry->any_of<TransformComponent>(m_entityHandle))
			{
				auto* transform = &(m_registry->get<TransformComponent>(m_entityHandle));
				
				partition("Freedom_18", "Transform", darkGreen);
				
				ImGui::SliderFloat3("Position", &transform->position.x, -25.0f, 25.0f, "%.2f");
				ImGui::SliderFloat3("Scale", &transform->scale.x, -25.0f, 25.0f, "%.2f");
				ImGui::SliderFloat3("Rotation", &transform->rotation.x, -25.0f, 25.0f, "%.2f");

				setSpacing(spacing);
			}

			if (m_registry->any_of<CameraComponent>(m_entityHandle))
			{
				auto* camera = &(m_registry->get<CameraComponent>(m_entityHandle));

				partition("Freedom_18", "Camera", darkGreen);

				//Not sure what the min/max values should be
				ImGui::SliderFloat("FOV", &camera->fov, 45.0f, 90.0f, "%.2f");
				ImGui::SliderFloat("Far Z", &camera->farZ, 100.0f, 1000.0f, "%.2f");
				ImGui::SliderFloat("Near Z", &camera->nearZ, 0.01f, 0.1f, "%.2f");
				ImGui::SliderFloat2("Viewport", &camera->viewport.x, 1.0f, 5.0f, "%.2f");

				setSpacing(spacing);
			}

			if (m_registry->any_of<ColorComponent>(m_entityHandle))
			{
				auto* color = &(m_registry->get<ColorComponent>(m_entityHandle));

				partition("Freedom_18", "Color", darkGreen);

				ImGui::SliderFloat4("Color", &color->color.r, 0.0f, 1.0f, "%.2f");

				setSpacing(spacing);
			}

			if (m_registry->any_of<TextureComponent>(m_entityHandle))
			{
				auto* texture = &(m_registry->get<TextureComponent>(m_entityHandle));

				partition("Freedom_18", "Texture", darkGreen);

				std::string id = "ID: " + std::to_string(texture->texID);
				std::string name = "Name: " + texture->texName;

				ImGui::Text(id.c_str());
				ImGui::Text(name.c_str());

				setSpacing(spacing);
			}

			if (m_registry->any_of<AnimationComponent>(m_entityHandle))
			{
				auto* animation = &(m_registry->get<AnimationComponent>(m_entityHandle));

				partition("Freedom_18", "Animation", darkGreen);

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
			}
		}

		ImGui::End();
	}
}
