#pragma once
#include "Entity.h"

class CTank : public CEntity
{

	enum STATES
	{
		MOVE,
		ATTACK,
		TAUNT,
		RETREAT
	};

private:
	const static int TANK_LOOK_OFFSET = 90;
	const float SWORD_SWING_SPEED = 500.f;
	const float SHIELD_SWING_SPEED = 500.f;
	const float SWORD_SWING_ROT_AMOUNT = 90.f;
	const float SWORD_SWING_INIT_AMOUNT = 0.f;
	const float SWORD_TRANSLATE_AMOUNT = 3.f;
	const float SWORD_TRANSLATE_INIT_AMOUNT = 0.f;
	const float m_AttackDelay = 1.0f;
	const float m_SkillDelay = 15.0f;
	const float m_TauntDuration = 3.0f;

	bool m_SwordSwing;
	bool m_ShieldSwing;

	float m_SwordRotation;
	float m_ShieldRotation;

	float m_SwordTranslate;

public:
	CTank();
	~CTank();
	void RunFSM(double dt, vector<CEntity*> ListOfCharacters, Vector3 newTargetPosition = 0, Vector3 newDangerPosition = 0);

	void UpdateAttacking(double dt);
	bool TauntCheck(vector<CEntity*> ListOfCharacters);
	void UpdateTaunt(double dt);
	void TickTimer(double dt);
	void CustomStates(double dt);
	float TargetOverRide(void);

	float GetChildRotation(int ChildID);
	float GetChildTranslation(int ChildID);
	float GetSwordRotation(void);
	float GetShieldRotation(void);
};

