#include "Zombie.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../PlayerInfo/PlayerInfo.h"


#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

Zombie::Zombie()
	: GenericEntity(NULL)
	, defaultPosition(Vector3(0.0f, 0.0f, 0.0f))
	, defaultTarget(Vector3(0.0f, 0.0f, 0.0f))
	, defaultUp(Vector3(0.0f, 0.0f, 0.0f))
	, target(Vector3(0.0f, 0.0f, 0.0f))
	, up(Vector3(0.0f, 0.0f, 0.0f))
	, maxBoundary(Vector3(0.0f, 0.0f, 0.0f))
	, minBoundary(Vector3(0.0f, 0.0f, 0.0f))
	, m_pTerrain(NULL)
	, m_iSeed(0)
	, got_leg(true)
	, got_head(true)
{
}


Zombie::~Zombie()
{
}

void Zombie::Init(void)
{
	// Set the default values
	defaultPosition.Set(0, 0, 10);
	defaultTarget.Set(0, 0, 0);
	defaultUp.Set(0, 1, 0);

	// Set the current values
	position.Set(10.0f, 0.0f, 0.0f);
	if (m_pTerrain)
		target = GenerateTarget();
	else
		target.Set(10.0f, 0.0f, 450.0f);
	up.Set(0.0f, 1.0f, 0.0f);

	// Set Boundary
	maxBoundary.Set(1, 1, 1);
	minBoundary.Set(-1, -1, -1);

	// Set speed
	m_dSpeed = 10.0;

	// Set Health
	m_health = 100;

	// Initialise the LOD meshes
	//InitLOD("ZombieTorsoH", "ZombieTorsoM", "ZombieTorsoL");

	//baseNode = CSceneGraph::GetInstance()->AddNode(this);
	//this->SetAABB(Vector3(1, 1, 1), Vector3(-1, -1, -1));

	//for (int i = 0; i < ZombieParts::TOTAL_PARTS; ++i)
	//{
	//	ZombieParts* zombiePart;
	//	CSceneNode* zombieNode;
	//	
	//	switch (i)
	//	{
	//	case ZombieParts::LEFT_ARM:
	//		zombiePart = new ZombieParts();
	//		zombiePart->SetType(i);
	//		zombiePart->Init();
	//		zombieNode = baseNode->AddChild(zombiePart);
	//		this->SetAABB(Vector3(100, 100, 100), Vector3(-100, -100, -100));
	//		break;
	//	case ZombieParts::LEFT_LEG:
	//		zombiePart = new ZombieParts();
	//		zombiePart->SetType(i);
	//		zombiePart->Init();
	//		zombieNode = baseNode->AddChild(zombiePart);
	//		this->SetAABB(Vector3(100, 100, 100), Vector3(-100, -100, -100));
	//		break;
	//	case ZombieParts::RIGHT_ARM:
	//		zombiePart = new ZombieParts();
	//		zombiePart->SetType(i);
	//		zombiePart->Init();
	//		zombieNode = baseNode->AddChild(zombiePart);
	//		this->SetAABB(Vector3(100, 100, 100), Vector3(-100, -100, -100));
	//		break;
	//	case ZombieParts::RIGHT_LEG:
	//		zombiePart = new ZombieParts();
	//		zombiePart->SetType(i);
	//		zombiePart->Init();
	//		zombieNode = baseNode->AddChild(zombiePart);
	//		this->SetAABB(Vector3(100, 100, 100), Vector3(-100, -100, -100));
	//		break;
	//	default:
	//		break;
	//	}

	//	zombiePart = NULL;
	//}
	//// Initialise the Collider
	//this->SetCollider(true);

	//// Add to EntityManager
	//EntityManager::GetInstance()->AddEntity(this, true);
}

