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
}

CParticle::CParticle(Vector3 position, Vector3 targetPosition, bool m_Active = false, float speed = 0.f)
{
	
	this->position = position;
	this->targetPosition = targetPosition;
	this->m_Active = m_Active;
	this->speed = speed;
}

void CParticle::Update(double dt)
{
	if (m_Active)
	{
		Vector3 direction = (targetPosition - position).Normalized();

		position += direction * (speed * dt);

		if ((position - targetPosition).Length() < 3)
			m_Active = false;
	}
}

