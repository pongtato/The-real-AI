#pragma once
#include "Entity.h"
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
	void RunFSM(double dt);
	void UpdateAttacking(void);
};

