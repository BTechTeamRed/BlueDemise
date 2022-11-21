#include "UIRenderer.h"

#include "Engine/Utilities/Log.h"

#include "Engine/ResourceManagement/Serializer.h"
#include "Engine/Rendering/Window.h"
#include "Engine/SceneBuilder/Scene.h"
#include "Engine/SceneBuilder/InputSystem.h"



namespace Engine
{
#pragma region UI Management
	bool UIRenderer::initializeUI(Window& window, Scene& scene)
	{
		//Initialize the UI using ImGui OpenGL v3.3
		if (!UserInterface::initialize(window.getWindow()))
		{
			return false;
		}

		//Load custom fonts - we must load a different for each size we require
		//We are using MyriadPro currently as the Freedom font does not display numeric values
		if (!UserInterface::loadFont("MyriadPro_bold.otf", 12, "Freedom_12"))
		{
			//Error loading font
			GE_CORE_ERROR("Failed to load custom fonts");
		}

		if (!UserInterface::loadFont("MyriadPro_bold.otf", 18, "Freedom_18"))
		{
			//Error loading font
			GE_CORE_ERROR("Failed to load custom fonts");
		}

		const int menuHeight = 18;
		const auto panelWidth = 0.2f * window.getWidth();
		const auto halfWindowHeight = 0.5f * (window.getHeight() - menuHeight);

		m_tagDialog.setPosition(glm::uvec2(0.5f * window.getWidth(), 0.5f * window.getHeight()));

		m_explorerPanel.setPosition(glm::uvec2(0, menuHeight));
		m_explorerPanel.setDimension(glm::uvec2(panelWidth, window.getWidth()));

		m_gamePanel.setPosition(glm::uvec2(panelWidth + 3, menuHeight));
		m_gamePanel.setDimension(glm::uvec2(panelWidth * 3, halfWindowHeight));
		m_gamePanel.setInitialPosition();
		
		m_hierarchyPanel.setPosition(glm::uvec2(panelWidth * 4 - 4, menuHeight));
		m_hierarchyPanel.setDimension(glm::uvec2(panelWidth, halfWindowHeight));

		m_inspectorPanel.setPosition(glm::uvec2(panelWidth * 4 - 4, halfWindowHeight + menuHeight));
		m_inspectorPanel.setDimension(glm::uvec2(panelWidth, halfWindowHeight));

		m_componentsPanels[(int)ComponentsPanel::PanelType::Components].setPosition(glm::uvec2(panelWidth * 3 - 1, menuHeight));
		m_componentsPanels[(int)ComponentsPanel::PanelType::Components].setDimension(glm::uvec2(panelWidth, halfWindowHeight));


		//TODO - Does this need to be read from a .json file?
		m_componentsPanels[(int)ComponentsPanel::PanelType::Components].addComponent("Transform");
		m_componentsPanels[(int)ComponentsPanel::PanelType::Components].addComponent("Camera");
		m_componentsPanels[(int)ComponentsPanel::PanelType::Components].addComponent("Material");
		m_componentsPanels[(int)ComponentsPanel::PanelType::Components].addComponent("Serializable");
		//m_componentsPanels[(int)ComponentsPanel::PanelType::Components].addComponent("Animation");

		m_componentsPanels[(int)ComponentsPanel::PanelType::Components].setPosition(glm::uvec2(panelWidth * 4 - 2, menuHeight + 0.5f * (window.getHeight() - menuHeight)));
		m_componentsPanels[(int)ComponentsPanel::PanelType::Components].setDimension(glm::uvec2(panelWidth, halfWindowHeight));

		m_componentsPanels[(int)ComponentsPanel::PanelType::Components].setPosition(glm::uvec2(panelWidth * 3 - 3, menuHeight + 0.5f * (window.getHeight() - menuHeight)));
		m_componentsPanels[(int)ComponentsPanel::PanelType::Components].setDimension(glm::uvec2(panelWidth, halfWindowHeight));

		m_inspectorPanel.setRegistry(&scene.m_registry);

		return true;

	}

