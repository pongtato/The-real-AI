#include "Tank.h"


CTank::CTank()
{
	IsTarget = false;
	ClassName = "Tank: ";
	m_MoveSpeed = 20.f;
	m_RunSpeed = m_MoveSpeed * 0.5f;
	ID = TANK;
	targetID = TANK;
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
	m_AttackRangeOffset = (m_AttackRange - 5.f);
	m_AttackSpeed = 1.0f;
	m_LastAttackTimer = m_AttackDelay;

	TakingAction = false;
	
	m_SwordTranslate = 0.0f;
	m_Cooldown = m_SkillDelay;

	m_Priority = 1;
}


CTank::~CTank()
{
}

void CTank::RunFSM(double dt, vector<CEntity*> ListOfCharacters, Vector3 newTargetPosition, Vector3 newDangerPosition)
{
	CBoss* Boss = NULL;
	for (unsigned int i = 0; i < ListOfCharacters.size(); ++i)
	{
		if (ListOfCharacters[i]->GetTYPE() == "BOSS")
		{
			Boss = (CBoss*)ListOfCharacters[i];
			break;
		}
	}
	TargetPosition = newTargetPosition;
	DangerPosition = newDangerPosition;
	//Face the targets position
	FaceTarget();
	TickTimer(dt);

	if (IsTarget && GetHpPercent() <= 25)
	{
		state = RETREAT;
	}
	if (Boss->GetCastingSkillBool()) // If Boss is casting AoE skill, overwrite any existing state into RETREAT
	{
		//test
		state = RETREAT;
		m_DangerZone = Boss->GetSkillRadius();
	}
	else
	{
		m_DangerZone = Boss->GetAttackRange() * 1.5f;
	}
	switch (state)
	{
	case MOVE:
		if (m_AttackRangeOffset < (TargetPosition - Position).Length())
		{
			Move(ListOfCharacters,TargetPosition, dt);
		}
		else if (m_Cooldown >= m_SkillDelay && TauntCheck(ListOfCharacters))
		{
			state = TAUNT;
		}
		else if (AttackCheck(ListOfCharacters))
		{
			m_isBlock = true;
			state = BLOCK;
		}
		//Taunt uanvailable
		else if (m_LastAttackTimer >= m_AttackDelay)
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
				UpdateAttacking(Boss, dt);
				m_StateChangeTimer = 0.0f;
			}
			else if (AttackCheck(ListOfCharacters))
			{
				m_isBlock = true;
				state = BLOCK;
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
	case TAUNT:
		if (m_AttackRange >= (TargetPosition - Position).Length())
		{
			TakingAction = true;
		}

		if (TakingAction)
		{
			if (m_Cooldown >= m_SkillDelay)
			{
				//Do taunt
				CustomStates(dt);
			}
			else
			{
				TakingAction = false;
				state = ATTACK;
			}
		}
		break;
	case BLOCK:
		TakingAction = true;

		if (TakingAction)
		{
			if (m_isBlock)
			{
				//Do taunt
				CustomStates(dt);
			}
			else
			{
				TakingAction = false;
				state = ATTACK;
			}
		}
		break;
	case RETREAT:
		if (m_DangerZone > (Position - DangerPosition).Length())
		{
			m_StateChangeTimer = 0.0f;
			Retreat(ListOfCharacters, DangerPosition, dt);
		}
		else if (m_StateChangeTimer <= StateChangeDelay)
		{
			Retreat(ListOfCharacters, DangerPosition, dt);
		}
		else
		{
			state = MOVE;
		}

		if (GetHpPercent() > 25)
		{
			state = MOVE;
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

float CTank::GetChildTranslation(int ChildID)
{
	switch (ChildID)
	{
	case 1:
		return this->m_SwordTranslate;
		break;
	case 2:
	
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
	m_Cooldown += dt;
	m_StateChangeTimer += dt;
}

void CTank::UpdateAttacking(CEntity* target, double dt)
{
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
		

		if (CBoss* boss = dynamic_cast<CBoss*>(target))
		{
			boss->SetCurrentHealthPoint(boss->GetCurrentHealthPoint() - m_Damage); // Damage the boss
			boss->AddDamageTaken(m_Damage); // Update the damage threshold of the boss
			m_LastAttackTimer = 0.0f;
			TakingAction = false;
			m_SwordSwing = false;

#if _DEBUG
			cout << "===========================================" << endl;
			cout << this->GetID() << " damages " << boss->GetID() << " for: " << m_Damage << endl;
			cout << boss->GetID() << " current HP: " << boss->GetCurrentHealthPoint() << endl;
			cout << boss->GetID() << " damage taken since last AoE: " << boss->GetDamageTaken() << endl;
			cout << "===========================================" << endl;
#endif
		}
	}
}

void CTank::CustomStates(double dt)
{
	if (!m_isBlock)
	{
		UpdateTaunt(dt);
	}
	else
	{
		UpdateBlock(dt);
	}
}

void CTank::UpdateBlock(double dt)
{
	bool HasReturned;

	//Blocking
	if (!m_ShieldSwing)
	{
		m_ShieldRotation = EntityRotation(dt, SHIELD_SWING_SPEED, SHIELD_SWING_ROT_AMOUNT, m_ShieldRotation);
	}
	//Shield returning
	else
	{
		m_ShieldRotation = EntityRotation(dt, SHIELD_SWING_SPEED, SHIELD_SWING_INIT_AMOUNT, m_ShieldRotation);
	}

	if (m_ShieldRotation <= SHIELD_SWING_ROT_AMOUNT)
	{
		m_ShieldSwing = true;
	}

	else if (m_ShieldRotation >= SHIELD_SWING_INIT_AMOUNT)
	{
		TakingAction = false;
		m_ShieldSwing = false;
		m_isBlock = false;
	}
}

void CTank::UpdateTaunt(double dt)
{
	bool HasReturned;
	m_Priority = 2;

	//Attacking
	if (!m_SwordSwing)
	{
		m_SwordTranslate = EntityTranslation(dt, (SWORD_SWING_SPEED * 0.05f), SWORD_TRANSLATE_AMOUNT, m_SwordTranslate);
	}
	//Sword returning
	else
	{
		m_SwordTranslate = EntityTranslation(dt, (SWORD_SWING_SPEED * 0.05f), SWORD_TRANSLATE_INIT_AMOUNT, m_SwordTranslate);
	}

	if (m_SwordTranslate >= SWORD_TRANSLATE_AMOUNT)
	{
		m_SwordSwing = true;
	}

	else if (m_SwordTranslate <= SWORD_TRANSLATE_INIT_AMOUNT)
	{
		m_Cooldown = 0.0f;
		m_Priority = 1;
		TakingAction = false;
		m_SwordSwing = false;
	}
}

bool CTank::TauntCheck(vector<CEntity*> ListOfCharacters)
{
	for (int i = 0; i < ListOfCharacters.size(); ++i)
	{
		if (ListOfCharacters[i]->GetTYPE() == "BOSS")
		{
			if (ListOfCharacters[i]->GetTargetID() != CEntity::TANK)
			{
				//Boss is meant to be taunted
				return true;
			}
		}
	}
	//No boss to taunt
	return false;
}

bool CTank::AttackCheck(vector<CEntity*> ListOfCharacters)
{
	for (int i = 0; i < ListOfCharacters.size(); ++i)
	{
		if (ListOfCharacters[i]->GetTYPE() == "BOSS")
		{
			if (ListOfCharacters[i]->GetState() == CTank::ATTACK && ListOfCharacters[i]->GetTargetIDNO() == this->ID_NO)
			{
				//Currently the target to be attacked, try to block
				int RNG = Probability(0, 100);
#if _DEBUG
				cout << " Trying to block, ROLLED: " << RNG << endl;
#endif
				if (RNG <= BLOCK_CHANCE)
				{
#if _DEBUG
					cout << " BLOCKING! " << endl;
#endif
					return true;
				}
			}
		}
	}
	//No need to block
	return false;
}

float CTank::TargetOverRide(void)
{
	return m_TauntDuration;
}

int CTank::GetState(void)
{
	return this->state;
}

bool CTank::DamageNullfiy(void)
{
	return m_isBlock;
}

string CTank::PrintState(void)
{
	string DummyText;

	switch (state)
	{
	case CTank::MOVE:
		DummyText = ClassName + "MOVE";
		return DummyText;
		break;
	case CTank::ATTACK:
		DummyText = ClassName + "ATTACK";
		return DummyText;
		break;
	case CTank::TAUNT:
		DummyText = ClassName + "TAUNT";
		return DummyText;
		break;
	case CTank::BLOCK:
		DummyText = ClassName + "BLOCK";
		return DummyText;
		break;
	case CTank::RETREAT:
		DummyText = ClassName + "RETREAT";
		return DummyText;
		break;
	default:
		DummyText = ClassName + "NULL";
		return DummyText;
		break;
	}
}
