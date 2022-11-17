#include "glad/glad.h"
#include "Engine/SceneBuilder/Scene.h"

#include "Engine/Utilities/Log.h"

#include "Entity.h"
#include "Components.h"
#include <Engine/Utilities/DeltaTime.h>
#include "Engine/Scripts/ScriptableBehavior.h"
#include "InputSystem.h"
#include "Engine/ResourceManagement/Serializer.h"
#include "Engine/Rendering/Renderer.h"

const float DT_THRESHOLD = 10;

namespace Engine
{
#pragma region Runtime Functions
	//Initialize the scene and loop update until the window should be closed. 
	//I feel a new loop method should be used rather than 'when the window closes'
	void Scene::onRuntimeStart()
	{
		Renderer::getInstance()->initializeScene(*this);

		//initialize the window for UI
		if (showUI) {
			if (!initializeUI()) return;
		}

		while (!m_closeScene)
		{
			m_deltaTime.updateDeltaTime();
			m_deltaTime = m_deltaTime > DT_THRESHOLD ? 0 : m_deltaTime;
			onRuntimeUpdate(m_deltaTime);
		}

		onRuntimeStop();
	}

	//Upon the scenes conclusion, terminate vertices objects, and call stopScene.
	void Scene::onRuntimeStop()
	{
		const auto view = getEntities<const VerticesComponent>();
		for (auto [entity, vertices] : view.each())
		{
			glDeleteVertexArrays(1, &vertices.vaoID);
			glDeleteBuffers(1, &vertices.vboID);
		}

		Renderer::getInstance()->stopScene(*this);
	}

	//Per scene update loop, add scripts to entities if enabled and render the scene.
	void Scene::onRuntimeUpdate(const DeltaTime& dt)
	{
		//get a view on entities with a script Component, and execute their onUpdate.
		const auto entities = getEntities<ScriptComponent>();
		for (auto [entity, script] : entities.each())
		{
			//initialize the script instance and run OnCreate();
			if (!script.m_instance)
			{
				script.m_instance = script.instantiateScript();
				script.m_instance->m_entity = Entity{ entity, this };
				script.m_instance->onCreate();
			}

			if (script.m_instance->m_enabled) script.m_instance->onUpdate(dt);//don't update if entity is disabled
		}
		
		Renderer::getInstance()->renderScene(dt, *this);
		
		glfwPollEvents();

		//UI window
		if (showUI) {
			glfwMakeContextCurrent(m_window);
			renderUI();
			glfwSwapBuffers(m_window);
		}
		
		//Execute onLateUpdate().
		for (auto [entity, script] : entities.each())
		{
			if (script.m_instance->m_enabled) script.m_instance->onLateUpdate(dt);//don't update if entity is disabled
		}

		glfwPollEvents();
	}
#pragma endregion

#pragma region UI Management
	bool Scene::initializeUI()
	{
		//Initialize the UI using ImGui OpenGL v3.3
		if (!UserInterface::initialize(m_window))
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
		const auto panelWidth = 0.2f * m_windowWidth;
		const auto halfWindowHeight = 0.5f * (m_windowHeight - menuHeight);

		m_tagDialog.setPosition(glm::uvec2(0.5f * m_windowWidth, 0.5f * m_windowHeight));

		m_explorerPanel.setPosition(glm::uvec2(0, menuHeight));
		m_explorerPanel.setDimension(glm::uvec2(panelWidth, m_windowHeight));

		m_gamePanel.setPosition(glm::uvec2(panelWidth + 3, menuHeight));
		m_gamePanel.setDimension(glm::uvec2(panelWidth * 3, halfWindowHeight));

		m_hierarchyPanel.setPosition(glm::uvec2(panelWidth * 4 - 4, menuHeight));
		m_hierarchyPanel.setDimension(glm::uvec2(panelWidth, halfWindowHeight));

		m_inspectorPanel.setPosition(glm::uvec2(panelWidth * 4 - 4, halfWindowHeight + menuHeight));
		m_inspectorPanel.setDimension(glm::uvec2(panelWidth, halfWindowHeight));

		m_componentsPanels[(int)ComponentsPanel::PanelType::Components].setPosition(glm::uvec2(panelWidth * 3 - 1, menuHeight));
		m_componentsPanels[(int)ComponentsPanel::PanelType::Components].setDimension(glm::uvec2(panelWidth, halfWindowHeight));


		//TODO - Does this need to be read from a .json file?
		m_componentsPanels[(int)ComponentsPanel::PanelType::Components].addComponent("Transform");
		m_componentsPanels[(int)ComponentsPanel::PanelType::Components].addComponent("Camera");
		m_componentsPanels[(int)ComponentsPanel::PanelType::Components].addComponent("Color");
		//m_componentsPanels[(int)ComponentsPanel::PanelType::Components].addComponent("Animation");
		m_componentsPanels[(int)ComponentsPanel::PanelType::Components].addComponent("Texture");

		m_componentsPanels[(int)ComponentsPanel::PanelType::Components].setPosition(glm::uvec2(panelWidth * 4 - 2, menuHeight + 0.5f * (m_windowHeight - menuHeight)));
		m_componentsPanels[(int)ComponentsPanel::PanelType::Components].setDimension(glm::uvec2(panelWidth, halfWindowHeight));

		m_componentsPanels[(int)ComponentsPanel::PanelType::Components].setPosition(glm::uvec2(panelWidth * 3 - 3, menuHeight + 0.5f * (m_windowHeight - menuHeight)));
		m_componentsPanels[(int)ComponentsPanel::PanelType::Components].setDimension(glm::uvec2(panelWidth, halfWindowHeight));

		m_inspectorPanel.setRegistry(&m_registry);
		
		return true;

	}

