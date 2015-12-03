#include "Boss.h"


CBoss::CBoss()
:TargetChangeDelay(10.f)
{
	ClassName = "Boss: ";
	TargetAcquireRange = 10.f;
	TargetChangeTimer = TargetChangeDelay;
	m_MoveSpeed = 10.f;
	m_RunSpeed = m_MoveSpeed * 0.5f;
	ID = BOSS;

	ResetRange = 10000.f;
	CurrentTarget = 0;

	InitialPos.Set(
		Probability(0, 100),
		0.1f, 
		Probability(0, 100));

	Position = InitialPos;
	m_HP = 2000;
	m_Curent_HP = m_HP;
	m_IsCastingSkill = false;
	m_DamageThreshold = 100.f;
	m_TotalDamageTaken = 0.f;
	m_CastingTimer = 0.f;
	m_Damage = DEFAULT_DAMAGE;
	m_Skill_Range_Radius = 1.f;
	m_ArmSwing = false;
	m_ArmRotation = 0.0f;

	//Attack Init
	m_AttackRange = 15.f;
	m_AttackRangeOffset = (m_AttackRange - 5.f);
	m_AttackSpeed = 1.0f;
	m_LastAttackTimer = m_AttackDelay;

	TakingAction = false;
	IsTaunted = false;
}


CBoss::~CBoss()
{
}
float CBoss::GetSkillRadius(void)
{
	return m_Skill_Range_Radius;
}
void CBoss::TickTimer(double dt)
{
	m_StateChangeTimer += dt;
	TargetChangeTimer += (float)dt;
	if (m_LastAttackTimer <= m_AttackSpeed)
		m_LastAttackTimer += (float)dt;

	if (m_Cooldown > 0)
		m_Cooldown -= (float)dt;

	if (m_IsCastingSkill)
	{
		m_CastingTimer += (float)dt;
		m_Skill_Range_Radius += (float)dt * 50.f;
	}
	if (m_TauntTimer > 0)
	{
		m_TauntTimer -= (float)dt;
	}
	else
	{
		IsTaunted = false;
	}
}
bool CBoss::GetCastingSkillBool(void)
{
	return m_IsCastingSkill;
}

int CBoss::GetCurrentTarget(void)
{
	return this->CurrentTarget;
}

void CBoss::TargetPriorityCheck(vector<CEntity*> ListOfEnemies)
{
	int currentThreat = 1;
	int newTarget = 0;

	for (int i = 0; i < ListOfEnemies.size(); ++i)
	{
		if (ListOfEnemies[i]->GetPriorityLevel() > currentThreat)
		{
			currentThreat = ListOfEnemies[i]->GetPriorityLevel();
			newTarget = i;
		}
	}

	if (currentThreat > 1)
	{
		IsTaunted = true;
		m_TauntTimer = ListOfEnemies[newTarget]->TargetOverRide();
		TargetPosition = ListOfEnemies[newTarget]->GetPosition();
		CurrentTargetType = ListOfEnemies[newTarget]->GetTargetID();
		targetID_NO = ListOfEnemies[newTarget]->GetIDNO();
		CurrentTarget = targetID_NO;
	}
}

void CBoss::ChooseTarget(vector<CEntity*> ListOfEnemies)
{
	if (TargetChangeTimer > TargetChangeDelay)
	{
		CurrentTarget = Probability(0, ListOfEnemies.size() - 1);
		TargetChangeTimer = 0.0f;
	}

	if (ListOfEnemies[CurrentTarget]->GetTYPE() == BOSS)
	{
		CurrentTarget = Probability(0, ListOfEnemies.size() - 1);
	}
	else
	{
		TargetPosition = ListOfEnemies[CurrentTarget]->GetPosition();
		CurrentTargetType = ListOfEnemies[CurrentTarget]->GetTargetID();
		targetID_NO = ListOfEnemies[CurrentTarget]->GetIDNO();
	}
}