void Zombie::Init(float x, float y)
{
	// Set the default values
	defaultPosition.Set(0, 0, 10);
	defaultTarget.Set(0, 0, 0);
	defaultUp.Set(0, 1, 0);

	// Set the current values
	position.Set(x, 10.0f, y);
	if (m_pTerrain)
		target = GenerateTarget();
	else
		target.Set(10.0f, 0.0f, 450.0f);
	up.Set(0.0f, 1.0f, 0.0f);

	// Set Boundary
	maxBoundary.Set(1, 1, 1);
	minBoundary.Set(-1, -1, -1);

	// Set speed
	m_dSpeed = 5.0;

	// Set Health
	m_health = 100;

	// Initialise the LOD meshes
	InitLOD("ZombieBodyH", "ZombieBodyM", "ZombieBodyL");

	baseNode = CSceneGraph::GetInstance()->AddNode(this);
	this->SetAABB(Vector3(2, 3, 2), Vector3(-2, -3, -2));

	Math::InitRNG();

	for (int i = 0; i < EntityZombiePart::TOTAL_PART; ++i)
	{
		//if (Math::RandIntMinMax(0, 100) < 50)
		//{
			EntityZombiePart* zombiePart;
			CSceneNode* zombieNode;

			zombiePart = new EntityZombiePart(i);
			zombiePart->Init();
			zombiePart->SetParent(this);
			EntityManager::GetInstance()->AddEntity(zombiePart, true);
			zombieNode = baseNode->AddChild(zombiePart);

			zombiePart = NULL;
	//	}
	}
	// Initialise the Collider
	this->SetCollider(true);

	// Add to EntityManager
	EntityManager::GetInstance()->AddEntity(this, true);
}

// Reset this player instance to default
void Zombie::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

// Set position
void Zombie::SetPos(const Vector3& pos)
{
	position = pos;
}

// Set target
void Zombie::SetTarget(const Vector3& target)
{
	this->target = target;
}
// Set Up
void Zombie::SetUp(const Vector3& up)
{
	this->up = up;
}
// Set the boundary for the player info
void Zombie::SetBoundary(Vector3 max, Vector3 min)
{
	maxBoundary = max;
	minBoundary = min;
}
// Set the terrain for the player info
void Zombie::SetTerrain(GroundEntity* m_pTerrain)
{
	if (m_pTerrain != NULL)
	{
		this->m_pTerrain = m_pTerrain;

		SetBoundary(this->m_pTerrain->GetMaxBoundary(), this->m_pTerrain->GetMinBoundary());
	}
}

// Get position
Vector3 Zombie::GetPos(void) const
{
	return position;
}

// Get target
Vector3 Zombie::GetTarget(void) const
{
	return target;
}
// Get Up
Vector3 Zombie::GetUp(void) const
{
	return up;
}
// Get the terrain for the player info
GroundEntity* Zombie::GetTerrain(void)
{
	return m_pTerrain;
}

