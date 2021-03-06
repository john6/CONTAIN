#pragma once
#include "Entity.h"

class Sector;
class PlayerChar;
class Blast;
class Projectile;

class Enemy :
	public Entity
{
protected:
	float maxHealth;
	float health;
	sf::Color origColorFill;
	sf::Color origColorOutLine;
	sf::Color deathColorFill;
	sf::Color deathColorOutLine;
	sf::Color zombieColorFill;
	sf::Color zombieColorOutLine;

	//visuals
	float tendrilSize;
	Polygon trianglePoly;
	float tendrilRotation;
	Vector2f lastDmgProjDir;
	Vector2f lastColl_dir;
	float lastDmgProjStrength;

public:
	std::shared_ptr<Entity> charPtr;
	float speed;
	float stunSecs;
	bool metal;
	bool isBoss;

	Enemy(DIFFICULTY i_diff, Vector2f i_startPosition, RigidBody i_rb, TypeID i_typeID = ENEMY_SEEK);
	~Enemy();

	void Update(float i_stepSize) override;

	Vector2f CreateRandomDir();

	void Destroy() override;

	void GenerateDeathEffects(ANIMTYPE animType, CollisionData i_coll = CollisionData());

	void CollideWithPainWall(CollisionData i_coll) override;

	void CollideWithProjectile(CollisionData i_coll) override;

	void UpdateVisuals(float i_stepSize) override;

	void UpdateTendrilPosition(float i_stepSize);

	void GenerateDamageEffects(CollisionData i_collisionCopy);

	void DropPowerUp();

	void UpdateHealth(float i_stepSize);

	virtual void TakeDamage(float i_dmg, CollisionData i_coll);

	virtual void Stun(float i_stunTime);

	virtual void SetDiffVars(int i_diff);

	drawablePtrVect CreateDrawables(float i_lerp_fraction) override;

	void ChangeColorHealth();

	void TurnToMetal();

};
