#pragma once
#include "Entity.h"
class CMage : public CEntity
{
public:
	CMage();
	~CMage();
	void RunFSM(double dt);

	void UpdateAttacking(void);

};
