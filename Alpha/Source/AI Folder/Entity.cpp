#include "Entity.h"


CEntity::CEntity()
{
	srand((unsigned)time(NULL));
	state = MOVE;
	IsTarget = false;
	m_Rotation = 0.0f;
	m_LastAttackTimer = 0.f;
	m_StateChangeTimer = 0.f;
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

void CEntity::RandomSpawn(int lowerLimit, int upperLimit)
{
	int Rand1 = rand() % (upperLimit - lowerLimit + 1) + lowerLimit;
	int Rand2 = rand() % (upperLimit - lowerLimit + 1) + lowerLimit;
	this->Position.Set(Rand1, 0.0f, Rand2);
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

float CEntity::GetRotation(void)
{
	return this->m_Rotation;
}

void CEntity::SetID(string newID, string newTYPE, int ID_NO)
{
	this->ID = newID;
	this->TYPE = newTYPE;
	this->ID_NO = ID_NO;
}

string CEntity::GetID(void)
{
	return this->ID;
}

string CEntity::GetTYPE(void)
{
	return this->TYPE;
}

void CEntity::FaceTarget(void)
{
	Vector3 Direction = TargetPosition - Position;
	if (!Direction.IsZero())
	{
		Direction.Normalize();
	}
		this->m_Rotation = Math::RadianToDegree(atan2(Direction.x, Direction.z)) + MAYA_MODEL_OFFSET;
}

void CEntity::SetRotation(float Rotate)
{
	this->m_Rotation = Rotate;
}

float CEntity::EntityRotation(double dt, float speed, float MaxRotate, float InputRotation)
{
	int RotationDirection = (InputRotation > MaxRotate ? 1 : -1);
	{
		return InputRotation - speed * (float)dt * RotationDirection;
	}
}

float CEntity::EntityTranslation(double dt, float speed, float MaxTranslate, float InputTranslate)
{
	int TranslateDirection = (InputTranslate > MaxTranslate ? 1 : -1);
	{
		return InputTranslate - speed * (float)dt * TranslateDirection;
	}
}

void CEntity::Move(Vector3 TargetDestination, double dt)
{
	Vector3 Direction;

	// Get the direction
	Direction = (TargetDestination - Position).Normalized();
	Position += Direction * m_MoveSpeed * dt;
}

void CEntity::Retreat(Vector3 TargetDestination, double dt)
{
	Vector3 Direction;

	// Get the direction
	Direction = (Position - TargetDestination).Normalized();
	Position += Direction * m_RunSpeed * dt;
}

void CEntity::Attack(void)
{

}

void CEntity::RunFSM(double dt, Vector3 newTargetPosition, Vector3 newDangerPosition)
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

void CEntity::RunFSM(double dt, vector<CEntity*> ListOfCharacters, Vector3 newTargetPosition, Vector3 newDangerPosition)
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

void CEntity::UpdateAttacking(CEntity*, double dt)
{

}

void CEntity::SetCurrentHealthPoint(float hp)
{
	if (hp >= m_HP)
	{
		this->m_Curent_HP = m_HP;
	}
	else
	{
		this->m_Curent_HP = hp;
	}
}

float CEntity::GetCurrentHealthPoint(void)
{
	return m_Curent_HP;
}

void CEntity::CustomStates(double dt)
{
	
}

CEntity::Targets CEntity::GetTargetID(void)
{
	return this->targetID;
}

int CEntity::GetPriorityLevel(void)
{
	return this->m_Priority;
}

int CEntity::GetIDNO(void)
{
	return this->ID_NO;
}

int CEntity::GetTargetIDNO(void)
{
	return this->targetID_NO;
}

bool CEntity::DamageNullfiy(void)
{
	return false;
}

