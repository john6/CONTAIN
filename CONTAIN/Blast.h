#pragma once
#include "Entity.h"


class Blast : public Entity
{
private:
	std::shared_ptr<Entity> charP;

public:
	int blastType;
	float deathTimerMax;
	float deathTimer;
	float blastRadius;
	float strength;
	float stunTime;

	Blast(Vector2f i_startPosition, int i_blastType, float i_strength, float i_stunTime, float i_blastRadius);
	~Blast();

	void Update(float i_stepSize) override;

	void CollideWithEnemy(CollisionData i_coll) override;

	void CollideWithWall(CollisionData i_coll) override;

	void UpdateVisuals(float i_stepSize) override;
};