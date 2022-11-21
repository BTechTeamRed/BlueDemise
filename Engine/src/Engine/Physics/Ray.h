#pragma once

#include "glm/glm.hpp"

namespace Engine
{
	/// <summary>
	/// Class representing a mathematical ray
	/// </summary>
	class Ray
	{
	public:
		/// <summary>
		/// C'tor
		/// </summary>
		/// <param name="origin">Origin of the ray in world space</param>
		/// <param name="vector">Direction of the ray (unit vector)</param>
		Ray(glm::vec3& origin, glm::vec3& vector)
			:m_origin(origin), m_vector(glm::normalize(vector)) {}

		/// <summary>
		/// Getter for vector
		/// </summary>
		/// <returns>The direction of the ray</returns>
		glm::vec3& getVector() { return m_vector; }
		/// <summary>
		/// Getter for the origin of the ray
		/// </summary>
		/// <returns>The origin of the ray</returns>
		glm::vec3& getOrigin() { return m_origin;  }
	protected:
	private:
		/// <summary>
		/// Direction of the ray
		/// </summary>
		glm::vec3 m_vector;
		/// <summary>
		/// Origin of the ray
		/// </summary>
		glm::vec3 m_origin;
	};
}