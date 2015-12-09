#pragma once
#include "Vector3.h"
#include "Entity.h"
#include "../Mesh.h"

class CParticle
{
public:
	CParticle();
	CParticle(Vector3 position, Vector3 targetPosition, bool m_Active, float speed);
	CParticle(Vector3 position, CEntity* target, bool m_Active, float speed);
	~CParticle();

	void Update(double dt);

	Mesh* mesh;
	CEntity* trackingTarget;
	Vector3 position;
	Vector3 targetPosition;
	Vector3 direction;
	float speed;
	bool m_Active;
	float scale;
	float scaleTimer;
};