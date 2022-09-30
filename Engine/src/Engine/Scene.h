#pragma once

#include "entt.h"
#include "Components.h"

namespace Engine
{
    class Entity;

    class Scene
    {
    public:

        // Function to create an entity and add it to the scene
        // Takes in a tag as a string for input
        Entity createEntity(std::string tag);

        void onRuntimeStart();
        void onRuntimeStop();
        void onRuntimeUpdate();

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
        int m_windowWidth = 480;
        int m_windowHeight = 480;


        GLuint m_programId;

        friend class Entity;
    };
}
