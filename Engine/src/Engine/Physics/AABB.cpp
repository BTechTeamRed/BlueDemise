#include "AABB.h"

using namespace Engine;

AABB::AABB(glm::vec3& dimensions, glm::vec3& position)
	:m_dimensions(dimensions), m_position(position), m_corners(new glm::vec3[8])
{
	updateCorners();
}

AABB::~AABB()
{
	if (m_corners)
	{
		delete m_corners;
	}
}

bool AABB::intersect(AABB* other)
{
	//TODO: speration axis theorem
	return false;
}

bool AABB::intersect(Ray& ray)
{
	//TODO: ray intersection code
	bool intersect = false;
	float denom;
	float t;
	glm::vec3 point;
	glm::vec3 normals[6];
	glm::vec3 origin[6];
	
	// Set origin point for face planes
	origin[0] = origin[1] = origin[2] = m_corners[LBB];
	origin[3] = origin[4] = origin[5] = m_corners[RTF];

	// Top-face
	normals[0] = m_corners[LTB] - m_corners[LBB];
	// Front-face
	normals[1] = m_corners[LBF] - m_corners[LBB];
	// Right-face
	normals[2] = m_corners[RBB] - m_corners[LBB];
	// Back-face
	normals[3] = m_corners[RTB] - m_corners[RTF];
	// Left-face
	normals[4] = m_corners[LTF] - m_corners[RTF];
	// Bottom-face
	normals[5] = m_corners[RBF] - m_corners[RBF];

	// Check ray against face planes
	for (int i = 0; !intersect && i < 6; ++i)
	{
		denom = glm::dot(normals[i], ray.getVector());
		// Ignore parallel faces
		if (denom != 0.0f)
		{
			t = glm::dot(origin[i] - ray.getOrigin(), normals[i]);
			t /= denom;
			point = ray.getOrigin() + ray.getVector() * t;
			intersect = containsPoint(point);
		}
	}

	return intersect;
}

bool AABB::containsPoint(glm::vec3& point)
{
	// Check x-axis
	bool xCheck = point.x >= m_corners[0].x + m_position.x && 
		point.x <= m_corners[3].x + m_position.x;
	// Check y-axis
	bool yCheck = point.y <= m_corners[0].y + m_position.y && 
		point.y >= m_corners[4].y + m_position.y;
	// Check z-axis
	bool zCheck = point.z <= m_corners[0].z + m_position.z && 
		point.z >= m_corners[1].z + m_position.z;
	return xCheck && yCheck && zCheck;
}

bool AABB::containsBox(AABB* other)
{
	// Check if this AABB contains the corners of the other AABB
	return containsPoint(other->m_corners[0] + other->m_position) 
		&& containsPoint(other->m_corners[1] + other->m_position)
		&& containsPoint(other->m_corners[2] + other->m_position)
		&& containsPoint(other->m_corners[3] + other->m_position)
		&& containsPoint(other->m_corners[4] + other->m_position)
		&& containsPoint(other->m_corners[5] + other->m_position)
		&& containsPoint(other->m_corners[6] + other->m_position)
		&& containsPoint(other->m_corners[7] + other->m_position);
}

void AABB::updateDimensions(glm::vec3& dimensions)
{
	m_dimensions = dimensions;
	updateCorners();
}

void AABB::updatePosition(glm::vec3& position)
{
	m_position = position;
}

void AABB::updateCorners()
{
	// Using RHR
	float front = m_dimensions.z / 2;
	float back = -front;
	float right = m_dimensions.x / 2;
	float left = -right;
	float top = m_dimensions.y / 2;
	float bottom = -top;

	m_corners[0] = glm::vec3(left, top, front);
	m_corners[1] = glm::vec3(left, top, back);
	m_corners[2] = glm::vec3(right, top, front);
	m_corners[3] = glm::vec3(right, top, back);
	m_corners[4] = glm::vec3(left, bottom, front);
	m_corners[5] = glm::vec3(left, bottom, back);
	m_corners[6] = glm::vec3(right, bottom, front);
	m_corners[7] = glm::vec3(right, bottom, back);
}

glm::vec3& AABB::getDimensions()
{
	return m_dimensions;
}

glm::vec3& AABB::getPosition()
{
	return m_position;
}