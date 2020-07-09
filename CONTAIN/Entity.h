#pragma once
#include "GLOBAL_CONSTANTS.h"
#include "RigidBody.h"
#include "PlayerController.h"
#include <random>
#include "Math.h"

class Game;
class Level;
class Sector;
class PlayerChar;
class Projectile;
class Enemy;
class Door;
class Wall;
class EndObject;
class PainWall;
class PowerUp;
class Blast;
class CrazyBoi;

class Entity
{
//enum EntityGroup { PlayerChar, Enemy, Projectile, Wall, Door };

private:

protected:
	sf::Color fillColor;
	sf::Color outlineColor;
	bool killMeNextLoop;

public:
	bool intangible;
	bool fixedPosition;

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

	virtual void CollideWithPainWall(PainWall * i_painWallPtr);

	virtual void CollideWithDoor(Door* i_doorPtr);

	virtual void CollideWithEndObject(EndObject* i_endPtr);

	virtual void CollideWithPowUp(PowerUp* i_powUpPtr);

	virtual void CollideWithBlast(Blast* i_blastPtr);
};

class PlayerChar :
	public Entity
{
private:
	Game* gamePtr;
	PlayerController pController;
	hiRes_time_point lastShotFired;
	float shipRateOfFire;
	float shipSpeed;
	int maxHealth;
	int health;
	float dmgRate;
	hiRes_time_point lastDamageReceived;

public:
	float weaponDelay;
	int specialAmmo;
	int numShots;

	PlayerChar(RigidBody i_rb, Vector2f i_startPosition, Game* gamePtr, int i_strtHealth);
	~PlayerChar();

	void Update(float i_stepSize) override;

	void UpdateMovement(float i_stepSize);

	void UpdateHealth(float i_stepSize);

	void ResetHealth();

	void AddHealth(int i_healthUp);

	void AcceptWeaponInput(float i_stepSize);

	void TakeDamage(float i_dmg);

	float GetCurrHealth();

	void CollideWithPainWall(PainWall * i_painWallPtr) override;

	void CollideWithDoor(Door* i_doorPtr) override;

	void CollideWithEndObject(EndObject* i_endPtr) override;

	void CollideWithPowUp(PowerUp* i_powUpPtr) override;

	void ShootBasic(Vector2f i_mousePos);

	void ShootWall(Vector2f i_mousePos);

	void ShootAOE();
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

	void CollideWithPainWall(PainWall * i_painWallPtr) override;

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
	float stunSecs;


	Enemy(RigidBody i_rb, Vector2f i_startPosition, std::shared_ptr<Entity> i_charPtr, Sector* i_sectPtr);
	~Enemy();

	void Update(float i_stepSize) override;

	void Destroy() override;

	void CollideWithPainWall(PainWall * i_painWallPtr) override;

	void CollideWithPlayer(PlayerChar* i_playerPtr) override;

	void CollideWithBlast(Blast* i_blastPtr) override;

	void Stun(float i_stunTime);
};

class CrazyBoi :
	public Enemy
{
private:
	Sector* sectPtr;
	Vector2f currDir;
	float sameDirTime;
	float timeTillDirSwitch;

public:
	CrazyBoi(RigidBody i_rb, Vector2f i_startPosition, std::shared_ptr<Entity> i_charPtr, Sector* i_sectPtr);
	~CrazyBoi();

	Vector2f CreateRandomDir();

	void Update(float i_stepSize) override;

	void CollideWithPainWall(PainWall * i_painWallPtr) override;
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

class PainWall :
	public Entity
{
private:
	Sector* sectPtr;

public:
	PainWall(RigidBody i_rb, Vector2f i_startPosition, Sector* i_sectPtr);
	~PainWall();
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

class PowerUp :
	public Entity
{
private:
	Sector* sectPtr;

public:
	int powType;

	PowerUp(Sector * i_sectPtr, int i_powType, RigidBody i_rb, Vector2f i_startPosition);
	~PowerUp();

	void Update(float i_stepSize) override;
};

class Blocker : public Entity 
{
private:
	Sector* sectPtr;

public:
	Blocker(RigidBody i_rb, Vector2f i_startPosition);
	~Blocker();
};

class Blast : public Entity
{
private:
	Sector* sectPtr;

public:
	int blastType;
	float deathTimer;

	Blast(Sector* i_sectPtr, int i_blastType, RigidBody i_rb, Vector2f i_startPosition);
	~Blast();

	void Update(float i_stepSize) override;
};