#include "Entity.h"


CEntity::CEntity()
{
	srand((unsigned)time(NULL));
	state = MOVE;
	IsTarget = false;
}


CEntity::~CEntity()
{
}

Vector3 CEntity::GetPosition()
{
	return this->Position;
}

void CEntity::SetTargetPosition(Vector3 TargetPosition)
{
	this->TargetPosition = TargetPosition;
}

void CEntity::SetDangerPosition(Vector3 DangerPosition)
{
	this->DangerPosition = DangerPosition;
}

int CEntity::Probability(int lowerLimit, int upperLimit)
{
	return rand() % (upperLimit - lowerLimit + 1) + lowerLimit;
}

void CEntity::SetIsTarget(bool TF)
{
	this->IsTarget = TF;
}

float CEntity::GetAttackRange(void)
{
	return this->m_AttackRange;
}

void CEntity::SetDangerZone(float DZ)
{
	this->m_DangerZone = DZ;
}

float CEntity::GetHpPercent(void)
{
	float m_HP_Percent = ((float)this->m_Curent_HP / (float)this->m_HP) * 100;

	return m_HP_Percent;
}

void CEntity::Move(Vector3 TargetDestination, double dt)
{
	Vector3 Direction;
	
	// Get the direction
	Direction = TargetDestination - Position;
	Position += Direction.Normalized() * m_MoveSpeed * dt;
}

void CEntity::Retreat(Vector3 TargetDestination, double dt)
{
	Vector3 Direction;

	// Get the direction
	Direction = Position - TargetDestination;
	Position += Direction.Normalized() * m_RunSpeed * dt;
}

void CEntity::Attack(void)
{

}

void CEntity::RunFSM(double dt)
{

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
}
void CEntity::UpdateAttacking(void)
{

}

string CEntity::GetState(void)
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
	default:
		DummyText = ClassName + "NULL";
		return DummyText;
		break;
	}
}