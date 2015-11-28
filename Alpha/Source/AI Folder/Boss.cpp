#include "Boss.h"


CBoss::CBoss()
:TargetChangeProbability (33.3f)
,TargetChangeDelay(300.f)
{
	InitialPos.Set(60.f, 0.f, 0.f);
	ClassName = "Boss: ";
	Position = InitialPos;
	TargetAcquireRange = 10.f;
	IsTaunted = false;
	TargetChangeTimer = TargetChangeDelay;
	m_AttackRange = 0.5f;
	m_MoveSpeed = 0.01f;
	m_RunSpeed = m_MoveSpeed * 0.5f;
	ID = BOSS;
	ResetRange = 6.f;
}


CBoss::~CBoss()
{
}

int CBoss::ChooseTarget(int RNG)
{
	
	//Choose the unlucky boi
	// 0 ~ 33
	if (RNG >= 0 && RNG <= TargetChangeProbability * (HEALER+1))
	{
		target = HEALER;
	}
	// 34 ~ 66
	else if (RNG > TargetChangeProbability * (HEALER+1) && RNG <= TargetChangeProbability * (MAGE + 1))
	{
		target = MAGE;
	}
	//67 ~ 99
	else if (RNG > TargetChangeProbability * (MAGE+1) && RNG <= TargetChangeProbability * (TANK+1))
	{
		target = TANK;
	}

	if (m_AttackRange < TargetPosition.Length())
	{
		state = MOVE;
	}

	//Clear for next choice
	TargetList.clear();

	return target;
}

void CBoss::TickTimer(void)
{
	TargetChangeTimer += 1.f;
}


void CBoss::RunFSM(double dt)
{
	if ((InitialPos - Position).Length() > ResetRange)
	{
		this->state = RESET;
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
	case RESET:
		if ((TargetPosition - Position).Length() > 1.f)
		{
			Move(InitialPos);
		}
		else
		{
			state = MOVE;
		}
		break;
	default:
		break;
	}
	UpdateAttacking();
}

string CBoss::GetState(void)
{
	string DummyText;
	switch (state)
	{
	case MOVE:
		DummyText = ClassName + "MOVE";
		return DummyText;
		break;
	case ATTACK:
		DummyText = ClassName + "ATTACK";
		return DummyText;
		break;
	case RETREAT:
		DummyText = ClassName + "RETREAT";
		return DummyText;
		break;
	case RESET:
		DummyText = ClassName + "RESET";
		return DummyText;
		break;
	default:
		DummyText = ClassName + "NULL";
		return DummyText;
		break;
	}
}
void CBoss::UpdateAttacking(void)
{
	if (state == ATTACK)

	{
	}

	else
	{
	}
}