	void Scene::shutdownUI()
	{
		UserInterface::shutdown();
	}

	void Scene::renderUI()
	{
		UserInterface::startUI();

		m_mainMenu.show();

		m_gamePanel.show(); //This function will take in an fbo when one is created

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
			Engine::Serializer::serializeScene(this, "testScene");
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
				tag = "<Entity_" + std::to_string(m_registry.size() + 1) + ">";
			}

			Entity entity = createEntity(tag);
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
				auto camera = m_registry.any_of<CameraComponent>(handle);

				//Only add this component if there isn't already one attached
				if (!camera)
				{
					m_registry.emplace<CameraComponent>(
						handle,
						90.f,
						glm::mat4(1.f),
						glm::vec2(4.8, 4.8),
						100.0f,
						0.1f
						);
				}
			}

			if (component == "Transform")
			{
				auto transform = m_registry.any_of<TransformComponent>(handle);

				//Only add this component if there isn't already one attached
				if (!transform)
				{
					m_registry.emplace<TransformComponent>(
						handle,
						glm::vec3(0, 0, 0),
						glm::vec3(1, 1, 1),
						glm::vec3(0, 0, 0));
				}
			}

			if (component == "Color")
			{
				auto color = m_registry.any_of<ColorComponent>(handle);

				//Only add this component if there isn't already one attached
				if (!color)
				{
					m_registry.emplace<ColorComponent>(
						handle,
						glm::vec4(1, 1, 1, 1.0f));
				}
			}

			if (component == "Texture")
			{
				auto texture = m_registry.any_of<TextureComponent>(handle);

				//Only add this component if there isn't already one attached
				if (!texture)
				{
					m_registry.emplace<TextureComponent>(
						handle,
						0,
						"<Texture>");
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

#pragma region Entity Creation
	//Create an entity from the m_registry with the provided tag component, and return the entity.
	Entity Scene::createEntity(std::string tag)
	{
		Entity entity(m_registry.create(), this);
		entity.addComponent<TagComponent>(tag);
		m_hierarchyPanel.addEntity(tag, entity.getHandle());
		return entity;
	}
#pragma endregion
}