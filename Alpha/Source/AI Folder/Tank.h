#pragma once
#include "Entity.h"

class CTank : public CEntity
{
	
public:
	CTank();
	~CTank();
	void RunFSM(double dt, Vector3 newTargetPosition, Vector3 newDangerPosition);

	void UpdateAttacking(void);
};

