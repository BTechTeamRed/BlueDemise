#pragma once

#include "glm/glm.hpp"

namespace Engine
{
	class Ray
	{
	public:
		Ray(glm::vec3& origin, glm::vec3& vector)
			:m_origin(origin), m_vector(glm::normalize(vector)) {}

		glm::vec3& getVector() { return m_vector; }
		glm::vec3& getOrigin() { return m_origin;  }
	protected:
	private:
		glm::vec3 m_vector;
		glm::vec3 m_origin;
	};
}