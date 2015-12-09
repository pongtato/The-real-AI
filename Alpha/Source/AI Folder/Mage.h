#pragma once
#include "Entity.h"
#include "Particle.h"
#include "Boss.h"
#include "../ResourceManager.h"
class CMage : public CEntity
{
	enum STATES
	{
		MOVE,
		ATTACK,
		RETREAT,
		CAST_SKILL
	};

private:
	const static int MAGE_LOOK_OFFSET = 90;
	const float STAFF_SWING_SPEED = 1000.f;
	const float STAFF_SWING_ROT_AMOUNT = 90.f;
	const float STAFF_SWING_INIT_AMOUNT = 0.f;
	const float m_AttackDelay = 1.0f;
	const static int DEFAULT_CASTING_TIME = 2;
	const static int DEFAULT_COOLDOWN = 10;
	const static int DEFAULT_SKILL_DAMAGE = 20;

	bool m_StaffSwing;
	bool m_IsCastingSkill;
	float m_CastingTimer;
	float m_StaffRotation;

public:
	CMage();
	~CMage();
	void RunFSM(double dt, vector<CEntity*> ListOfEnemies, vector<CParticle*>&ListOfParticles, ResourceManager &manager, Vector3 newTargetPosition = 0, Vector3 newDangerPosition = 0);
	void UpdateAttacking(CEntity*,vector<CParticle*>&,ResourceManager, double dt);
	void CustomStates(double dt);
	float GetChildRotation(int ChildID);
	float GetChildTranslation(int ChildID);
	float TargetOverRide(void);
	void TickTimer(double dt);
	int GetState(void);
	bool DamageNullfiy(void);
	bool GetCastingSkillBool(void);
	string PrintState(void);
};
