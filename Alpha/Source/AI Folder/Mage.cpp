#include "Mage.h"


CMage::CMage()
{
	IsTarget = false;
	ClassName = "Mage: ";
	Position.Set(40.f, 0.f, -20.f);
	m_MoveSpeed = 0.02f;
	m_AttackRange = 3.f;
	m_RunSpeed = m_MoveSpeed * 0.5f;
	ID = MAGE;
}


CMage::~CMage()
{
}

void CMage::RunFSM(double dt)
{
	if (m_DangerZone > (Position - DangerPosition).Length())
	{
		state = RETREAT;
	}

	switch (state)
	{
	case MOVE:
		if (m_AttackRange < (TargetPosition - Position).Length())
		{
			Move(TargetPosition);
		}
		else
		{
			state = ATTACK;
		}
		break;
	case ATTACK:
		if (m_AttackRange >= (TargetPosition - Position).Length())
		{
			//Do attack 
		}
		else
		{
			state = MOVE;
		}
		break;
	case RETREAT:
		if (m_DangerZone > (Position - DangerPosition).Length())
		{
			Retreat(DangerPosition);
		}
		else
		{
			state = ATTACK;
		}
		break;
	default:
		break;
	}
	UpdateAttacking();
}
void CMage::UpdateAttacking(void)
{
	if (state == ATTACK)
	{
	}
	else
	{
	}
}