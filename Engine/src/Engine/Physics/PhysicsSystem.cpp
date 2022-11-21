#include "PhysicsSystem.h"
#include "Engine/SceneBuilder/Components.h"
#include "Engine/SceneBuilder/Entity.h"
#include "Engine/Utilities/Log.h"
#include "Engine/Physics/OctNode.h"
#include "Engine/Physics/Ray.h"

using namespace Engine;

PhysicsSystem::PhysicsSystem(glm::vec3& dimensions, glm::vec3& center)
	:m_root(new OctNode(dimensions, center)), m_entityList()
{}

PhysicsSystem::~PhysicsSystem()
{
	if (m_root)
	{
		delete m_root;
	}
	for (auto entity : m_entityList)
	{
		delete entity;
	}
}

void PhysicsSystem::update()
{
	m_root->update();
}

std::list<Entity*> PhysicsSystem::raycast(Ray& ray)
{
	return m_root->raycast(ray);
}

std::list<Entity*> PhysicsSystem::raycast(glm::vec3& origin, glm::vec3& vector)
{
	Ray ray(origin, vector);
	return m_root->raycast(ray);
}

bool PhysicsSystem::insert(Entity* entity)
{
	//TODO: insertion code
	bool inserted = false;
	PhysicsComponent* component = nullptr;
	if (entity->hasComponent<PhysicsComponent>())
	{
		component = &entity->getComponent<PhysicsComponent>();
		inserted = m_root->insert(entity, component);
	}
	if (inserted)
	{
		m_entityList.push_back(entity);
	}

	return inserted;
}

bool PhysicsSystem::remove(Entity* entity)
{
	bool removed = false;
	PhysicsComponent* component = nullptr;
	if (entity->hasComponent<PhysicsComponent>())
	{
		component = &entity->getComponent<PhysicsComponent>();
		removed = m_root->remove(entity, component);
	}
	auto it = m_entityList.begin();
	if (removed)
	{
		for (; (*it)->getHandle() != entity->getHandle() && it != m_entityList.end(); ++it)
		{
		}
		if (it != m_entityList.end())
		{
			m_entityList.erase(it);
		}
	}

	return removed;
}