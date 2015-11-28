#pragma once
#include "Entity.h"
#include  <vector>

class CBoss : public CEntity
{

public:
	CBoss();
	~CBoss();

	enum STATES
	{
		MOVE,
		HEAL,
		RETREAT,
		RESET
	};

	const float TargetChangeDelay;	
	bool IsTaunted;	
	int CurrentTarget;
	float TargetAcquireRange;
	float TargetChangeTimer;
	float ResetRange;
	Vector3 InitialPos;
	vector<CEntity*> TargetList;

	string GetState(void);
	void UpdateAttacking(void);
	void TickTimer(double dt);
	void RunFSM(double dt, vector<CEntity*> ListOfEnemies);	
	int GetCurrentTarget(void);
};

