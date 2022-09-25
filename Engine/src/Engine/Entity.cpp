#include "entity.h"

namespace Engine
{
    Entity::Entity(entt::entity entity_handle, Scene* scene)
        : m_entityHandle(entity_handle), m_scene(scene)
    {}
}
