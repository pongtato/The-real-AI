#pragma once
#include "Vector3.h"
#include "../Mesh.h"

class CParticle
{
public:
	CParticle();
	CParticle(Vector3 position, Vector3 targetPosition, bool m_Active, float speed);
	~CParticle();

	void Update(double dt);

	Mesh* mesh;
	Vector3 position;
	Vector3 targetPosition;
	float speed;
	bool m_Active;
};