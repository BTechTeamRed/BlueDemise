#include "Scene.h"
#include "Entity.h"

// Function to create an entity and add it to the scene
// Takes in a tag as a string for input
Entity Scene::createEntity(std::string tag)
{
    Entity entity(m_registry.create(), this);
    entity.addComponent<TagComponent>(tag);
    return entity;
}