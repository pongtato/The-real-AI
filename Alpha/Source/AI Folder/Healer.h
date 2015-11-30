#pragma once
#include "Entity.h"
#include  <vector>

class CHealer : public CEntity
{
	enum STATES
	{
		MOVE,
		HEAL,
		RETREAT
	};
private:
	const static int HEALER_LOOK_OFFSET = 90;
	const float ROD_SWING_SPEED = 1000.f;
	const float ROD_SWING_ROT_AMOUNT = 90.f;
	const float ROD_SWING_INIT_AMOUNT = 0.f;
	const float m_AttackDelay = 1.0f;

	bool m_RodSwing;
	
	float m_RodRotation;

public:
	CHealer();
	~CHealer();

	string GetState(void);
	void RunFSM(double dt, vector<CEntity*> ListOfCharacters, Vector3 newTargetPosition = 0, Vector3 newDangerPosition = 0);
	void UpdateAttacking(double dt);
	float GetChildRotation(int ChildID);
	float GetChildTranslation(int ChildID);
	void CustomStates(double dt);
	float TargetOverRide(void);
	void TickTimer(double dt);
};

