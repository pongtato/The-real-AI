#include "Tank.h"


CTank::CTank()
{
	IsTarget = false;
	ClassName = "Tank: ";
	Position.Set(40.f, 0.f, 0.f);
	m_MoveSpeed = 20.f;
	m_AttackRange = 15.0f;
	m_RunSpeed = m_MoveSpeed * 0.5f;
	ID = TANK;

	m_HP = 100;
	//Testing
	m_Curent_HP = 50;
}


CTank::~CTank()
{
}

void CTank::RunFSM(double dt, Vector3 newTargetPosition, Vector3 newDangerPosition)
{
	TargetPosition = newTargetPosition;
	DangerPosition = newDangerPosition;

	if (IsTarget)
	{
		state = RETREAT;
	}

	switch (state)
	{
	case MOVE:
		if (m_AttackRange < (TargetPosition - Position).Length())
		{
			Move(TargetPosition, dt);
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
			Retreat(DangerPosition, dt);
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
