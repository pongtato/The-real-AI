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

	enum STATES
	{
		MOVE,
		HEAL,
		RETREAT,
		CAST_SKILL,
		RESET
	};

	string GetState(void);
	void UpdateAttacking(CEntity*);
	void TickTimer(double dt);
	void RunFSM(double dt, vector<CEntity*> ListOfEnemies, Vector3 newTargetPosition = 0, Vector3 newDangerPosition = 0);
	int GetCurrentTarget(void);

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

	
};

