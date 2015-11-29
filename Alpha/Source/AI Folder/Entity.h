#pragma once
#include <string>
#include <math.h>  
#include <iostream>
#include "MyMath.h"
#include "Vector3.h"
#include <time.h>
#include <stdlib.h>
#include <vector>

using namespace std;

#define MAYA_MODEL_OFFSET 90

class CEntity
{
protected:
	int state;
	int m_HP;				// Max HP of this AI
	int m_Curent_HP;		// Current HP of this AI
	int m_Priority;
	float m_Rotation;		// Rotation value of this AI
	float m_MoveSpeed;		// SPD of this AI
	float m_RunSpeed;		// Retreating SPD of this AI
	float m_Damage;			// ATK stat of this AI
	float m_AttackSpeed;	// AS stat of this AI
	float m_LastAttackTimer;//	Time of last known attack
	float m_Cooldown;		// Skill cooldown of this AI
	float m_AttackRange;	// Attack Range of this AI
	float m_DangerZone;		// Caution zone of this AI

	bool IsTarget;

	Vector3 Position;
	Vector3 TargetPosition;
	Vector3 DangerPosition;	

	string ClassName;
	string ID;
	string TYPE;

public:
	CEntity();
	~CEntity();

	void SetTargetPosition(Vector3 TargetPosition);
	void SetDangerPosition(Vector3 DangerPosition);
	void SetIsTarget(bool TF);
	void SetDangerZone(float DZ);
	void FaceTarget(void);
	void SetCurrentHealthPoint(float);
	float GetCurrentHealthPoint(void);
	void SetID(string newID, string newTYPE);
	float GetHpPercent(void);
	float GetAttackRange(void);
	float GetRotation(void);
	Vector3 GetPosition();
	string GetID(void);
	string GetTYPE(void);

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

	void Move(Vector3 TargetDestination, double dt);
	void Retreat(Vector3 TargetDestination, double dt);

	void Attack(void);
	virtual void UpdateAttacking(void);

	virtual void RunFSM(double dt, Vector3 newTargetPosition = 0, Vector3 newDangerPosition = 0);
	virtual void RunFSM(double dt, vector<CEntity*> ListOfCharacters, Vector3 newTargetPosition = 0, Vector3 newDangerPosition = 0);

	virtual int Probability(int lowerLimit, int upperLimit);

	virtual void RandomSpawn(int lowerLimit, int upperLimit);

	virtual string GetState(void);
};