	void UIRenderer::shutdownUI()
	{
		UserInterface::shutdown();
	}

	void UIRenderer::renderUI(Scene& scene, Window& window)
	{
		UserInterface::startUI();

		m_mainMenu.show();

		m_gamePanel.show(window); //This function will take in an fbo when one is created
		InputSystem::getInstance()->setWindowOffset(m_gamePanel.getPos());

		m_explorerPanel.show();
		m_hierarchyPanel.show();
		m_inspectorPanel.show();

		for (auto& panel : m_componentsPanels)
		{
			panel.show();
		}

		m_tagDialog.update();
		m_tagDialog.show();

		UserInterface::endUI();

		//UI handlers===========================================================

		if (m_mainMenu.getIsSaveItemClicked())
		{
			//Save entities and components to a JSON file
			Engine::Serializer::serializeScene(&scene, "testScene");
		}

		//We look inside our own local entity map to search for
		//the entity the user clicked on in the Inspector panel 
		m_inspectorPanel.setSelectedEntity(m_hierarchyPanel.getSelectedEntity());

		if (m_hierarchyPanel.isAddButtonClicked())
		{
			m_tagDialog.setIsVisible(true);
		}

		//If we click on OKAY when labelling the new entity and no name has been set it will be called ENTITY
		//Otherwise we label it based on the name input in the tag dialog box
		if (m_tagDialog.getButtonState().okay)
		{
			auto tag = m_tagDialog.getTag();

			if (tag.empty())
			{
				tag = "<Entity_" + std::to_string(scene. m_registry.size() + 1) + ">";
			}

			Entity entity = scene.createEntity(tag);
		}

		if (m_tagDialog.getButtonState().cancel)
		{
			//Do nothing if we cancel
		}

		if (m_componentsPanels[2].isAddButtonClicked())
		{
			//Check which component was selected to be added to entity
			auto component = m_componentsPanels[2].getSelectedComponent();

			//Get the handle on the selected entity to add the component to
			auto handle = m_hierarchyPanel.getSelectedEntity();

			if (component == "Camera")
			{
				auto camera = scene.m_registry.any_of<CameraComponent>(handle);

				//Only add this component if there isn't already one attached
				if (!camera)
				{
					scene.m_registry.emplace<CameraComponent>(
						handle,
						1024.f,
						1.78f,
						100.0f,
						0.1f
						);
				}
			}

			if (component == "Transform")
			{
				auto transform = scene.m_registry.any_of<TransformComponent>(handle);

				//Only add this component if there isn't already one attached
				if (!transform)
				{
					scene.m_registry.emplace<TransformComponent>(
						handle,
						glm::vec3(0, 0, 0),
						glm::vec3(1, 1, 1),
						glm::vec3(0, 0, 0));
				}
			}

			if (component == "Material")
			{
				auto material = scene.m_registry.any_of<MaterialComponent>(handle);

				//Only add this component if there isn't already one attached
				if (!material)
				{
					scene.m_registry.emplace<MaterialComponent>(
						handle,
						glm::vec4(1, 1, 1, 1.0f),
						0,
						"<Texture>",
						0
						);
				}
			}

			if (component == "Serializable")
			{
				auto serializable = scene.m_registry.any_of<SerializableComponent>(handle);

				//Only add this component if there isn't already one attached
				if (!serializable)
				{
					scene.m_registry.emplace<SerializableComponent>(
						handle,
						true);
				}
			}


			//if (component == "Animation")
			//{
			//	auto animation = m_registry.any_of<AnimationComponent>(handle);

			//	//Only add this component if there isn't already one attached
			//	if (!animation)
			//	{
			//		m_registry.emplace<AnimationComponent>(
			//			handle,
			//			0,
			//			1.0f,
			//			1.0f,
			//			1.0f,
			//			1);
			//	}
			//}
		}
	}

#pragma endregion

#pragma region Update UI

	void UIRenderer::updateHierarchyPanel(std::string tag, Entity entity)
	{
		m_hierarchyPanel.addEntity(tag, entity.getHandle());
	}

#pragma endregion
}