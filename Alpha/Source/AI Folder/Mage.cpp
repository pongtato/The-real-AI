#include "Mage.h"


CMage::CMage()
{
	IsTarget = false;
	ClassName = "Mage: ";
	Position.Set(40.f, 0.f, -20.f);
	m_MoveSpeed = 20.f;
	m_AttackRange = 45.f;
	m_RunSpeed = m_MoveSpeed * 0.5f;
	ID = MAGE;

	m_HP = 100;
	m_Curent_HP = m_HP;
}


CMage::~CMage()
{
}

void CMage::RunFSM(double dt, Vector3 newTargetPosition, Vector3 newDangerPosition)
{
	TargetPosition = newTargetPosition;
	DangerPosition = newDangerPosition;

	if (m_DangerZone > (Position - DangerPosition).Length())
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
void CMage::UpdateAttacking(void)
{
	if (state == ATTACK)
	{
	}
	else
	{
	}
}