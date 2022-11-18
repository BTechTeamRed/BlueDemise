#include "OctTree.h"
#include "Engine/SceneBuilder/Components.h"
#include "Engine/SceneBuilder/Entity.h"
#include "Engine/Utilities/Log.h"

using namespace Engine;

OctTree::OctTree(glm::vec3& dimensions, glm::vec3& center)
	:m_root(new OctNode(dimensions, center))
{}

OctTree::~OctTree()
{
	if (m_root)
	{
		delete m_root;
	}
}

std::list<Entity*> OctTree::raycast(Ray& ray)
{
	return m_root->raycast(ray);
}

std::list<Entity*> OctTree::raycast(glm::vec3& origin, glm::vec3& vector)
{
	Ray ray(origin, vector);
	return m_root->raycast(ray);
}

bool OctTree::insert(Entity* entity)
{
	//TODO: insertion code
	bool inserted = false;
	PhysicsComponent* component = nullptr;
	if (entity->hasComponent<PhysicsComponent>())
	{
		component = &entity->getComponent<PhysicsComponent>();
		inserted = m_root->insert(entity, component);
	}
	std::string tag = entity->getComponent<TagComponent>().tag;
	std::string result = inserted ? "OctTree::insert: {0} inserted" : "OctTree::insert: {0} failed insertion";
	GE_CORE_TRACE(result, tag);

	return inserted;
}

bool OctTree::remove(Entity* entity)
{
	//TODO: removal code
	bool removed = false;
	PhysicsComponent* component = nullptr;
	if (entity->hasComponent<PhysicsComponent>())
	{
		component = &entity->getComponent<PhysicsComponent>();
		removed = m_root->remove(entity, component);
	}

	return removed;
}