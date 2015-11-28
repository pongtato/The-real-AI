#pragma once
#include "Entity.h"

class CTank : public CEntity
{
	
public:
	CTank();
	~CTank();
	void RunFSM(double dt);

	void UpdateAttacking(void);
};

