#pragma once

#include "entt.h"
#include "Components.h"

namespace Engine
{
    class Entity;

    class Scene
    {
    private:
        // Registry is a container to hold entities
        entt::registry m_registry;
    public:
        Scene() = default;
        ~Scene() = default;

        // Function to create an entity and add it to the scene
        // Takes in a tag as a string for input
        Entity createEntity(std::string tag);

        friend class Entity;
    };
}
