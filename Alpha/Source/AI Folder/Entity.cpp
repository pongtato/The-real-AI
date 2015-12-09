#include "Entity.h"


CEntity::CEntity()
{
	srand((unsigned)time(NULL));
	state = MOVE;
	IsTarget = false;
	m_Rotation = 0.0f;
	m_LastAttackTimer = 0.f;
	m_StateChangeTimer = 0.f;
	Direction.SetZero();
	m_Active = true;
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

void CEntity::Move(vector<CEntity*> ListOfCharacters,Vector3 TargetDestination, double dt)
{
	// Get the direction
	Direction = (TargetDestination - Position);

	Vector3 Alignment = ComputeAlignment(ListOfCharacters);
	Vector3 Cohesion = ComputeCohesion(ListOfCharacters);
	Vector3 Seperation = ComputeSeperation(ListOfCharacters);

	Direction.Normalize() * m_MoveSpeed;

	Direction += (Alignment  * m_alignmentWeight) + (Cohesion  * m_cohesionWeight) + (Seperation  * m_separationWeight);

	Position += Direction * m_MoveSpeed * dt;


	if (Position.x <= -150.f)
	{
		Position.x = 0.0f;
	}

	if (Position.z <= -150.f)
	{
		Position.z = 0.0f;
	}

	if (Position.x >= 150.f)
	{
		Position.x = 0.0f;
	}

	if (Position.z >= 150.f)
	{
		Position.z = 0.0f;
	}

	//Position.x = Math::Wrap(Position.x, -150.f, 150.f);
	//Position.z = Math::Wrap(Position.z, -150.f, 150.f);


	Direction.SetZero();
}

void CEntity::Retreat(vector<CEntity*> ListOfCharacters,Vector3 TargetDestination, double dt)
{
	// Get the direction
	Direction = (Position - TargetDestination);


	Vector3 Alignment = ComputeAlignment(ListOfCharacters);
	Vector3 Cohesion = ComputeCohesion(ListOfCharacters);
	Vector3 Seperation = ComputeSeperation(ListOfCharacters);

	Direction.Normalize() * m_RunSpeed;

	Direction += (Alignment  * m_alignmentWeight) + (Cohesion  * m_cohesionWeight) + (Seperation  * m_separationWeight);
	
	Position += Direction * m_RunSpeed * dt;

	//Position.x = Math::Wrap(Position.x, -150.f, 150.f);
	//Position.z = Math::Wrap(Position.z, -150.f, 150.f);
}

bool CEntity::SeekHealer(vector<CEntity*> ListOfCharacters)
{
	float theLength = 0.f;
	bool healerExists = false;
	for (int i = 0; i < ListOfCharacters.size(); ++i)
	{
		if (ListOfCharacters[i]->GetTYPE() == "HEALER")
		{
			float theCompare = (ListOfCharacters[i]->GetPosition() - Position).Length();
			if (theLength == 0.f)
			{
				this->HealPosition = ListOfCharacters[i]->GetPosition();
				theLength = theCompare;
				healerExists = true;
			}
			else if (theCompare < theLength)
			{
				this->HealPosition = ListOfCharacters[i]->GetPosition();
				theLength = theCompare;
				healerExists = true;
			}
		}
	}

	return healerExists;
}

void CEntity::Attack(void)
{

}

void CEntity::RunFSM(double dt, Vector3 newTargetPosition, Vector3 newDangerPosition)
{
	
}

void CEntity::RunFSM(double dt, vector<CEntity*> ListOfCharacters, Vector3 newTargetPosition, Vector3 newDangerPosition)
{
	
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

Vector3 CEntity::GetVelocity()
{
	return this->Direction;
}

Vector2 CEntity::seek(Vector2 target)
{
	Vector2 PositionCopy = Vector2(Position.x, Position.z);
	Vector2 direction = target - PositionCopy;
	if (!direction.IsZero())
	{
		direction = direction.Normalized();
		direction *= m_MoveSpeed;
	}

	Vector2 DirectionCopy = Vector2(Direction.x, Direction.z);
	Vector2 steer = direction - DirectionCopy;
	//Math::Clamp(steer.Length(), 0.f, maxforce);

	if (steer.Length() >= m_maxforce)
	{
		steer *= (m_maxforce / steer.Length());
	}

	return steer;
}

Vector3 CEntity::ComputeAlignment(vector<CEntity*> ListOfCharacters)
{
	Vector2 sum;
	int count = 0;

	if (this->TYPE == "BOSS")
	{
		//return Vector3(0, 0, 0);
	}

	for (int i = 0; i < ListOfCharacters.size(); ++i)
	{
		if (ListOfCharacters[i] != this && ListOfCharacters[i]->GetActive())
		//if (ListOfCharacters[i] != this && ListOfCharacters[i]->TYPE != "BOSS")
		{
			float distance = (Position - ListOfCharacters[i]->GetPosition()).Length();
			if (distance < m_flockZone)
			{
				Vector2 VelCopy = Vector2(ListOfCharacters[i]->GetVelocity().x, ListOfCharacters[i]->GetVelocity().z);
				sum += VelCopy;
				count++;
			}
		}
	}

	if (count > 0)
	{
		sum.x /= (float)count;
		sum.y /= (float)count;
		if (!sum.IsZero())
		{
			sum = sum.Normalized();
			sum *= m_MoveSpeed;
		}
		Vector2 DirectionCopy = Vector2(Direction.x, Direction.z);
		Vector2 steer = sum - DirectionCopy;

		if (steer.Length() >= m_maxforce)
		{
			steer *= (m_maxforce / steer.Length());
		}
		return Vector3(steer.x, 0, steer.y);

	}
	else
	{
		return Vector3(0, 0, 0);
	}
}

Vector3 CEntity::ComputeCohesion(vector<CEntity*> ListOfCharacters)
{
	Vector2 sum;
	int count = 0;

	if (this->TYPE == "BOSS")
	{
		//return Vector3(0, 0, 0);
	}

	for (int i = 0; i < ListOfCharacters.size(); ++i)
	{
		if (ListOfCharacters[i] != this && ListOfCharacters[i]->GetActive())
		//if (ListOfCharacters[i] != this && ListOfCharacters[i]->TYPE != "BOSS")
		{
			float distance = (Position - ListOfCharacters[i]->GetPosition()).Length();
			if (distance < m_flockZone)
			{
				Vector2 PositionCopy = Vector2(ListOfCharacters[i]->GetPosition().x, ListOfCharacters[i]->GetPosition().z);
				sum += PositionCopy;
				count++;
			}
		}
	}

	if (count > 0)
	{
		sum.x /= (float)count;
		sum.y /= (float)count;

		Vector2 theSeek = seek(sum);
		return Vector3(theSeek.x, 0, theSeek.y);
	}
	else
	{
		return Vector3(0, 0, 0);
	}
}

Vector3 CEntity::ComputeSeperation(vector<CEntity*> ListOfCharacters)
{
	Vector2 steer;
	steer.SetZero();

	int count = 0;

	if (this->TYPE == "BOSS")
	{
		//return Vector3(0, 0, 0);
	}

	for (int i = 0; i < ListOfCharacters.size(); ++i)
	{
		if (ListOfCharacters[i] != this && ListOfCharacters[i]->GetActive())
		//if (ListOfCharacters[i] != this && ListOfCharacters[i]->TYPE != "BOSS")
		{
			float distance = (Position - ListOfCharacters[i]->GetPosition()).Length();
			if (distance < m_seperationZone)
			{
				Vector3 DirectionCopy = Position - ListOfCharacters[i]->GetPosition();
				Vector2 diff = Vector2(DirectionCopy.x, DirectionCopy.z);
				if (!diff.IsZero())
				{
					diff = diff.Normalized();
					diff.x /= distance;
					diff.y /= distance;
				}
				steer += diff;
				count++;
			}
		}
	}

	if (count > 0)
	{
		steer.x /= (float)count;
		steer.y /= (float)count;
	}

	if (steer.Length() > 0)
	{

		//Math::Clamp(steer.Length(), maxspeed, maxspeed);
		steer = steer.Normalized();
		steer *= m_MoveSpeed;
		Vector2 DirectionCopy = Vector2(Direction.x, Direction.z);
		steer -= DirectionCopy;
		//Math::Clamp(steer.Length(), 0.f, maxforce);

		if (steer.Length() >= m_maxforce)
		{
			steer *= (m_maxforce / steer.Length());
		}
	}

	return Vector3(steer.x, 0, steer.y);
}

void CEntity::ComputeDangerPosition(vector<CEntity*> ListOfCharacters)
{
	for (int i = 0; i < ListOfCharacters.size(); ++i)
	{
		if (ListOfCharacters[i]->GetTYPE() == "BOSS")
		{
			if ((ListOfCharacters[i]->GetPosition() - Position).Length() < (DangerPosition - Position).Length())
			{
				this->DangerPosition = ListOfCharacters[i]->GetPosition();
			}	
		}
	}
}

bool CEntity::GetActive(void)
{
	return this->m_Active;
}

void CEntity::SetActive(bool TF)
{
	this->m_Active = TF;
}

bool CEntity::SeekDead(vector<CEntity*> ListOfCharacters)
{
	bool isDead = false;
	for (int i = 0; i < ListOfCharacters.size(); ++i)
	{
		if (ListOfCharacters[i]->GetTYPE() != "BOSS")
		{
			if (!ListOfCharacters[i]->GetActive())
			{
				this->TargetPosition = ListOfCharacters[i]->GetPosition();
				isDead = true;
			}
		}
	}

	return isDead;
}