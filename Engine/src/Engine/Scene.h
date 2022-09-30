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
        void renderScene();

        //TODO: Replace with serialized objects once that's added in
        VerticesComponent createTriangle();

    private:
        // Registry is a container to hold entities
        entt::registry m_registry;
        struct GLFWwindow* m_window;
        GLuint m_programId;
        struct DefaultShaders {
            GLuint fillShader;
        } defaultShaders;

        unsigned int m_vbo, m_ibo, m_vao;

        friend class Entity;
    };
}
