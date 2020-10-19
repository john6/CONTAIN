#pragma once
#include "Entity.h"


class Blast : public Entity
{
private:
	std::shared_ptr<Entity> charP;

public:
	int blastType;
	float deathTimer;
	float strength;
	float stunTime;

	Blast(Vector2f i_startPosition, int i_blastType, float i_strength, float i_stunTime,
		RigidBody i_rb = RigidBody(std::make_shared<Circle>(150.0f), STATIC));
	~Blast();

	void Update(float i_stepSize) override;
};