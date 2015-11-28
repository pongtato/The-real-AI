#pragma once
#include "Entity.h"

class CTank : public CEntity
{
	
public:
	CTank();
	~CTank();
	void RunFSM(double dt, Vector3 newTargetPosition = 0, Vector3 newDangerPosition = 0);

	void UpdateAttacking(void);
	
	const static int TANK_LOOK_OFFSET = 90;
};

