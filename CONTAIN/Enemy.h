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

public:
	std::shared_ptr<Entity> charPtr;
	float speed;
	float stunSecs;
	bool metal;

	Enemy(DIFFICULTY i_diff, Vector2f i_startPosition, RigidBody i_rb, TypeID i_typeID = ENEMY_SEEK);
	~Enemy();

	void Update(float i_stepSize) override;

	Vector2f CreateRandomDir();

	void Destroy() override;

	void CollideWithPainWall(PainWall * i_painWallPtr) override;

	void CollideWithPlayer(PlayerChar* i_playerPtr) override;

	void CollideWithBlast(Blast* i_blastPtr) override;

	void CollideWithProjectile(Projectile* i_projPtr) override;

	void DropPowerUp();

	void UpdateHealth(float i_stepSize);

	virtual void TakeDamage(float i_dmg);

	virtual void Stun(float i_stunTime);

	virtual void SetDiffVars(int i_diff);

	void ChangeColorHealth();

	void TurnToMetal();
};
