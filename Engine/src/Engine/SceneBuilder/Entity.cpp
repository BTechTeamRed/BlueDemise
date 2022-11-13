#include "Entity.h"

namespace Engine
{
    int Entity::s_totalEntities = 0;

    int Entity::getTotalEntities()
    {
        return s_totalEntities;
    }

    Entity::Entity(entt::entity entityHandle, Scene* scene)
        : m_scene(scene), m_entityHandle(entityHandle)
    {
        s_totalEntities++;
    }
}
