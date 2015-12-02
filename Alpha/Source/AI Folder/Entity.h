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

	void SetTargetPosition(Vector3 TargetPosition);
	void SetDangerPosition(Vector3 DangerPosition);
	void SetIsTarget(bool TF);
	void SetDangerZone(float DZ);
	void FaceTarget(void);
	void SetRotation(float Rotate);
	void SetCurrentHealthPoint(float);
	void SetID(string newID, string newTYPE, int ID_NO);
	bool TakingAction;
	float EntityRotation(double dt, float speed, float MaxRotate, float InputRotation);
	float EntityTranslation(double dt, float speed, float MaxTranslate, float InputTranslate);
	float GetCurrentHealthPoint(void);
	float GetHpPercent(void);
	float GetAttackRange(void);
	float GetRotation(void);
	int GetPriorityLevel(void);
	int GetIDNO(void);
	int GetTargetIDNO(void);
	Vector3 GetPosition();
	string GetID(void);
	string GetTYPE(void);
	Targets GetTargetID(void);

	void Move(Vector3 TargetDestination, double dt);
	void Retreat(Vector3 TargetDestination, double dt);

	void Attack(void);
	virtual void UpdateAttacking(CEntity*, double dt);
	virtual void CustomStates(double dt);

	virtual void RunFSM(double dt, Vector3 newTargetPosition = 0, Vector3 newDangerPosition = 0);
	virtual void RunFSM(double dt, vector<CEntity*> ListOfCharacters, Vector3 newTargetPosition = 0, Vector3 newDangerPosition = 0);

	virtual int Probability(int lowerLimit, int upperLimit);

	virtual void RandomSpawn(int lowerLimit, int upperLimit);

	virtual float GetChildRotation(int ChildID) = 0;
	virtual float GetChildTranslation(int ChildID) = 0;
	//Over ride bosses current target
	virtual float TargetOverRide(void) = 0;
	virtual void TickTimer(double dt) = 0;

	virtual bool DamageNullfiy(void) = 0;

	virtual string PrintState(void) = 0;
	virtual int GetState(void) = 0;

protected:
	int state;
	int m_HP;				// Max HP of this AI
	int m_Curent_HP;		// Current HP of this AI
	int m_Priority;			// Boss attack priority level
	int ID_NO;				// Number assigned to this unit
	int targetID_NO;		// Targets assigned number
	float m_Rotation;		// Rotation value of this AI
	float m_MoveSpeed;		// SPD of this AI
	float m_RunSpeed;		// Retreating SPD of this AI
	float m_Damage;			// ATK stat of this AI
	float m_AttackSpeed;	// AS stat of this AI
	float m_LastAttackTimer;// Time of last known attack
	float m_Cooldown;		// Skill cooldown of this AI
	float m_AttackRange;	// Attack Range of this AI
	float m_DangerZone;		// Caution zone of this AI
	float m_InitialRotation;// Where to reset the child node to etc Sword starting point

	bool IsTarget;

	Vector3 Position;
	Vector3 TargetPosition;
	Vector3 DangerPosition;

	string ClassName;
	string ID;
	
	Targets targetID;
	
	string TYPE;
};

