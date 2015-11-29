#include "Healer.h"


CHealer::CHealer()
{
	IsTarget = false;
	ClassName = "Healer: ";
	m_MoveSpeed = 30.f;
	m_RunSpeed = m_MoveSpeed * 0.5f;
	ID = HEALER;

	m_HP = 100;
	m_Curent_HP = m_HP;

	Position.Set(
		Probability(0, 100),
		0.1f,
		Probability(0, 100));

	m_RodRotation = 0.0f;
	m_RodSwing = false;

	//Attack Init
	m_AttackRange = 45.f;
	m_AttackSpeed = 1.0f;
	m_LastAttackTimer = m_AttackDelay;

	TakingAction = false;
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

void CHealer::RunFSM(double dt, vector<CEntity*> ListOfCharacters, Vector3 newTargetPosition, Vector3 newDangerPosition)
{
	float lowestHP = 0;

	DangerPosition = newDangerPosition;
	//Face the targets position
	FaceTarget();
	TickTimer(dt);

	//Go through list for lowest hp
	for (int i = 0; i < ListOfCharacters.size(); ++i)
	{
		if (ListOfCharacters[i]->GetHpPercent() > lowestHP && ListOfCharacters[i]->GetTYPE() != "BOSS")
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
			TakingAction = true;
		}

		if (TakingAction)
		{
			if (m_LastAttackTimer >= m_AttackDelay)
			{
				//Do attack 
				UpdateAttacking(dt);
			}
			else
			{
				TakingAction = false;
			}
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
}

void CHealer::UpdateAttacking(double dt)
{
	//True  =  - speed, curr rotation > target rotation
	//False  =  + speed, curr rotation < target rotation
	bool SwingDirection;
	bool HasReturned;

	//Attacking
	if (!m_RodSwing)
	{
		m_RodRotation = EntityRotation(dt, ROD_SWING_SPEED, ROD_SWING_ROT_AMOUNT, m_RodRotation);
	}
	//Sword returning
	else
	{
		m_RodRotation = EntityRotation(dt, ROD_SWING_SPEED, ROD_SWING_INIT_AMOUNT, m_RodRotation);
	}

	if (m_RodRotation >= ROD_SWING_ROT_AMOUNT)
	{
		m_RodSwing = true;
	}

	else if (m_RodRotation <= ROD_SWING_INIT_AMOUNT)
	{
		m_LastAttackTimer = 0.0f;
		TakingAction = false;
		m_RodSwing = false;
	}
}

float CHealer::GetChildRotation(int ChildID)
{
	switch (ChildID)
	{
	case 1:
		return this->m_RodRotation;
		break;
	case 2:
		break;
	}

	return 0;
}

void CHealer::TickTimer(double dt)
{
	m_LastAttackTimer += m_AttackSpeed * dt;
}
