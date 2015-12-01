#pragma once
#include "Entity.h"
#include "Boss.h"
class CMage : public CEntity
{
private:
	const static int MAGE_LOOK_OFFSET = 90;
	const float STAFF_SWING_SPEED = 1000.f;
	const float STAFF_SWING_ROT_AMOUNT = 90.f;
	const float STAFF_SWING_INIT_AMOUNT = 0.f;
	const float m_AttackDelay = 1.0f;

	bool m_StaffSwing;

	float m_StaffRotation;

public:
	CMage();
	~CMage();
	void RunFSM(double dt, vector<CEntity*> ListOfEnemies, Vector3 newTargetPosition = 0, Vector3 newDangerPosition = 0);
	void UpdateAttacking(CEntity*, double dt);
	void CustomStates(double dt);
	float GetChildRotation(int ChildID);
	float GetChildTranslation(int ChildID);
	float TargetOverRide(void);
	void TickTimer(double dt);
	int GetState(void);
	bool DamageNullfiy(void);
};
