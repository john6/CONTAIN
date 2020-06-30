#pragma once
#include "GLOBAL_CONSTANTS.h"
#include "RigidBody.h"
#include "PlayerController.h"

class Game;
class Level;
class Sector;

class Entity
{
//enum EntityGroup { PlayerChar, Enemy, Projectile, Wall, Door };

private:

protected:
	sf::Color fillColor;
	sf::Color outlineColor;
	bool killMeNextLoop;

public:
	Entity();
	Entity(RigidBody i_rb, Vector2f i_startPosition);
	~Entity();

	RigidBody rb;

	const bool MarkedForDeath();

	virtual void CollideWith(Entity& i_other) = 0;

	virtual void Update(float i_stepSize) = 0;

	virtual void Destroy() = 0;

	std::unique_ptr<sf::Shape> CreateDrawable(float i_lerp_fraction);
};

class PlayerChar :
	public Entity
{
private:
	int health;
	Game* gamePtr;
	PlayerController pController;
	hiRes_time_point lastShotFired;
	int shipRateOfFire;
	float shipSpeed;

	float dmgRate;
	hiRes_time_point lastDamageReceived;

public:

	PlayerChar(RigidBody i_rb, Vector2f i_startPosition, Game* gamePtr);
	~PlayerChar();

	void CollideWith(Entity& i_other) override;

	void Update(float i_stepSize) override;

	void UpdateMovement(float i_stepSize);

	void UpdateHealth(float i_stepSize);

	void TakeDamage(float i_dmg);

	void Destroy() override;
};

class Projectile :
	public Entity
{
private:
	Sector* lvlPtr;

public:
	Projectile(RigidBody i_rb, Vector2f i_startPosition);
	~Projectile();

	void CollideWith(Entity& i_ent) override;

	void Update(float i_stepSize) override;

	void Destroy() override;
};

class Enemy :
	public Entity
{
private:
	Sector* sectPtr;

public:
	std::shared_ptr<Entity> charPtr;
	float speed;

	Enemy(RigidBody i_rb, Vector2f i_startPosition, std::shared_ptr<Entity> i_charPtr, Sector* i_sectPtr);
	~Enemy();

	void CollideWith(Entity& i_other) override;

	void Update(float i_stepSize) override;

	void Destroy() override;
};

class Wall :
	public Entity
{
private:
	Sector* sectPtr;

public:
	Wall(RigidBody i_rb, Vector2f i_startPosition, Sector* i_sectPtr);
	~Wall();

	void CollideWith(Entity& i_other) override;

	void Update(float i_stepSize) override;

	void Destroy() override;
};

class Door :
	public Entity
{
private:
	Sector* sectPtr;
 	Vector2f outPos;
	MapCoord outCoord;

public:
	Door(RigidBody i_rb, Vector2f i_startPosition, Sector* i_sectPtr, MapCoord i_toSect, Vector2f i_outCoord);
	~Door();

	bool open;

	void CollideWith(Entity& i_other) override ;

	const MapCoord GetOutCoord();

	const Vector2f GetOutPos();

	void Update(float i_stepSize) override;

	void Destroy() override;
};