// Update
void Zombie::Update(double dt)
{
	if (got_leg)
	{
		if (baseNode->GetChildrenList().size() >= 0)
		{
			vector<CSceneNode*> temp = baseNode->GetChildrenList();
			for (vector<CSceneNode*>::iterator it = temp.begin(); it != temp.end(); ++it)
			{
				if (dynamic_cast<EntityZombiePart*>((*it)->GetEntity())->GetType() == EntityZombiePart::RIGHT_LEG ||
					dynamic_cast<EntityZombiePart*>((*it)->GetEntity())->GetType() == EntityZombiePart::LEFT_LEG)
				{
					got_leg = true;
					break;
				}
				got_leg = false;
			}
		}
	}

	if (got_head)
	{
		if (baseNode->GetChildrenList().size() >= 0)
		{
			vector<CSceneNode*> temp = baseNode->GetChildrenList();
			for (vector<CSceneNode*>::iterator it = temp.begin(); it != temp.end(); ++it)
			{
				if (dynamic_cast<EntityZombiePart*>((*it)->GetEntity())->GetType() == EntityZombiePart::HEAD)
				{
					got_head = true;
					break;
				}
				got_head = false;
			}
		}
	}

	// Update the target
	if (m_health > 25 && got_head)
	{
		target = CPlayerInfo::GetInstance()->GetPos();
		target.y = position.y;
	}
	else
	{
		if ((position - target).Length() < 10)
		{
			target = GenerateTarget();
			target.y = position.y;
		}
	}

	try
	{
		Vector3 viewVector = (target - position).Normalized();

		position += viewVector * (float)m_dSpeed * (float)dt;
		
		Vector3 right = viewVector.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(viewVector).Normalized();

		Vector3 new_position;

		for (int i = 0; i < baseNode->GetNumOfChild(); i++)
		{
			EntityZombiePart *temp = dynamic_cast<EntityZombiePart*>(baseNode->GetChildrenList()[i]->GetEntity());
					
			if (temp->GetType() == EntityZombiePart::LEFT_ARM)
			{
				new_position = baseNode->GetEntity()->GetPosition() - (viewVector.Cross(up) * 3);
				new_position.y += 1;
				temp->SetPosition(new_position);
			}
			else if (temp->GetType() == EntityZombiePart::RIGHT_ARM)
			{
				new_position = baseNode->GetEntity()->GetPosition() + (viewVector.Cross(up) * 3);
				new_position.y += 1;
				temp->SetPosition(new_position);
			}
			else if (temp->GetType() == EntityZombiePart::LEFT_LEG)
			{
				new_position = baseNode->GetEntity()->GetPosition() - (viewVector.Cross(up) * 1.1);
				new_position.y -= 5;
				temp->SetPosition(new_position);
			}
			else if (temp->GetType() == EntityZombiePart::RIGHT_LEG)
			{
				new_position = baseNode->GetEntity()->GetPosition() + (viewVector.Cross(up) * 1.1);
				new_position.y -= 5;
				temp->SetPosition(new_position);
			}
			else if (temp->GetType() == EntityZombiePart::HEAD)
			{
				new_position = baseNode->GetEntity()->GetPosition() - (viewVector.Cross(up) * 0.05);
				new_position.y += 4.5f;
				temp->SetPosition(new_position);
			}
		}
	}
	catch (const DivideByZero)
	{

	}

	if (!got_leg)
	{
		if (m_dSpeed != 1)
			m_dSpeed = 1;
		if (position.y > -9)
		{
			position.y -= 0.7f;
			target.y = position.y;
		}
	}
	else
	{
		// Constrain the position
		Constrain();
	}

}

// Constrain the position within the borders
void Zombie::Constrain(void)
{
	// Constrain player within the boundary
	if (position.x > maxBoundary.x - 1.0f)
		position.x = maxBoundary.x - 1.0f;
	if (position.z > maxBoundary.z - 1.0f)
		position.z = maxBoundary.z - 1.0f;
	if (position.x < minBoundary.x + 1.0f)
		position.x = minBoundary.x + 1.0f;
	if (position.z < minBoundary.z + 1.0f)
		position.z = minBoundary.z + 1.0f;

	if (abs(((target.x - position.x) * (target.x - position.x) - (target.z - position.z)*(target.z - position.z))) < m_dSpeed)
	{
		target = GenerateTarget();
	}

	// if the y position is not equal to terrain height at that position, 
	// then update y position to the terrain height
	if (position.y != m_pTerrain->GetTerrainHeight(position))
		position.y = m_pTerrain->GetTerrainHeight(position);
}

