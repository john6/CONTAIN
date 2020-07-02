#pragma once
#include "GLOBAL_CONSTANTS.h"
#include "RigidBody.h"
#include "PlayerController.h"

class Game;
class Level;
class Sector;
class PlayerChar;
class Projectile;
class Enemy;
class Door;
class Wall;
class EndObject;

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

	void CollideWith(Entity& i_other);

	virtual void Update(float i_stepSize);

	virtual void Destroy();

	std::unique_ptr<sf::Shape> CreateDrawable(float i_lerp_fraction);

	virtual void CollideWithPlayer(PlayerChar* i_playerPtr);

	virtual void CollideWithProjectile(Projectile* i_projPtr);

	virtual void CollideWithEnemy(Enemy* i_enemyPtr);

	virtual void CollideWithWall(Wall* i_wallPtr);

	virtual void CollideWithDoor(Door* i_doorPtr);

	virtual void CollideWithEndObject(EndObject* i_endPtr);
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

	void Update(float i_stepSize) override;

	void UpdateMovement(float i_stepSize);

	void UpdateHealth(float i_stepSize);

	void TakeDamage(float i_dmg);

	float GetCurrHealth();

	void CollideWithDoor(Door* i_doorPtr) override;

	void CollideWithEndObject(EndObject* i_endPtr) override;
};

class Projectile :
	public Entity
{
private:
	Sector* lvlPtr;

public:
	Projectile(RigidBody i_rb, Vector2f i_startPosition);
	~Projectile();

	void Update(float i_stepSize) override;

	void Destroy() override;

	void CollideWithPlayer(PlayerChar* i_playerPtr) override;

	void CollideWithProjectile(Projectile* i_projPtr) override;

	void CollideWithEnemy(Enemy* i_enemyPtr) override;

	void CollideWithWall(Wall* i_wallPtr) override;

	void CollideWithDoor(Door* i_doorPtr) override;
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

	void Update(float i_stepSize) override;

	void Destroy() override;

	void CollideWithPlayer(PlayerChar* i_playerPtr) override;
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

	const MapCoord GetOutCoord();

	const Vector2f GetOutPos();

	void Update(float i_stepSize) override;
};

class Wall :
	public Entity
{
private:
	Sector* sectPtr;

public:
	Wall(RigidBody i_rb, Vector2f i_startPosition, Sector* i_sectPtr);
	~Wall();
};

class EndObject :
	public Entity
{
private:
	Sector* sectPtr;

public:
	EndObject(RigidBody i_rb, Vector2f i_startPosition, Sector* i_sectPtr);
	~EndObject();
};