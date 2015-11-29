#include "Tank.h"


CTank::CTank()
{
	IsTarget = false;
	ClassName = "Tank: ";
	m_MoveSpeed = 20.f;
	m_RunSpeed = m_MoveSpeed * 0.5f;
	ID = TANK;
	m_AttackSpeed = 1.0f;
	m_Damage = 10.f;
	m_HP = 100;
	//Testing
	m_Curent_HP = 50;

	Position.Set(
		Probability(0, 100),
		0.1f,
		Probability(0, 100));

	m_SwordRotation = 0.0f;
	m_ShieldRotation = 0.0f;

	m_SwordSwing = false;
	m_ShieldSwing = false;

	//Attack Init
	m_AttackRange = 15.0f;
	m_AttackSpeed = 1.0f;
	m_LastAttackTimer = m_AttackDelay;

	TakingAction = false;
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
	TickTimer(dt);

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

float CTank::GetChildRotation(int ChildID)
{
	switch (ChildID)
	{
	case 1:
		return GetSwordRotation();
		break;
	case 2:
		return GetShieldRotation();
		break;
	}
}

float CTank::GetSwordRotation(void)
{
	return this->m_SwordRotation;
}

float CTank::GetShieldRotation(void)
{
	return this->m_ShieldRotation;
}

void CTank::TickTimer(double dt)
{
	m_LastAttackTimer += m_AttackSpeed * dt;
}

void CTank::UpdateAttacking(double dt)
{
	//True  =  - speed, curr rotation > target rotation
	//False  =  + speed, curr rotation < target rotation
	bool SwingDirection;
	bool HasReturned;

	//Attacking
	if (!m_SwordSwing)
	{
		m_SwordRotation = EntityRotation(dt, SWORD_SWING_SPEED, SWORD_SWING_ROT_AMOUNT, m_SwordRotation);
	}
	//Sword returning
	else
	{
		m_SwordRotation = EntityRotation(dt, SWORD_SWING_SPEED, SWORD_SWING_INIT_AMOUNT, m_SwordRotation);
	}

	if (m_SwordRotation >= SWORD_SWING_ROT_AMOUNT)
	{
		m_SwordSwing = true;
	}

	else if (m_SwordRotation <= SWORD_SWING_INIT_AMOUNT)
	{
		m_LastAttackTimer = 0.0f;
		TakingAction = false;
		m_SwordSwing = false;
	}
}
