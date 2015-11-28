#include "Healer.h"


CHealer::CHealer()
{
	IsTarget = false;
	ClassName = "Healer: ";
	Position.Set(3, -3);
	m_MoveSpeed = 0.03f;
	m_AttackRange = 3.f;
	m_RunSpeed = m_MoveSpeed * 0.5f;
	ID = HEALER;
}


CHealer::~CHealer()
{
}

string CHealer::GetState(void)
{
	string DummyText;

	switch (state)
	{
	case CHealer::MOVE:
		DummyText = ClassName + "MOVE";
		return DummyText;
		break;
	case CHealer::HEAL:
		DummyText = ClassName + "HEAL";
		return DummyText;
		break;
	case CHealer::RETREAT:
		DummyText = ClassName + "RETREAT";
		return DummyText;
		break;
	default:
		DummyText = ClassName + "NULL";
		return DummyText;
		break;
	}
}

void CHealer::RunFSM(double dt)
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
void CHealer::UpdateAttacking(void)
{
	if (state == HEAL)
	{
	}

	else
	{
	}
}
