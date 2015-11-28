#include "Tank.h"


CTank::CTank()
{
	IsTarget = false;
	ClassName = "Tank: ";
	m_MoveSpeed = 20.f;
	m_AttackRange = 15.0f;
	m_RunSpeed = m_MoveSpeed * 0.5f;
	ID = TANK;

	m_HP = 100;
	//Testing
	m_Curent_HP = 50;

	Position.Set(
		Probability(0, 100),
		0.1f,
		Probability(0, 100));
}


CTank::~CTank()
{
}

void CTank::RunFSM(double dt, Vector3 newTargetPosition, Vector3 newDangerPosition)
{
	TargetPosition = newTargetPosition;
	DangerPosition = newDangerPosition;
	//Face the targets position
	FaceTarget();

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
	default:
		break;
	}
}
void CTank::UpdateAttacking(void)
{

}
