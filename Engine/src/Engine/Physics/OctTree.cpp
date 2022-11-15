#include "OctTree.h"

using namespace Engine;

#pragma region OctNode Code
const int OctNode::REBALANCE_FACTOR = 8;

OctNode::OctNode(glm::vec3& center, glm::vec3& dimensions, OctNode* parent)
	:m_bounds(new AABB(dimensions, center)), m_parent(parent)
{
}

OctNode::~OctNode()
{
	for (int i = LTF; i <= RBB; ++i)
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
	//TODO: implement checking for appropriate entity storage (no oct tree violations)
	/*if (m_children)
	{
		for (int i = LTF; i <= RBB; ++i)
		{

		}
	}*/
}

std::list<Entity*>& OctNode::raycast(Ray& ray)
{
	//TODO: raycasting code
	// Can probably be made more efficent by casting ray against child's bounding box
	std::list<Entity*> picks;
	
	// Check self
	for (auto ent : m_entityList)
	{
		if (ent.second->boundingBox->intersect(ray))
		{
			picks.push_back(ent.first);
		}
	}
	// Check children
	for (int i = LTF; m_children && i <= RBB; ++i)
	{
		for (auto pick : m_children[i]->raycast(ray))
		{
			picks.push_back(pick);
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
		//TODO: refine check using math and refactor
		bool found = false;
		for (int i = 0; i < 8 && !found; ++i)
		{
			// Check if child contains the component
			if (m_children[i]->getAABB()->containsBox(bounds))
			{
				found = true;
				inserted = m_children[i]->insert(entity, component);
			}
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
	//TODO: removal code
	bool removed = false;
	// Check self
	auto it = m_entityList.begin();
	auto check = std::make_pair(entity, component);;
	for (bool found = false;!found && it != m_entityList.end(); ++it)
	{
		found = *it == check;
	}
	if (it != m_entityList.end())
	{
		removed = false;
		m_entityList.remove(*it);
	}
	// Check children
	else
	{
		for (int i = LTF; i <= RBB; ++i)
		{
			removed = m_children[i]->remove(entity, component);
		}
	}

	return removed;
}

void OctNode::subdivide()
{
	//TODO: subdivision code
	if (!m_children)
	{
		glm::vec3 center = m_bounds->getPosition();
		// Calculate dimensions of children
		glm::vec3 dimensions = m_bounds->getDimensions() / 2.0f;
		// Calculate positions of children
		std::vector<glm::vec3> centers(8);
		// Left-side
		centers[LTF].x = centers[LTB].x = centers[LBF].x = centers[LBB].x =  center.x - dimensions.x;
		// Right-side
		centers[RTF].x = centers[RTB].x = centers[RBF].x = centers[RBB].x = center.x + dimensions.x;
		// Top-side
		centers[LTF].y = centers[LTB].y = centers[RTF].y = centers[RTB].y = center.y + dimensions.y;
		// Bottom-side
		centers[LBF].y = centers[LBB].y = centers[RBF].y = centers[RBB].y = center.y - dimensions.y;
		// Front-side
		centers[LTF].z = centers[RTF].z = centers[LBF].z = centers[RBF].z = center.z + dimensions.z;
		// Back-side
		centers[LTB].z = centers[RTB].z = centers[LBB].z = centers[RBB].z = center.z - dimensions.z;

		m_children = new OctNode*[8];
		for (int i = LTF; i <= RBB; ++i)
		{
			m_children[i] = new OctNode(dimensions, centers[i]);
		}
	}
}

void OctNode::rebalance()
{
	//TODO: rebalance code
	if (m_children)
	{
		PhysicsComponent* component;
		AABB* bounds;
		glm::vec3 maxChildSize = m_bounds->getDimensions() / 2.0f;
		glm::vec3 compSize;
		std::list<std::pair<Entity*, PhysicsComponent*>> removal;
		// Go through entities and see if we can push entities down tree
		bool moved;
		for (auto ent : m_entityList)
		{
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
			// Push to children
			else if (compSize.x < maxChildSize.x &&
					 compSize.y < maxChildSize.y &&
					 compSize.z < maxChildSize.z)
			{
				// More child checking code to refactor
				for (int i = LTF; i <= RBB; i++)
				{
					if (m_children[i]->getAABB()->containsBox(bounds))
					{
						if (m_children[i]->insert(ent.first, ent.second))
						{
							removal.push_back(ent);
						}
					}
				}
			}
		}
		// Remove inserted entities
		for (auto pair : removal)
		{
			remove(pair.first, pair.second);
		}
	}
}

AABB* OctNode::getAABB()
{
	return m_bounds;
}
#pragma endregion

#pragma region OctTree Code
std::list<Entity*>& OctTree::raycast(Ray& ray)
{
	return m_root->raycast(ray);
}

std::list<Entity*>& OctTree::raycast(glm::vec3& origin, glm::vec3& vector)
{
	Ray ray(origin, vector);
	return m_root->raycast(ray);
}

bool OctTree::insert(Entity* entity)
{
	//TODO: insertion code
	bool inserted = false;
	PhysicsComponent* component = nullptr;
	if (entity->hasComponent<PhysicsComponent>())
	{
		component = &entity->getComponent<PhysicsComponent>();
		inserted = m_root->insert(entity, component);
	}

	return inserted;
}

bool OctTree::remove(Entity* entity)
{
	//TODO: removal code
	bool removed = false;
	PhysicsComponent* component = nullptr;
	if (entity->hasComponent<PhysicsComponent>())
	{
		component = &entity->getComponent<PhysicsComponent>();
		removed = m_root->remove(entity, component);
	}

	return removed;
}
#pragma endregion