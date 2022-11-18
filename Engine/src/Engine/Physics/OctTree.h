#pragma once

#include <list>
#include "glm/glm.hpp"
#include "AABB.h"
#include "Ray.h"
#include "OctNode.h"

namespace Engine
{
	class Entity;

	class OctTree
	{
	public:
		OctTree(glm::vec3& dimensions, glm::vec3& center = glm::vec3());
		~OctTree();

		OctTree(OctTree& other) = delete;
		void operator=(OctTree& other) = delete;

		std::list<Entity*> raycast(Ray& ray);
		std::list<Entity*> raycast(glm::vec3& origin, glm::vec3& vector);
		bool insert(Entity* entity);
		bool remove(Entity* entity);
	protected:
	private:
		OctNode* m_root;
	};
};