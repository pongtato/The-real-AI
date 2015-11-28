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
	void RunFSM(double dt, vector<CEntity*> ListOfCharacters, Vector3 newDangerPosition);
	void UpdateAttacking(void);
};

