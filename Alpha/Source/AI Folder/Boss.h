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

	float TargetChangeTimer;
	const float TargetChangeDelay;
	float TargetAcquireRange;
	bool IsTaunted;
	const int TargetChangeProbability;
	vector<CEntity*> TargetList;

	int ChooseTarget(int RNG);
	void TickTimer(void);
	Targets target;


	Vector3 InitialPos;
	float ResetRange;

	void RunFSM(double dt);
	string GetState(void);

	void UpdateAttacking(void);
};

