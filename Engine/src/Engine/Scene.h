#pragma once

#include "../../../thirdparty/entt/entt.h"
#include "Components.h"

class Entity;

class Scene {

private:
    // Registry is a container to hold entities
    entt::registry m_registry;
public:
    Scene() = default;
    ~Scene() = default;
    Entity createEntity(std::string tag);

    friend class Entity;
};