#pragma once
#include "Entity.h"

class Sector;
class PlayerChar;
class Blocker;

class Projectile :
	public Entity
{
public:
	int projType;

	Projectile(Vector2f i_startPosition, int i_projType = 0, RigidBody i_rb = RigidBody(std::make_shared<Circle>(GLBVRS::PROJECTILE_RADIUS), HEAVYBALL));
	~Projectile();
	//projtype 0 is sent from the player,
	//projType 1 is sent from an enemy

	void Update(float i_stepSize) override;

	void Destroy() override;

	void CollideWithPlayer(CollisionData i_coll) override;

	void CollideWithProjectile(CollisionData i_coll) override;

	void CollideWithBlocker(CollisionData i_coll) override;

	void CollideWithEnemy(CollisionData i_coll) override;

	void CollideWithWall(CollisionData i_coll) override;

	void CollideWithPainWall(CollisionData i_coll) override;

	void Explode();

	void CollideWithDoor(CollisionData i_coll) override;
};
