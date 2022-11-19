#pragma once

#include "glm/glm.hpp"	
#include "Ray.h"

namespace Engine
{
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

	class AABB
	{
	public:
		explicit AABB(glm::vec3& dimensions, glm::vec3& position);
		//TODO: copy constructor
		~AABB();

		bool intersect(AABB* other);
		bool intersect(Ray& ray);
		bool containsPoint(glm::vec3& point);
		bool containsBox(AABB* other);
		void updateDimensions(glm::vec3& dimensions);
		void updatePosition(glm::vec3& position);
		glm::vec3 getCorner(Corner corner);
		glm::vec3& getDimensions();
		glm::vec3& getPosition();
	protected:
	private:
		glm::vec3 m_dimensions;
		glm::vec3* m_corners;
		glm::vec3 m_position;

		inline void updateCorners();
	};
}