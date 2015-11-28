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

public:
	CHealer();
	~CHealer();

	string GetState(void);
	void RunFSM(double dt, vector<CEntity*> ListOfCharacters, Vector3 newTargetPosition = 0, Vector3 newDangerPosition = 0);
	void UpdateAttacking(void);
};

