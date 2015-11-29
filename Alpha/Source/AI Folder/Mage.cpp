#include "Mage.h"


CMage::CMage()
{
	IsTarget = false;
	ClassName = "Mage: ";
	m_MoveSpeed = 20.f;
	
	m_RunSpeed = m_MoveSpeed * 0.5f;
	ID = MAGE;
	m_HP = 100;
	m_Curent_HP = m_HP;

	Position.Set(
		Probability(0, 100),
		0.1f,
		Probability(0, 100));

	m_StaffRotation = 0.0f;

	m_StaffSwing = false;

	//Attack Init
	m_AttackRange = 45.f;
	m_AttackSpeed = 1.0f;
	m_LastAttackTimer = m_AttackDelay;

	TakingAction = false;
}


CMage::~CMage()
{
}

void CMage::RunFSM(double dt, Vector3 newTargetPosition, Vector3 newDangerPosition)
{
	TargetPosition = newTargetPosition;
	DangerPosition = newDangerPosition;
	//Face the targets position
	FaceTarget();

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
			UpdateAttacking(dt);
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
void CMage::UpdateAttacking(double dt)
{
	//True  =  - speed, curr rotation > target rotation
	//False  =  + speed, curr rotation < target rotation
	bool SwingDirection = (m_StaffRotation > STAFF_SWING_ROT_AMOUNT ? true : false);
	bool HasReturned = (m_StaffRotation > 0 ? true : false);

	TakingAction = true;

	//Attacking
	if (!m_StaffSwing)
	{
		m_StaffRotation = EntityRotation(dt, STAFF_SWING_SPEED, STAFF_SWING_ROT_AMOUNT, m_StaffRotation);
	}
	//Sword returning
	else
	{
		m_StaffRotation = EntityRotation(dt, STAFF_SWING_SPEED, STAFF_SWING_INIT_AMOUNT, m_StaffRotation);
	}

	//Attacking trigger
	if (SwingDirection)
	{
		if (m_StaffRotation < STAFF_SWING_ROT_AMOUNT)
		{
			//Trigger sword return
			m_StaffRotation = STAFF_SWING_ROT_AMOUNT;
			m_StaffSwing = true;
		}
	}
	else
	{
		if (m_StaffRotation > STAFF_SWING_ROT_AMOUNT)
		{
			//Trigger sword return
			m_StaffRotation = STAFF_SWING_ROT_AMOUNT;
			m_StaffSwing = true;
		}
	}

	//Return trigger
	if (HasReturned)
	{
		if (m_StaffRotation < STAFF_SWING_ROT_AMOUNT)
		{
			//Trigger attack cooldown
			m_StaffRotation = STAFF_SWING_INIT_AMOUNT;
			m_LastAttackTimer = 0.0f;
			m_StaffSwing = false;
			TakingAction = false;
		}
	}
	else
	{
		if (m_StaffRotation > STAFF_SWING_ROT_AMOUNT)
		{
			//Trigger attack cooldown
			m_StaffRotation = STAFF_SWING_INIT_AMOUNT;
			m_LastAttackTimer = 0.0f;
			m_StaffSwing = false;
			TakingAction = false;
		}
	}
}

float CMage::GetChildRotation(int ChildID)
{
	switch (ChildID)
	{
	case 1:	
		break;
	case 2:
		break;
	}

	return 0;
}

void CMage::TickTimer(double dt)
{
	m_LastAttackTimer += m_AttackSpeed * dt;
}