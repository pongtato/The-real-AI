#pragma once
#include <string>
#include <math.h>  
#include <iostream>
#include "MyMath.h"
#include "Vector3.h"
#include <time.h>
#include <stdlib.h>

using namespace std;

class CEntity
{
public:
	CEntity();
	~CEntity();

	enum STATES
	{
		MOVE,
		ATTACK,
		RETREAT
	};

	enum Targets
	{
		HEALER,
		MAGE,
		TANK,
		BOSS,
	};

	int state;
	int m_HP;
	int m_Priority;
	float m_MoveSpeed;
	float m_RunSpeed;
	float m_Damage;
	float m_AttackSpeed;
	float m_Cooldown;
	float m_AttackRange;
	float m_DangerZone;

	Vector3 Position;
	Vector3 TargetPosition;
	Vector3 DangerPosition;

	void Move(Vector3 TargetDestination);
	void Retreat(Vector3 TargetDestination);

	void Attack(void);
	virtual void UpdateAttacking(void);

	virtual void RunFSM(double dt);

	virtual int Probability(int lowerLimit, int upperLimit);

	virtual string GetState(void);
	string ClassName;

	bool IsTarget;

	Targets ID;
};

