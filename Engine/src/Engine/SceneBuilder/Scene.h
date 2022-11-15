#pragma once

#include "entt.h"
#include "Components.h"
#include <Engine/Utilities/DeltaTime.h>
#include "Engine/Core.h"
#include <string>

//Added for UI
#include <array>
#include "Engine/EngineUI/ExplorerPanel.h"
#include "Engine/EngineUI/EntitiesPanel.h"
#include "Engine/EngineUI/ComponentsPanel.h"
#include "Engine/EngineUI/MainMenu.h"

namespace Engine
{
    class Entity;

    // Scene keeps a registry of Entities that are then rendered and processed as part of the game loop.
    // Also responsible for loading shaders, initializing OpenGl contexts and windows, and rendering.
    class ENGINE_API Scene
    {
    public:
        bool m_closeScene = false;

        #pragma region Entity Management
		
        // Function to create an entity and add it to the scene
        // Takes in a tag as a string for input
        Entity createEntity(std::string tag);

        // Gets a view of entities with the defined components.
        template<typename... Components>
        auto getEntities()
        {
            return m_registry.view<Components...>();
        }
        #pragma endregion
        
        #pragma region Runtime Functions
        // Executes actions at the start of runtime
        void onRuntimeStart();

        // Executes actions when runtime stops
        void onRuntimeStop();

        // Executes actions every time runtime is updated (every frame).
        void onRuntimeUpdate(const DeltaTime& dt);
        #pragma endregion


    private:
        std::string m_name;

        //main menu is the UI element that shows the game
        MainMenu m_mainMenu;

        //explorer panel is the UI element that shows the file explorer
        ExplorerPanel m_explorerPanel;

        //entities panel is the UI element that lists all the entities
        EntitiesPanel m_entitiesPanel;

        DeltaTime m_deltaTime{0};
        
        //creates an array of three components panels
        std::array<ComponentsPanel, 3> m_componentsPanels
        {
            "Active components",
            "Attributes",
            "Components"
        };

		//GL IDs for various objects. 
        GLuint m_programId;

        friend class Entity;
        friend class Serializer;

    public:
        // Registry is a container to hold entities
        //Made public to allow for GLFW callbacks to access entities
        entt::registry m_registry;
    };

    void windowResizeCallback(GLFWwindow* window, int width, int height);
}
