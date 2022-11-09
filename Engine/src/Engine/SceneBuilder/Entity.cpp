#include "Entity.h"

namespace Engine
{
    Entity::Entity(entt::entity entityHandle, Scene* scene)
        : m_scene(scene), m_entityHandle(entityHandle)
    {}
}