// Render
void Zombie::Render(void)
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	Vector3 viewVector = (target - position).Normalized();
	float angle = Math::RadianToDegree(atan2(viewVector.x, viewVector.z));
	modelStack.Translate(position.x, position.y, position.z);
	if(got_leg)
		modelStack.Rotate(angle, 0, 1, 0);
	else
	{
		modelStack.Rotate(angle, 0, 1, 0);
		modelStack.Rotate(25, 1, 0, 0);
	}
	//std::cout << position.x << "," << position.y << "," << position.z << std::endl;
	modelStack.Scale(scale.x, scale.y, scale.z);
	if (GetLODStatus() == true)
	{
		if (theDetailLevel != NO_DETAILS)
		{
			//cout << theDetailLevel << endl;
			RenderHelper::RenderMesh(GetLODMesh());
		}
	}
	modelStack.PopMatrix();
}

// Generate New Target
Vector3 Zombie::GenerateTarget(void)
{
	return Vector3(rand() % (int)((maxBoundary.x - minBoundary.x)*0.5),
		0.0f,
		rand() % (int)((maxBoundary.x - minBoundary.x)*0.5));
}

void Zombie::SetHealth(const int newHealth)
{
	m_health = newHealth;
}

int Zombie::GetHealth()
{
	return m_health;
}

bool Zombie::GetGotLeg()
{
	return got_leg;
}

// Set random seed
void Zombie::SetRandomSeed(const int m_iSeed)
{
	this->m_iSeed = m_iSeed;
}

///////////////////////////////////////////////////////////////////////////////////ZOMBIE PARTS/////////////////////////////////////////////////////////////////////////////////////
float rotating = 0;
EntityZombiePart::EntityZombiePart(int part_input) : GenericEntity(NULL), part(static_cast<PARTS>(part_input))
{
	if(part == RIGHT_ARM || part == LEFT_ARM || part == HEAD)
		SetAABB(Vector3(1.5, 1.5, 1.5) * scale.x, Vector3(-1.5, -1.5, -1.5)* scale.x);
	else if (part == RIGHT_LEG || part == LEFT_LEG)
		SetAABB(Vector3(1.5, 3, 1.5) * scale.x, Vector3(-1.5, -3, -1.5)* scale.x);
}

EntityZombiePart::~EntityZombiePart()
{
	
}

void EntityZombiePart::Init()
{
	std::string name_of_texture_high = "ZombieHigh_" + std::to_string(part);
	std::string name_of_texture_mid = "ZombieMid_" + std::to_string(part);
	std::string name_of_texture_low = "ZombieLow_" + std::to_string(part);
	InitLOD(name_of_texture_high, name_of_texture_mid, name_of_texture_low);

	SetCollider(true);
}

void EntityZombiePart::SetType(int newType)
{
	part = static_cast<PARTS>(newType);
}

EntityZombiePart::PARTS EntityZombiePart::GetType()
{
	return part;
}

void EntityZombiePart::Render(void)
{
	rotating += 0.0015;
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	Zombie hello;
	Vector3 viewVector = (parent->GetTarget() - position).Normalized();
	float angle = Math::RadianToDegree(atan2(viewVector.x, viewVector.z));
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Rotate(angle, 0, 1, 0);

	if (part == RIGHT_ARM || part == LEFT_ARM)
	{
		modelStack.Rotate(cosf(rotating) * 3 * ((part == RIGHT_ARM) ? -1 : 1), 1, 1, 0);
		if(!parent->GetGotLeg())
			modelStack.Rotate(25, 1, 0, 0);
	}
	else if (part == RIGHT_LEG || part == LEFT_LEG)
		modelStack.Rotate(cosf(rotating) * 8 * ((part == RIGHT_LEG) ? -1: 1), 1, 0, 0);


	//std::cout << position.x << "," << position.y << "," << position.z << std::endl;
	modelStack.Scale(scale.x, scale.y, scale.z);
	if (GetLODStatus() == true)
	{
		if (theDetailLevel != NO_DETAILS)
		{
			//cout << theDetailLevel << endl;
			RenderHelper::RenderMesh(GetLODMesh());
		}
	}
	modelStack.PopMatrix();
}

void EntityZombiePart::SetParent(Zombie *parent_input)
{
	parent = parent_input;
}
