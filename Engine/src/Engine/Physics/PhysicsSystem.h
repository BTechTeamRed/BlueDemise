#pragma once

#include <list>
#include "glm/glm.hpp"

namespace Engine
{
	// Forward declarations
	class Entity;
	class AABB;
	class Ray;
	class OctNode;

	/// <summary>
	/// Handle physics via an oct-tree: 3D space graph where space is divided into 8 containers, each representing a portion of the space
	/// </summary>
	class PhysicsSystem
	{
	public:
		/// <summary>
		/// C'tor
		/// </summary>
		/// <param name="dimensions">Dimensions of the world</param>
		/// <param name="center">Center of the world - default: 0 0 0 </param>
		PhysicsSystem(glm::vec3& dimensions, glm::vec3& center = glm::vec3());

		// D'tor
		~PhysicsSystem();

		// Prevent copying
		PhysicsSystem(PhysicsSystem& other) = delete;
		void operator=(PhysicsSystem& other) = delete;
		
		/// <summary>
		/// Update root node
		/// </summary>
		void update();

		/// <summary>
		/// Raycast using specified ray
		/// </summary>
		/// <param name="ray">Ray to check against</param>
		/// <returns>List of entities intersected by ray</returns>
		std::list<Entity*> raycast(Ray& ray);

		/// <summary>
		/// Raycast using a ray starting at the origin haeding along vector
		/// </summary>
		/// <param name="origin">Origin point of the ray</param>
		/// <param name="vector">Heading of the ray</param>
		/// <returns>List of entities intersected by ray</returns>
		std::list<Entity*> raycast(glm::vec3& origin, glm::vec3& vector);

		/// <summary>
		/// Insert specified entity into the world
		/// </summary>
		/// <param name="entity">Entity to insert</param>
		/// <returns>True if entity is inserted</returns>
		bool insert(Entity* entity);

		/// <summary>
		/// Remove specified entity from the world
		/// </summary>
		/// <param name="entity">Entity to remove</param>
		/// <returns>True if entity was removed</returns>
		bool remove(Entity* entity);
	protected:
	private:
		/// <summary>
		/// Root node of the tree
		/// </summary>
		OctNode* m_root;

		/// <summary>
		/// List of entities successfully inserted into the system
		/// </summary>
		std::list<Entity*> m_entityList;
	};
};