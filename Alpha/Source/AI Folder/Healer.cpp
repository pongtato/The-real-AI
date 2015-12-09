#include "Healer.h"


CHealer::CHealer()
{
	IsTarget = false;
	ClassName = "Healer: ";
	m_MoveSpeed = 30.f;
	m_RunSpeed = m_MoveSpeed * 0.5f;
	ID = HEALER;
	targetID = HEALER;
	m_Damage = 15.f;
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
	m_AttackRangeOffset = (m_AttackRange - 5.f);
	m_AttackSpeed = 1.0f;
	m_LastAttackTimer = m_AttackDelay;

	TakingAction = false;

	m_Priority = 1;
}


CHealer::~CHealer()
{
}

string CHealer::PrintState(void)
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
	case CHealer::HEAL_SELF:
		DummyText = ClassName + "HEAL SELF";
		return DummyText;
		break;
	case CHealer::RETREAT:
		DummyText = ClassName + "RETREAT";
		return DummyText;
		break;
	case CHealer::REVIVE:
		DummyText = ClassName + "REVIVE";
		return DummyText;
		break;
	case CHealer::DEAD:
		DummyText = ClassName + "DEAD";
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
	if (m_Curent_HP <= 0)
	{
		m_Active = false;
		state = DEAD;
		return;
	}

	CEntity* target = NULL;
	CBoss* Boss = NULL;
	for (unsigned int i = 0; i < ListOfCharacters.size(); ++i)
	{
		if (ListOfCharacters[i]->GetTYPE() == "BOSS")
		{
			Boss = (CBoss*)ListOfCharacters[i];
			break;
		}
	}
	DangerPosition = newDangerPosition;
	ComputeDangerPosition(ListOfCharacters);
	//Face the targets position
	FaceTarget();
	TickTimer(dt);

	//Go through list for lowest hp
	for (int i = 0; i < ListOfCharacters.size(); ++i)
	{
		if (target == NULL)
		{
			TargetPosition = ListOfCharacters[i]->GetPosition();
			target = ListOfCharacters[i];
		}
		else if (ListOfCharacters[i]->GetHpPercent() < target->GetHpPercent() && ListOfCharacters[i]->GetTYPE() != "BOSS")
		{
			TargetPosition = ListOfCharacters[i]->GetPosition();
			target = ListOfCharacters[i];
		}
	}

	if (m_DangerZone > (Position - DangerPosition).Length())
	{
		m_RodRotation = ROD_SWING_INIT_AMOUNT;
		state = RETREAT;
	}
	else
	{
		m_DangerZone = Boss->GetAttackRange() * 1.5f;
	}

	if (Boss->GetCastingSkillBool()) // If Boss is casting AoE skill, overwrite any existing state into RETREAT
	{
		//test
		state = RETREAT;
		m_DangerZone = Boss->GetSkillRadius();
	}
	switch (state)
	{
	case MOVE:
		if (m_AttackRangeOffset < (TargetPosition - Position).Length())
		{
			Move(ListOfCharacters,TargetPosition, dt);
		}
		else if (SeekDead(ListOfCharacters))
		{
			state = REVIVE;
		}
		else if ( this->GetHpPercent() > 40)
		{
			state = HEAL;
		}
		else
		{
			state = HEAL_SELF;
		}
		break;
	case HEAL:
		if (m_AttackRange >= (TargetPosition - Position).Length())
		{
			TakingAction = true;
		}

		if (TakingAction)
		{
			if (m_LastAttackTimer >= m_AttackDelay)
			{
				if (!target->GetActive())
				{
					state = REVIVE;
				}
				//Do Healing
				else if (target && target->GetActive())
				{
					UpdateAttacking(target, dt);
				}

				
				m_StateChangeTimer = 0.0f;
			}
			else
			{
				TakingAction = false;
			}
		}
		else
		{
			if (m_StateChangeTimer >= StateChangeDelay)
			{
				state = MOVE;
			}
		}
		break;
	case HEAL_SELF:

		if (GetHpPercent() <= 40)
		{
			TakingAction = true;
			if (m_LastAttackTimer >= m_AttackDelay)
			{
				//Do Healing
				UpdateAttacking(this, dt);
				m_StateChangeTimer = 0.0f;
			}
			else
			{
				TakingAction = false;
			}
		}
		else
		{
			if (m_StateChangeTimer >= StateChangeDelay)
			{
				state = MOVE;
			}
		}
		break;
	case RETREAT:
		if (m_DangerZone > (Position - DangerPosition).Length())
		{
			m_StateChangeTimer = 0.0f;
			Retreat(ListOfCharacters,DangerPosition, dt);
		}
		else if (m_StateChangeTimer <= StateChangeDelay)
		{
			Retreat(ListOfCharacters,DangerPosition, dt);
		}
		else
		{
			state = ATTACK;
		}
		break;
	case REVIVE:
		if (m_AttackRange >= (TargetPosition - Position).Length())
		{
			TakingAction = true;
		}
		else
		{
			Move(ListOfCharacters, TargetPosition, dt);
		}

		if (TakingAction)
		{
			if (m_LastAttackTimer >= m_AttackDelay)
			{
				//Do Healing
				if (target)
				{
					UpdateAttacking(target, dt);
				}
			}
			else
			{
				state = MOVE;
				TakingAction = false;
			}
		}
		break;
	default:
		break;
	}
}

void CHealer::UpdateAttacking(CEntity* target ,double dt)
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
		if (target->GetHpPercent() < 100)
		{
			if (!target->GetActive())
			{
				target->SetActive(true);
				target->SetCurrentHealthPoint(m_Damage * 3); // Heal the target
			}
			else
			{
				target->SetCurrentHealthPoint(target->GetCurrentHealthPoint() + m_Damage); // Heal the target
			}
			
		}
		m_LastAttackTimer = 0.0f;
		TakingAction = false;
		m_RodSwing = false;
#if _DEBUG
		cout << "===========================================" << endl;
		cout << this->GetID() << " heals " << target->GetID() << " for " << m_Damage << endl;
		cout << target->GetID() << " current HP : " << target->GetCurrentHealthPoint() << endl;
		cout << "===========================================" << endl;
#endif
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

float CHealer::GetChildTranslation(int ChildID)
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

void CHealer::TickTimer(double dt)
{
	m_LastAttackTimer += m_AttackSpeed * dt;
	m_StateChangeTimer += dt;
}

void CHealer::CustomStates(double dt)
{

}

float CHealer::TargetOverRide(void)
{
	return 0;
}

int CHealer::GetState(void)
{
	return this->state;
}

bool CHealer::DamageNullfiy(void)
{
	return false;
}