void CBoss::RunFSM(double dt, vector<CEntity*> ListOfEnemies, Vector3 newTargetPosition, Vector3 newDangerPosition)
{
	// BOSS RESET ---------------
	if (m_Curent_HP <= 0)
	{
		m_Curent_HP = m_HP;
		Position.Set(
			Probability(0, 100),
			0.1f,
			Probability(0, 100));
	}
	// --------------------------


	//Boss event timer
	TickTimer(dt);
	//Face the targets position
	FaceTarget();

	TargetPriorityCheck(ListOfEnemies);
	if (!IsTaunted)
	{
		ChooseTarget(ListOfEnemies);
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
		if (m_AttackRangeOffset < (TargetPosition - Position).Length())
		{
			Move(ListOfEnemies,TargetPosition, dt);
		}
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
				UpdateAttacking(ListOfEnemies[CurrentTarget], dt);
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
			Retreat(ListOfEnemies,DangerPosition, dt);
		}
		else
		{
			state = ATTACK;
		}
		break;
	case RESET:
		if ((TargetPosition - Position).Length() > 1.f)
		{
			Move(ListOfEnemies,InitialPos, dt);
		}
		else
		{
			state = MOVE;
		}
		break;
	case CAST_SKILL:
		if (m_CastingTimer > DEFAULT_CASTING_TIME)
		{
#if _DEBUG
			cout << "===========================================" << endl;
			cout << "Boss slams the ground for AoE Damage!" << endl;
			cout << "===========================================" << endl;
#endif
			m_IsCastingSkill = false;
			m_CastingTimer = 0;
			m_TotalDamageTaken = 0; // Reset the damage taken threshold

			// Damage every enemy around the Boss within a radius
			for (unsigned short i = 0; i < ListOfEnemies.size(); ++i)
			{
				if ((ListOfEnemies[i]->GetPosition() - Position).Length() < m_Skill_Range_Radius && ListOfEnemies[i]->GetTYPE() != "BOSS")
				{
					ListOfEnemies[i]->SetCurrentHealthPoint(ListOfEnemies[i]->GetCurrentHealthPoint() - DEFAULT_SKILL_DAMAGE);

#if _DEBUG
					cout << "===========================================" << endl;
					cout << ListOfEnemies[i]->GetID() << " takes " << DEFAULT_SKILL_DAMAGE << " Damage!" << endl;
					cout << ListOfEnemies[i]->GetID() << " current HP " << ListOfEnemies[i]->GetCurrentHealthPoint() << endl;
					cout << "===========================================" << endl;
#endif
				}
			}
			m_Skill_Range_Radius = 1.f;

			state = MOVE;
		}
		break;
	default:
		break;
	}
}

string CBoss::PrintState(void)
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
	case CAST_SKILL:
		DummyText = ClassName + "CASTING SKILL";
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
void CBoss::AddDamageTaken(float dmg)
{
	m_TotalDamageTaken += dmg;
}
float CBoss::GetDamageTaken(void)
{
	return m_TotalDamageTaken;
}

void CBoss::UpdateAttacking(CEntity* target, double dt)
{
//	// Damage the target when boss is able to attack
//	if (m_LastAttackTimer >= m_AttackSpeed)
//	{
//		target->SetCurrentHealthPoint(target->GetCurrentHealthPoint() - m_Damage);
//		m_LastAttackTimer = 0;
//
//#if _DEBUG
//		cout << "Boss damages the enemy for " << m_Damage << endl;
//#endif
//	}

	if (m_TotalDamageTaken >= m_DamageThreshold && m_Cooldown <= 0 && !m_IsCastingSkill)
	{
		m_IsCastingSkill = true;	// Set the boolean for casting to true
		state = CAST_SKILL;	// Change it's state to casting of skill
		m_Cooldown = DEFAULT_COOLDOWN;	// Put the skill on cooldown
#if _DEBUG
		cout << "Boss is channelling its skill!!!" << endl;
#endif
	}

	bool HasReturned;

	//Attacking
	if (!m_ArmSwing)
	{
		m_ArmRotation = EntityRotation(dt, ARM_SWING_SPEED, ARM_SWING_ROT_AMOUNT, m_ArmRotation);
	}
	//Sword returning
	else
	{
		m_ArmRotation = EntityRotation(dt, ARM_SWING_SPEED, ARM_SWING_INIT_AMOUNT, m_ArmRotation);
	}

	if (m_ArmRotation <= ARM_SWING_ROT_AMOUNT)
	{
		m_ArmSwing = true;
	}
	// Damage the target when boss is able to attack
	else if (m_ArmRotation >= ARM_SWING_INIT_AMOUNT)
	{
		if (target->DamageNullfiy())
		{
			cout << " Blocked " << endl;
#if _DEBUG
			cout << "Boss damages "<<target->GetID() << " for " << 0 << endl;
#endif
		}
		else
		{
			target->SetCurrentHealthPoint(target->GetCurrentHealthPoint() - m_Damage);
			cout << target->GetCurrentHealthPoint() << endl;

#if _DEBUG
			cout << "Boss damages " << target->GetID() << " for " << m_Damage << endl;
#endif

		}
		
		m_LastAttackTimer = 0.0f;
		TakingAction = false;
		m_ArmSwing = false;
	}
}

float CBoss::GetChildRotation(int ChildID)
{
	switch (ChildID)
	{
	case 1:
		return this->m_ArmRotation;
		break;
	case 2:
		break;
	}

	return 0;
}

float CBoss::GetChildTranslation(int ChildID)
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

void CBoss::CustomStates(double dt)
{
	
}

float CBoss::TargetOverRide(void)
{
	return 0;
}

int CBoss::GetState(void)
{
	return this->state;
}

bool CBoss::DamageNullfiy(void)
{
	return false;
}