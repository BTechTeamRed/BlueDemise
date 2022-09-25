#include "Scene.h"
#include "Entity.h"

namespace Engine
{
    Entity Scene::createEntity(std::string tag)
    {
        Entity entity(m_registry.create(), this);
        entity.addComponent<TagComponent>(tag);
        return entity;
    }
}
