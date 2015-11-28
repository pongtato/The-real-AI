#include "Boss.h"


CBoss::CBoss()
:TargetChangeDelay(30.f)
{
	ClassName = "Boss: ";
	TargetAcquireRange = 10.f;
	IsTaunted = false;
	TargetChangeTimer = TargetChangeDelay;
	m_AttackRange = 15.f;
	m_MoveSpeed = 10.f;
	m_RunSpeed = m_MoveSpeed * 0.5f;
	ID = BOSS;
	ResetRange = 100.f;
	CurrentTarget = 0;

	InitialPos.Set(
		Probability(0, 100),
		0.1f, 
		Probability(0, 100));

	Position = InitialPos;
}


CBoss::~CBoss()
{
}

void CBoss::TickTimer(double dt)
{
	TargetChangeTimer += 1.f;
}

int CBoss::GetCurrentTarget(void)
{
	return this->CurrentTarget;
}


void CBoss::RunFSM(double dt, vector<CEntity*> ListOfEnemies, Vector3 newTargetPosition, Vector3 newDangerPosition)
{
	//Boss event timer
	TickTimer(dt);
	//Face the targets position
	FaceTarget();

	if (TargetChangeTimer > TargetChangeDelay)
	{
		CurrentTarget =	Probability(0, ListOfEnemies.size()-1);
		TargetChangeTimer = 0.0f;
	}

	if (ListOfEnemies[CurrentTarget]->GetTYPE() == BOSS)
	{
		CurrentTarget = Probability(0, ListOfEnemies.size() - 1);
	}
	else
	{
		TargetPosition = ListOfEnemies[CurrentTarget]->GetPosition();
	}

	//Prevent game from leaving world space
	if ((InitialPos - Position).Length() > ResetRange)
	{
		this->state = RESET;
	}

	//Handle states
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
			UpdateAttacking();
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
	case RESET:
		if ((TargetPosition - Position).Length() > 1.f)
		{
			Move(InitialPos, dt);
		}
		else
		{
			state = MOVE;
		}
		break;
	default:
		break;
	}
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

}