#pragma once
#include "Entity.h"
#include  <vector>

#define BOSS "BOSS"

class CBoss : public CEntity
{

public:
	CBoss();
	~CBoss();

	static const int DEFAULT_COOLDOWN = 20;
	static const int DEFAULT_CASTING_TIME = 1;
	static const int DEFAULT_SKILL_RANGE = 10;
	static const int DEFAULT_SKILL_DAMAGE = 20;
	static const int DEFAULT_DAMAGE = 10;

	const static int BOSS_LOOK_OFFSET = 90;
	const float ARM_SWING_SPEED = 1000.f;
	const float ARM_SWING_ROT_AMOUNT = -90.f;
	const float ARM_SWING_INIT_AMOUNT = 0.f;
	const float m_AttackDelay = 1.0f;

	bool m_ArmSwing;

	float m_ArmRotation;

	float m_TauntTimer;

	enum STATES
	{
		MOVE,
		HEAL,
		RETREAT,
		CAST_SKILL,
		RESET
	};

	string PrintState(void);
	void UpdateAttacking(CEntity*, double dt);
	void TickTimer(double dt);
	void RunFSM(double dt, vector<CEntity*> ListOfEnemies, Vector3 newTargetPosition = 0, Vector3 newDangerPosition = 0);
	void CustomStates(double dt);
	int GetCurrentTarget(void);
	float GetChildRotation(int ChildID);
	float GetChildTranslation(int ChildID);
	float TargetOverRide(void);
	void TargetPriorityCheck(vector<CEntity*> ListOfEnemies);
	void ChooseTarget(vector<CEntity*> ListOfEnemies);

private:

	const float TargetChangeDelay;	
	bool IsTaunted;
	bool m_IsCastingSkill;	// Boolean to check if boss is casting skill
	float m_CastingTimer;	// Timer to keep track how long its cast time is
	int CurrentTarget;
	float TargetAcquireRange;
	float TargetChangeTimer;
	float ResetRange;
	float m_DamageThreshold;
	float m_TotalDamageTaken;
	Vector3 InitialPos;
	vector<CEntity*> TargetList;
	Targets CurrentTargetType;
	int GetState(void);
	bool DamageNullfiy(void);
};

