#pragma once
#include "Entity.h"
class CMage : public CEntity
{
public:
	CMage();
	~CMage();
	void RunFSM(double dt, Vector3 newTargetPosition = 0, Vector3 newDangerPosition = 0);

	void UpdateAttacking(void);

};
