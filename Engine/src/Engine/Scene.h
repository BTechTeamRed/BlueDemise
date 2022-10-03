#pragma once

#include "entt.h"
#include "ImportResources.h"
#include "Components.h"
#include "SourceGatherer.h"

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
        int m_windowWidth{480};
        int m_windowHeight{480};
		
        ImportResources m_importResources;

        GLuint m_programId;

        friend class Entity;
    };
}
