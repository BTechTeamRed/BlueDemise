#pragma once

#include "entt.h"
#include "Components.h"

//Added for UI
#include <array>
#include "ExplorerPanel.h"
#include "EntitiesPanel.h"
#include "ComponentsPanel.h"
#include "MainMenu.h"

namespace Engine
{
    class Entity;

    // Scene keeps a registry of Entities that are then rendered and processed as part of the game loop.
    // Also responsible for loading shaders, initializing OpenGl contexts and windows, and rendering.
    class Scene
    {
    public:

        // Function to create an entity and add it to the scene
        // Takes in a tag as a string for input
        Entity createEntity(std::string tag);

        // Executes actions at the start of runtime
        void onRuntimeStart();

        // Executes actions when runtime stops
        void onRuntimeStop();

        // Executes actions every time runtime is updated (every frame).
        void onRuntimeUpdate();

        // Gets a view of entities with the defined components.
        template<typename... Components>
        auto getEntities()
        {
            return m_registry.view<Components...>();
        }

    private:
        bool initializeGL();
        void renderScene();

        //TODO: shader wrapper so switching out between different shaders is easier
        void loadShaders();

        //TODO: Replace with serialized objects once that's added in
        void createEntities();
        VerticesComponent createTriangle();

    private:
        // Registry is a container to hold entities
        entt::registry m_registry;
        struct GLFWwindow* m_window;
        int m_windowWidth{1900};
        int m_windowHeight{1000};

        //main menu is the UI element that shows the game
        MainMenu m_mainMenu;

        //explorer panel is the UI element that shows the file explorer
        ExplorerPanel m_explorerPanel;

        //entities panel is the UI element that lists all the entities
        EntitiesPanel m_entitiesPanel;

        //creates an array of three components panels
        std::array<ComponentsPanel, 3> m_componentsPanels
        {
            "Active components",
            "Attributes",
            "Components"
        };

        GLuint m_programId;

        friend class Entity;
    };
}
