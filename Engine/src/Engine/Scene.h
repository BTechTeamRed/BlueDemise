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

        void fakeStart();
        void fakeUpdate();

        template<typename... Components>
        auto getEntities()
        {
            return m_registry.view<Components...>();
        }

        void fakeStart();
        void fakeUpdate();

    private:
        void renderScene();

        //TODO: Replace with serialized objects once that's added in
        VerticesComponent createTriangle();

    private:
        // Registry is a container to hold entities
        entt::registry m_registry;
        struct GLFWwindow* m_window;
        int m_programId;

        unsigned int m_vbo, m_ibo, m_vao;

        friend class Entity;
    };
}
