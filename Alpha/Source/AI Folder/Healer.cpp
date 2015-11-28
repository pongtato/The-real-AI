#include "Healer.h"


CHealer::CHealer()
{
	IsTarget = false;
	ClassName = "Healer: ";
	Position.Set(10.f,0.f,20.f);
	m_MoveSpeed = 30.f;
	m_AttackRange = 45.f;
	m_RunSpeed = m_MoveSpeed * 0.5f;
	ID = HEALER;

	m_HP = 100;
	m_Curent_HP = m_HP;
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

void CHealer::RunFSM(double dt, vector<CEntity*> ListOfCharacters, Vector3 newDangerPosition)
{
	float lowestHP = 0;

	DangerPosition = newDangerPosition;

	//Go through list for lowest hp
	for (int i = 0; i < ListOfCharacters.size(); ++i)
	{
		if (ListOfCharacters[i]->GetHpPercent() > lowestHP)
		{
			lowestHP = ListOfCharacters[i]->GetHpPercent();
			TargetPosition = ListOfCharacters[i]->GetPosition();
		}
	}

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
void CHealer::UpdateAttacking(void)
{
	if (state == HEAL)
	{
	}

	else
	{
	}
}
