#include "OctNode.h"
#include "Engine/SceneBuilder/Components.h"
#include "Engine/SceneBuilder/Entity.h"
#include "Engine/Utilities/Log.h"

using namespace Engine;

const int OctNode::REBALANCE_FACTOR = 8;

OctNode::OctNode(glm::vec3& dimensions, glm::vec3& center, OctNode* parent)
	:m_bounds(new AABB(dimensions, center)), m_parent(parent), m_children(nullptr)
{
	glm::vec3 dim = m_bounds->getDimensions();
	glm::vec3 pos = m_bounds->getPosition();
	GE_CORE_TRACE("node d: {0} {1} {2}, p: {3} {4} {5} OctNode::OctNode: Node created", dim.x, dim.y, dim.z, pos.x, pos.y, pos.z);
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
	if (m_children)
	{
		for (int i = LTF; i <= RBB; ++i)
		{
			m_children[i]->update();
		}
	}
	rebalance();
}

std::list<Entity*> OctNode::raycast(Ray& ray)
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
	//TODO: refactor to check only relevant children
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
	std::string tag = entity->getComponent<TagComponent>().tag;
	glm::vec3 dim = m_bounds->getDimensions();
	glm::vec3 pos = m_bounds->getPosition();
	GE_CORE_TRACE("node d: {1} {2} {3}, p: {4} {5} {6} OctNode::insert: Attempting insertion of {0}", tag, dim.x, dim.y, dim.z, pos.x, pos.y, pos.z);
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
		GE_CORE_TRACE("node d: {0} {1} {2}, p: {3} {4} {5} OctNode::insert: Rebalance triggered", dim.x, dim.y, dim.z, pos.x, pos.y, pos.z);
		subdivide();
		rebalance();
	}
	// Check children
	if (isChildSized && m_children)
	{
		GE_CORE_TRACE("node d: {0} {1} {2}, p: {3} {4} {5} OctNode::insert: Attempting children", dim.x, dim.y, dim.z, pos.x, pos.y, pos.z);
		//TODO: refine check using math and refactor
		int child = getChild(bounds->getPosition());
		if (child > -1 && m_children[child]->getAABB()->containsBox(bounds))
		{
			//
			GE_CORE_TRACE("node d: {0} {1} {2}, p: {3} {4} {5} OctNode::insert: Child found", dim.x, dim.y, dim.z, pos.x, pos.y, pos.z);
			inserted = m_children[child]->insert(entity, component);
		}
		//bool found = false;
		//for (int i = 0; i < 8 && !found; ++i)
		//{
		//	// Check if child contains the component
		//	if (m_children[i]->getAABB()->containsBox(bounds))
		//	{
		//		GE_CORE_TRACE("node d: {0} {1} {2}, p: {3} {4} {5} OctNode::insert: Child found {6}", dim.x, dim.y, dim.z, pos.x, pos.y, pos.z, i);
		//		found = true;
		//		inserted = m_children[i]->insert(entity, component);
		//	}
		//}
	}
	// Insert into self if item is within node
	if (!inserted && m_bounds->containsBox(bounds))
	{
		GE_CORE_TRACE("node d: {1} {2} {3}, p: {4} {5} {6} OctNode::insert: Inserted {0}", tag, dim.x, dim.y, dim.z, pos.x, pos.y, pos.z);
		inserted = true;
		m_entityList.push_back(std::make_pair(entity, component));
	}
	return inserted;
}

bool OctNode::remove(Entity* entity, PhysicsComponent* component)
{
	//TODO: removal code
	bool removed = false;

	return removed;
}

void OctNode::subdivide()
{
	//TODO: subdivision code
	if (!m_children)
	{
		glm::vec3 dim = m_bounds->getDimensions();
		glm::vec3 pos = m_bounds->getPosition();
		GE_CORE_TRACE("node d: {0} {1} {2}, p: {3} {4} {5} OctNode::subdivide: Dividing", dim.x, dim.y, dim.z, pos.x, pos.y, pos.z);
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
			m_children[i] = new OctNode(dimensions, centers[i]);
		}
	}
}

void OctNode::rebalance()
{
	//TODO: rebalance code
	if (m_children)
	{
		glm::vec3 dim = m_bounds->getDimensions();
		glm::vec3 pos = m_bounds->getPosition();
		GE_CORE_TRACE("node d: {0} {1} {2}, p: {3} {4} {5} OctNode::rebalance: Rebalancing", dim.x, dim.y, dim.z, pos.x, pos.y, pos.z);
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
				GE_CORE_TRACE("node d: {1} {2} {3}, p: {4} {5} {6} OctNode::rebalance: Inserting {0} into parent", tag, dim.x, dim.y, dim.z, pos.x, pos.y, pos.z);
				m_parent->insert(ent.first, ent.second);
				// Whether inserted or not, cull entity from tree
				removal.push_back(ent);
			}
			// Push to children
			else if (compSize.x < maxChildSize.x &&
					compSize.y < maxChildSize.y &&
					compSize.z < maxChildSize.z)
			{
				GE_CORE_TRACE("node d: {1} {2} {3}, p: {4} {5} {6} OctNode::rebalance: Attempting insertion of {0} into child", tag, dim.x, dim.y, dim.z, pos.x, pos.y, pos.z);
				int child = getChild(bounds->getPosition());
				if (child > -1 && m_children[child]->getAABB()->containsBox(bounds))
				{
					//
					GE_CORE_TRACE("node d: {1} {2} {3}, p: {4} {5} {6} OctNode::rebalance: Inserting {0} into child {7}", tag, dim.x, dim.y, dim.z, pos.x, pos.y, pos.z, child);
					if (m_children[child]->insert(ent.first, ent.second))
					{
						removal.push_back(ent);
					}
				}
				// More child checking code to refactor
				//for (int i = LTF; i <= RBB; i++)
				//{
				//	//
				//	if (m_children[i]->getAABB()->containsBox(bounds))
				//	{
				//		GE_CORE_TRACE("node d: {1} {2} {3}, p: {4} {5} {6} OctNode::rebalance: Inserting {0} into child", tag, dim.x, dim.y, dim.z, pos.x, pos.y, pos.z);
				//		if (m_children[i]->insert(ent.first, ent.second))
				//		{
				//			removal.push_back(ent);
				//		}
				//	}
				//}
			}
		}
		// Remove inserted entities
		for (auto pair : removal)
		{
			m_entityList.remove(pair);
			//remove(pair.first, pair.second);
		}
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