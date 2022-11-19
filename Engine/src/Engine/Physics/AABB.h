#pragma once

#include "glm/glm.hpp"	
#include "Ray.h"

namespace Engine
{
	/// <summary>
	/// Wrapper for corner indicies
	/// </summary>
	enum Corner
	{
		LTF = 0, // Left-Top-Front
		LTB,	 // Left-Top-Back
		RTF,	 // Right-Top-Front
		RTB,	 // Right-Top-Back
		LBF,	 // Left-Bottom-Front
		LBB,	 // Left-Bottom-Back
		RBF,	 // Right-Bottom-Front
		RBB		 // Right-Bottom-Back
	};

	/// <summary>
	/// Axis-aligned bounding box
	/// </summary>
	class AABB
	{
	public:
		/// <summary>
		/// C'tor
		/// </summary>
		/// <param name="dimensions">Dimensions of the AABB</param>
		/// <param name="position">Center of the AABB</param>
		AABB(glm::vec3& dimensions, glm::vec3& position);
		//TODO: Copy constructor
		// D'tor
		~AABB();

		/// <summary>
		/// Intersects this AABB with the one passed in - to be implemented
		/// </summary>
		/// <param name="other">The AABB to check against</param>
		/// <returns>True if the boxes intersect</returns>
		bool intersect(AABB* other);
		/// <summary>
		/// Intersects the AABB with the passed in ray
		/// </summary>
		/// <param name="ray">Ray to check against</param>
		/// <returns>True if the ray intersects the AABB</returns>
		bool intersect(Ray& ray);
		/// <summary>
		/// Checks whether the point exists in the space contained by the AABB
		/// </summary>
		/// <param name="point">The point</param>
		/// <returns>True if the point exists within the bounds of the AABB</returns>
		bool containsPoint(glm::vec3& point);
		/// <summary>
		/// Checks wether an AABB is contained by this AABB
		/// </summary>
		/// <param name="other">The box to check</param>
		/// <returns>True if the passed in AABB exists within this AABB</returns>
		bool containsBox(AABB* other);
		/// <summary>
		/// Updates the dimensions of the AABB
		/// </summary>
		/// <param name="dimensions">New dimensions of the AABB</param>
		void updateDimensions(glm::vec3& dimensions);
		/// <summary>
		/// Updates the position of the AABB
		/// </summary>
		/// <param name="position">New position of the AABB</param>
		void updatePosition(glm::vec3& position);
		/// <summary>
		/// Getter for a specific corner offset
		/// </summary>
		/// <param name="corner">Corner to get</param>
		/// <returns>The offset</returns>
		glm::vec3 getCorner(Corner corner);
		/// <summary>
		/// Getter for the dimensions of the AABB
		/// </summary>
		/// <returns>The dimensions</returns>
		glm::vec3& getDimensions();
		/// <summary>
		/// Getter for the position/center of the AABB
		/// </summary>
		/// <returns>The position</returns>
		glm::vec3& getPosition();
	protected:
	private:
		/// <summary>
		/// Dimensions of the AABB in units
		/// </summary>
		glm::vec3 m_dimensions;
		/// <summary>
		/// Offset for the different corners of the AABB
		/// </summary>
		glm::vec3* m_corners;
		/// <summary>
		/// Position of the AABB anchored at the center of the AABB
		/// </summary>
		glm::vec3 m_position;

		/// <summary>
		/// Code to update the corner offsets given the dimensions of the AABB
		/// </summary>
		inline void updateCorners();
	};
}