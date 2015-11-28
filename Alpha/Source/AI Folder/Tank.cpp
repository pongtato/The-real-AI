#include "Tank.h"


CTank::CTank()
{
	IsTarget = false;
	ClassName = "Tank: ";
	Position.Set(40.f, 0.0f, 10.f);
	m_MoveSpeed = 0.02f;
	m_AttackRange = 4.0f;
	m_RunSpeed = m_MoveSpeed * 0.5f;
	ID = TANK;
}


CTank::~CTank()
{
}

void CTank::RunFSM(double dt)
{
	if (IsTarget)
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
void CTank::UpdateAttacking(void)
{
	if (state == ATTACK)
	{
	}
	else
	{
	}
}
