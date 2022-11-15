#pragma once

#include <list>
#include <vector>
#include "glm/glm.hpp"
#include "SceneBuilder/Entity.h"
#include "SceneBuilder/Components.h"
#include "AABB.h"
#include "Ray.h"

struct PhysicsComponent;

namespace Engine
{
	class OctNode
	{
	public:
		OctNode(glm::vec3& center, glm::vec3& dimensions, OctNode* parent = nullptr);
		~OctNode();

		OctNode(OctNode& other) = delete;
		void operator=(OctNode& other) = delete;

		void update();
		std::list<Entity*>& raycast(Ray& ray);
		bool insert(Entity* entity, PhysicsComponent* component);
		bool remove(Entity* entity, PhysicsComponent* component);
		AABB* getAABB();
	protected:
	private:
		void subdivide();
		void rebalance();

		AABB* m_bounds;
		std::list<std::pair<Entity*, PhysicsComponent*>> m_entityList;
		OctNode* m_parent;
		OctNode** m_children;

		const static int REBALANCE_FACTOR;
	};

	class OctTree
	{
	public:
		explicit OctTree(glm::vec3& dimensions)
			:m_root(new OctNode(dimensions, glm::vec3())) {}
		~OctTree()
		{
			if (m_root)
			{
				delete m_root;
			}
		}

		OctTree(OctTree& other) = delete;
		void operator=(OctTree& other) = delete;

		std::list<Entity*>& raycast(Ray& ray);
		std::list<Entity*>& raycast(glm::vec3& origin, glm::vec3& vector);
		bool insert(Entity* entity);
		bool remove(Entity* entity);
	protected:
	private:
		OctNode* m_root;
	};
};