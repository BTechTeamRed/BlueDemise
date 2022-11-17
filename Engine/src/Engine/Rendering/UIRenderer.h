#pragma once
#include "Engine/Core.h"
#include <array>
#include "Engine/EngineUI/ComponentsPanel.h"
#include "Engine/EngineUI/ExplorerPanel.h"
#include "Engine/EngineUI/EntitiesPanel.h"
#include "Engine/EngineUI/MainMenu.h"
#include "Engine/EngineUI/GamePanel.h"
#include "Engine/EngineUI/InspectorPanel.h"
#include "Engine/EngineUI/TagDialog.h"
#include "Engine/EngineUI/HierarchyPanel.h"

namespace Engine
{
    class Scene;
    class Window;

	class ENGINE_API UIRenderer
	{
	public:
        bool initializeUI(Window& window, Scene& scene);
        void shutdownUI();
        void renderUI(Scene& scene);
        void updateHierarchyPanel(std::string tag, Entity entity);

	private:
        //creates an array of three components panels
        std::array<ComponentsPanel, 3> m_componentsPanels
        {
            "Active components",
            "Attributes",
            "Components"
        };

        //main menu is the UI element that shows the game
        MainMenu m_mainMenu;

        //explorer panel is the UI element that shows the file explorer
        ExplorerPanel m_explorerPanel;

        //entities panel is the UI element that lists all the entities
        HierarchyPanel m_hierarchyPanel;

        //inspector panel is the UI element that lists all the components for each entity
        InspectorPanel m_inspectorPanel;

        //game panel is the UI element that shows the render of the game
        GamePanel m_gamePanel;

        //dialog box to allow user to input the tag name for an entity
        TagDialog m_tagDialog;
	};
}