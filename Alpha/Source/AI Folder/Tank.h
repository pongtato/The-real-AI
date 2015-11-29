#pragma once
#include "Entity.h"

class CTank : public CEntity
{
private:
	const static int TANK_LOOK_OFFSET = 90;
	const float SWORD_SWING_SPEED = 500.f;
	const float SHIELD_SWING_SPEED = 500.f;
	const float SWORD_SWING_ROT_AMOUNT = 90.f;
	const float SWORD_SWING_INIT_AMOUNT = 0.f;
	const float m_AttackDelay = 1.0f;

	bool m_SwordSwing;
	bool m_ShieldSwing;

	float m_SwordRotation;
	float m_ShieldRotation;

public:
	CTank();
	~CTank();
	void RunFSM(double dt, Vector3 newTargetPosition = 0, Vector3 newDangerPosition = 0);

	void UpdateAttacking(double dt);
	void TickTimer(double dt);

	float GetChildRotation(int ChildID);
	float GetSwordRotation(void);
	float GetShieldRotation(void);
};

