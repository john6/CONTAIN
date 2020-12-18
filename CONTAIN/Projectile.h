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

	void CollideWithPlayer(PlayerChar* i_playerPtr) override;

	void CollideWithProjectile(Projectile* i_projPtr) override;

	void CollideWithBlocker(Blocker* i_blockPtr) override;

	void CollideWithEnemy(Enemy* i_enemyPtr, CollisionData i_collision) override;

	void CollideWithWall(Wall* i_wallPtr) override;

	void CollideWithPainWall(PainWall * i_painWallPtr) override;

	void Explode();

	void CollideWithDoor(Door* i_doorPtr) override;
};
