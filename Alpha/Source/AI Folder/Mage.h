#pragma once
#include "Entity.h"
class CMage : public CEntity
{
public:
	CMage();
	~CMage();
	void RunFSM(double dt, Vector3 newTargetPosition, Vector3 newDangerPosition);

	void UpdateAttacking(void);

};
