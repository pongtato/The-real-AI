#include "Mage.h"


CMage::CMage()
{
	IsTarget = false;
	ClassName = "Mage: ";
	m_MoveSpeed = 20.f;
	
	m_RunSpeed = m_MoveSpeed * 0.5f;
	ID = MAGE;
	targetID = MAGE;

	m_HP = 100;
	m_Curent_HP = m_HP;
	m_AttackSpeed = 1.0f;
	m_Damage = 10.f;
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

	m_Priority = 1;
}


CMage::~CMage()
{
}

void CMage::RunFSM(double dt, vector<CEntity*> ListOfEnemies, Vector3 newTargetPosition, Vector3 newDangerPosition)
{
	CEntity* Boss = NULL;
	for (unsigned int i = 0; i < ListOfEnemies.size(); ++i)
	{
		if (ListOfEnemies[i]->GetTYPE() == "BOSS")
		{
			Boss = ListOfEnemies[i];
			break;
		}
	}
	if (m_LastAttackTimer <= m_AttackSpeed)
		m_LastAttackTimer += (float)dt;

	TargetPosition = newTargetPosition;
	DangerPosition = newDangerPosition;
	//Face the targets position
	FaceTarget();
	TickTimer(dt);

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
void CMage::UpdateAttacking(double dt)
{
	//True  =  - speed, curr rotation > target rotation
	//False  =  + speed, curr rotation < target rotation
	bool SwingDirection;
	bool HasReturned;

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

	if (m_StaffRotation >= STAFF_SWING_ROT_AMOUNT)
	{
		m_StaffSwing = true;
	}

	else if (m_StaffRotation <= STAFF_SWING_INIT_AMOUNT)
	{
		m_LastAttackTimer = 0.0f;
		TakingAction = false;
		m_StaffSwing = false;
	}
}

float CMage::GetChildRotation(int ChildID)
{
	switch (ChildID)
	{
	case 1:	
		return this->m_StaffRotation;
		break;
	case 2:
		break;
	}

	return 0;
}

float CMage::GetChildTranslation(int ChildID)
{
	switch (ChildID)
	{
	case 1:
		return 0;
		break;
	case 2:

		break;
	}
}

void CMage::TickTimer(double dt)
{
	m_LastAttackTimer += m_AttackSpeed * dt;
}

void CMage::CustomStates(double dt)
{
	
}

float CMage::TargetOverRide(void)
{
	return 0;
}

int CMage::GetState(void)
{
	return this->state;
}

bool CMage::DamageNullfiy(void)
{
	return false;
}