#pragma once

#include <list>
#include <vector>
#include "glm/glm.hpp"
#include "AABB.h"

namespace Engine
{
	struct PhysicsComponent;
	class Entity;

	class OctNode
	{
	public:
		OctNode(glm::vec3& center, glm::vec3& dimensions, OctNode* parent = nullptr);
		~OctNode();

		OctNode(OctNode& other) = delete;
		void operator=(OctNode& other) = delete;

		void update();
		std::list<Entity*> raycast(Ray& ray);
		bool insert(Entity* entity, PhysicsComponent* component);
		bool remove(Entity* entity, PhysicsComponent* component);
		AABB* getAABB();
	protected:
	private:
		void subdivide();
		void rebalance();
		int getChild(glm::vec3& position);

		AABB* m_bounds;
		std::list<std::pair<Entity*, PhysicsComponent*>> m_entityList;
		OctNode* m_parent;
		OctNode** m_children;

		const static int REBALANCE_FACTOR;
	};
}