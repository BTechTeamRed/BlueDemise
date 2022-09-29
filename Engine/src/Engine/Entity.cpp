#include "Entity.h"

namespace Engine
{
    Entity::Entity(entt::entity entityHandle, Scene* scene)
        : m_entityHandle(entityHandle)
		, m_scene(scene)
    {}
}
