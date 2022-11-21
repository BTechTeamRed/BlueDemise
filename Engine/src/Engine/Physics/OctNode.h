#pragma once

#include <list>
#include <vector>
#include "glm/glm.hpp"

namespace Engine
{
	// Forward declarations
	struct PhysicsComponent;
	class Entity;
	class AABB;
	class Ray;

	/// <summary>
	/// Node of an oct-tree graph
	/// </summary>
	class OctNode
	{
	public:
		/// <summary>
		/// C'tor
		/// </summary>
		/// <param name="center">Center of the node</param>
		/// <param name="dimensions">Dimensions of the node</param>
		/// <param name="parent">Parent node, null if root</param>
		OctNode(glm::vec3& center, glm::vec3& dimensions, OctNode* parent = nullptr);

		// D'tor
		~OctNode();

		// Prevent copying
		OctNode(OctNode& other) = delete;
		void operator=(OctNode& other) = delete;

		/// <summary>
		/// Check integerity of nodes
		/// </summary>
		void update();

		/// <summary>
		/// Cast a ray into the world and intersect objects
		/// </summary>
		/// <param name="ray">Ray to intersect with</param>
		/// <returns>List of entities that ray intersected</returns>
		std::list<Entity*> raycast(Ray& ray);

		/// <summary>
		/// Insert entity into node or child node
		/// </summary>
		/// <param name="entity">Entity to insert</param>
		/// <param name="component">Physics component of the entity</param>
		/// <returns>True on successful insertion</returns>
		bool insert(Entity* entity, PhysicsComponent* component);

		/// <summary>
		/// Remove entity from node or child node
		/// </summary>
		/// <param name="entity">Entity to remove</param>
		/// <param name="component">Physics component of the entity</param>
		/// <returns>True on successful removal</returns>
		bool remove(Entity* entity, PhysicsComponent* component);

		/// <summary>
		/// Getter for the AABB
		/// </summary>
		/// <returns>The AABB</returns>
		AABB* getAABB();
	protected:
	private:
		/// <summary>
		/// Create children nodes
		/// </summary>
		void subdivide();

		/// <summary>
		/// Rebalance tree by pushing objects up or down as appropriate
		/// </summary>
		void rebalance();

		/// <summary>
		/// Based on the center position get child that object should check against
		/// </summary>
		/// <param name="position">Position of the object</param>
		/// <returns>-1 on failure, index of child on success</returns>
		int getChild(glm::vec3& position);

		/// <summary>
		/// AABB of the node
		/// </summary>
		AABB* m_bounds;

		/// <summary>
		/// List of entities that exist in the node
		/// </summary>
		std::list<std::pair<Entity*, PhysicsComponent*>> m_entityList;

		/// <summary>
		/// Pointer to the parent node
		/// </summary>
		OctNode* m_parent;

		/// <summary>
		/// Array of children nodes
		/// </summary>
		OctNode** m_children;

		/// <summary>
		/// Number of entities that can exist in a node before rebalancing
		/// </summary>
		const static int REBALANCE_FACTOR;
	};
}