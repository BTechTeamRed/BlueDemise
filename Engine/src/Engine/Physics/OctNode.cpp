#include "OctNode.h"
#include "Engine/SceneBuilder/Components.h"
#include "Engine/SceneBuilder/Entity.h"
#include "Engine/Utilities/Log.h"

using namespace Engine;

const int OctNode::REBALANCE_FACTOR = 8;

OctNode::OctNode(glm::vec3& dimensions, glm::vec3& center, OctNode* parent)
	:m_bounds(new AABB(dimensions, center)), m_parent(parent), m_children(nullptr)
{
}

OctNode::~OctNode()
{
	for (int i = LTF; i <= RBB && m_children; ++i)
	{
		if (m_children[i])
		{
			delete m_children[i];
		}
	}
	if (m_children) delete m_children;
	if (m_bounds) delete m_bounds;
}

void OctNode::update()
{
	// Update children first
	if (m_children)
	{
		for (int i = LTF; i <= RBB; ++i)
		{
			m_children[i]->update();
		}
	}
	// Check integregity of self
	rebalance();
}

std::list<Entity*> OctNode::raycast(Ray& ray)
{
	// Can probably be made more efficent by casting ray against child's bounding box
	std::list<Entity*> picks;
	glm::vec3 dim = m_bounds->getDimensions();
	glm::vec3 pos = m_bounds->getPosition();
	// Check self
	for (auto ent : m_entityList)
	{
		glm::vec3 dDim = ent.second->boundingBox->getDimensions();
		glm::vec3 dPos = ent.second->boundingBox->getPosition();
		if (ent.second->boundingBox->intersect(ray))
		{
			picks.push_back(ent.first);
		}
	}
	// Check children
	for (int i = LTF; m_children && i <= RBB; ++i)
	{
		if (m_children[i]->getAABB()->intersect(ray))
		{
			for (auto pick : m_children[i]->raycast(ray))
			{
				picks.push_back(pick);
			}
		}
	}

	return picks;
}

bool OctNode::insert(Entity* entity, PhysicsComponent* component)
{
	bool inserted = false;
	//Check if we can push to children
	AABB* bounds = component->boundingBox;
	glm::vec3 compSize = bounds->getDimensions();
	glm::vec3 maxChildSize = m_bounds->getDimensions() / 2.0f;
	// See if entity would fit into a child node
	bool isChildSized = compSize.x < maxChildSize.x &&
						compSize.y < maxChildSize.y &&
						compSize.z < maxChildSize.z;
	// Check of rebalancing is required
	if (m_entityList.size() >= REBALANCE_FACTOR)
	{
		subdivide();
		rebalance();
	}
	// Check children
	if (isChildSized && m_children)
	{
		int child = getChild(bounds->getPosition());
		if (child > -1 && m_children[child]->getAABB()->containsBox(bounds))
		{
			inserted = m_children[child]->insert(entity, component);
		}
	}
	// Insert into self if item is within node
	if (!inserted && m_bounds->containsBox(bounds))
	{
		inserted = true;
		m_entityList.push_back(std::make_pair(entity, component));
	}

	return inserted;
}

bool OctNode::remove(Entity* entity, PhysicsComponent* component)
{
	bool removed = false;
	int child = getChild(component->boundingBox->getPosition());
	// DFS
	if (m_children && child > -1)
	{
		removed = m_children[child]->remove(entity, component);
	}
	// Remove from self
	if(!removed)
	{
		for (auto it = m_entityList.begin(); !removed && it != m_entityList.end(); ++it)
		{
			removed = it->first->getHandle() == entity->getHandle();
		}
		if (removed)
		{
			m_entityList.remove(std::make_pair(entity, component));
		}
	}

	return removed;
}

