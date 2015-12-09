#include "Particle.h"

CParticle::CParticle()
{

}
CParticle::~CParticle()
{
	if (mesh)
	{
		delete mesh;
		mesh = NULL;
	}
	if (trackingTarget)
	{
		delete trackingTarget;
		trackingTarget = NULL;
	}
}

CParticle::CParticle(Vector3 position, Vector3 targetPosition, bool m_Active = false, float speed = 0.f)
{
	this->mesh = NULL;
	this->trackingTarget = NULL;
	this->position = position;
	this->targetPosition = targetPosition;
	this->m_Active = m_Active;
	this->speed = speed;
	this->direction.SetZero();
	scale = 1.f;
	scaleTimer = 0.f;
}

CParticle::CParticle(Vector3 position, CEntity* target, bool m_Active = false, float speed = 0.f)
{
	this->mesh = NULL;
	this->trackingTarget = target;
	this->position = position;
	this->targetPosition.SetZero();
	this->m_Active = m_Active;
	this->speed = speed;
	this->direction.SetZero();
	scale = 1.f;
	scaleTimer = 0.f;
}

void CParticle::Update(double dt)
{
	if (m_Active)
	{
		float totalspeed = (speed * dt);
		
		if (trackingTarget)
		{
			direction = (trackingTarget->GetPosition() - position).Normalized();
			targetPosition = trackingTarget->GetPosition();
		}
		else
		{
			direction = (targetPosition - position).Normalized();
		}

		if (scaleTimer <= 0)
			position += direction * totalspeed;

		if ((position - targetPosition).Length() <  scale + totalspeed)
			m_Active = false;

		if (scaleTimer > 0)
		{
			scale +=  dt;
			scaleTimer -= dt;
		}
	}
}

