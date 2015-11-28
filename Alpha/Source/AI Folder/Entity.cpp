#include "Entity.h"


CEntity::CEntity()
{
	state = MOVE;
	IsTarget = false;
}


CEntity::~CEntity()
{
}

void CEntity::Move(Vector3 TargetDestination)
{
	Vector3 Direction;
	
	// Get the direction
	Direction = TargetDestination - Position;
	Position += Direction.Normalized() * m_MoveSpeed;
}

void CEntity::Retreat(Vector3 TargetDestination)
{
	Vector3 Direction;

	// Get the direction
	Direction = Position - TargetDestination;
	Position += Direction.Normalized() * m_RunSpeed;
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