void OctNode::subdivide()
{
	if (!m_children)
	{
		glm::vec3 center = m_bounds->getPosition();
		// Calculate dimensions of children
		glm::vec3 dimensions = m_bounds->getDimensions() / 2.0f;
		// Calculate positions of children
		std::vector<glm::vec3> centers(8);
		// Left-side
		centers[LTF].x = centers[LTB].x = centers[LBF].x = centers[LBB].x = center.x - (dimensions.x / 2.0f);
		// Right-side
		centers[RTF].x = centers[RTB].x = centers[RBF].x = centers[RBB].x = center.x + (dimensions.x / 2.0f);
		// Top-side
		centers[LTF].y = centers[LTB].y = centers[RTF].y = centers[RTB].y = center.y + (dimensions.y / 2.0f);
		// Bottom-side
		centers[LBF].y = centers[LBB].y = centers[RBF].y = centers[RBB].y = center.y - (dimensions.y / 2.0f);
		// Front-side
		centers[LTF].z = centers[RTF].z = centers[LBF].z = centers[RBF].z = center.z + (dimensions.z / 2.0f);
		// Back-side
		centers[LTB].z = centers[RTB].z = centers[LBB].z = centers[RBB].z = center.z - (dimensions.z / 2.0f);

		m_children = new OctNode * [8];
		for (int i = LTF; i <= RBB; ++i)
		{
			m_children[i] = new OctNode(dimensions, centers[i], this);
		}
	}
}

void OctNode::rebalance()
{
	PhysicsComponent* component;
	AABB* bounds;

	glm::vec3 maxChildSize = m_bounds->getDimensions();
	glm::vec3 compSize;
	std::list<std::pair<Entity*, PhysicsComponent*>> removal;
	std::string tag;
	// Go through entities and see if we can push entities down tree
	bool moved;
	for (auto ent : m_entityList)
	{
		tag = ent.first->getComponent<TagComponent>().tag;
		moved = false;
		component = ent.second;
		bounds = component->boundingBox;
		compSize = bounds->getDimensions();
		// Push to parent
		if (m_parent && !m_bounds->containsBox(bounds))
		{
			m_parent->insert(ent.first, ent.second);
			// Whether inserted or not, cull entity from tree
			removal.push_back(ent);
		}
		// Push to children if child sized
		else if (m_children &&
				compSize.x < maxChildSize.x &&
				compSize.y < maxChildSize.y &&
				compSize.z < maxChildSize.z)
		{
			int child = getChild(bounds->getPosition());
			if (child > -1 && m_children[child]->getAABB()->containsBox(bounds))
			{
				if (m_children[child]->insert(ent.first, ent.second))
				{
					removal.push_back(ent);
				}
			}
		}
	}
	// Remove inserted entities
	for (auto pair : removal)
	{
		m_entityList.remove(pair);
	}
}

int OctNode::getChild(glm::vec3& position)
{
	glm::vec3 delta = position - m_bounds->getPosition();

	int child = -1;
	// Child exists on a boundry
	if (delta.x == 0 || delta.y == 0 || delta.z == 0)
	{
	}
	else if (delta.x < 0 && delta.y > 0 && delta.z > 0)
	{
		child = LTF;
	}
	else if (delta.x < 0 && delta.y > 0 && delta.z < 0)
	{
		child = LTB;
	}
	else if (delta.x > 0 && delta.y > 0 && delta.z > 0)
	{
		child = RTF;
	}
	else if (delta.x > 0 && delta.y > 0 && delta.z < 0)
	{
		child = RTB;
	}
	else if (delta.x < 0 && delta.y < 0 && delta.z > 0)
	{
		child = LBF;
	}
	else if (delta.x < 0 && delta.y < 0 && delta.z < 0)
	{
		child = LBB;
	}
	else if (delta.x > 0 && delta.y < 0 && delta.z > 0)
	{
		child = RBF;
	}
	else if (delta.x > 0 && delta.y < 0 && delta.z < 0)
	{
		child = RBB;
	}
	
	return child;
}

AABB* OctNode::getAABB()
{
	return m_bounds;
}