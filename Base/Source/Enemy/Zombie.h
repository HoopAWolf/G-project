#pragma once
#include "../GenericEntity.h"
#include "../GroundEntity.h"
#include "../SceneGraph/SceneGraph.h"

class Mesh;

class Zombie :
	public GenericEntity
{
protected:
	Vector3 defaultPosition, defaultTarget, defaultUp;
	Vector3 target, up;
	Vector3 maxBoundary, minBoundary;
	GroundEntity* m_pTerrain;

	double m_dSpeed;
	double m_dAcceleration;

	int m_iSeed;

	int m_health;
	CSceneNode* baseNode;
	bool got_leg, got_head;
public:
	Zombie(void);
	virtual ~Zombie();

	void Init(void);
	void Init(float x, float y);
	// Reset this player instance to default
	void Reset(void);

	// Set position
	void SetPos(const Vector3& pos);
	// Set target
	void SetTarget(const Vector3& target);
	// Set Up
	void SetUp(const Vector3& up);
	// Set the boundary for the player info
	void SetBoundary(Vector3 max, Vector3 min);
	// Set the terrain for the player info
	void SetTerrain(GroundEntity* m_pTerrain);

	// Get position
	Vector3 GetPos(void) const;
	// Get target
	Vector3 GetTarget(void) const;
	// Get Up
	Vector3 GetUp(void) const;
	// Get the terrain for the player info
	GroundEntity* GetTerrain(void);

	// Update
	void Update(double dt = 0.0333f);

	// Constrain the position within the borders
	void Constrain(void);
	// Render
	void Render(void);
	// Set random seed
	void SetRandomSeed(const int m_iSeed);
	// Generate New Target
	Vector3 GenerateTarget(void);

	void SetHealth(const int newHealth);
	int GetHealth();
	bool GetGotLeg();
};

class EntityZombiePart : public GenericEntity
{

public:

	enum PARTS
	{
		HEAD = 0,
		RIGHT_ARM,
		LEFT_ARM,
		RIGHT_LEG,
		LEFT_LEG,
		TOTAL_PART
	};

	EntityZombiePart(int);

	~EntityZombiePart();

	void Init();

	void SetType(int newType);
	PARTS GetType();

	void Render(void);
	void SetParent(Zombie *);

private:
	PARTS part;
	Zombie *parent;
	Vector3 target;
};