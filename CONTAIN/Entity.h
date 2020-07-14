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
class ShootyBoi;
//class SpawnyBoi;
//class ChargyBoi;

class Entity
{
protected:
	sf::Color fillColor;
	sf::Color outlineColor;
	bool killMeNextLoop;
	const TypeID typeID;

public:
	bool intangible;
	bool fixedPosition;

	Entity();
	Entity(Vector2f i_startPosition, RigidBody i_rb, TypeID i_typeID);
	~Entity();

	RigidBody rb;

	const bool MarkedForDeath();

	const TypeID GetTypeID();

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
	float BlastRadius;
	float wallWidth;
	float wallHeight;

public:
	float weaponDelay;
	int maxSpecialAmmo;
	int currSpecialAmmo;
	int numShots;

	PlayerChar(Game* i_gamePtr, int i_strtHealth, Vector2f i_startPosition, 
		RigidBody i_rb = RigidBody(std::make_shared<Rectangle>(100.0f, 100.0f), METAL));
	~PlayerChar();

	void Update(float i_stepSize) override;

	void UpdateMovement(float i_stepSize);

	void UpdateHealth(float i_stepSize);

	void ResetHealth();

	void ResetSpecialAmmo();

	void AddSpecialAmmo(int i_ammo);

	void AddHealth(int i_healthUp);

	void AcceptWeaponInput(float i_stepSize);

	void TakeDamage(float i_dmg);

	void ReceivePowerUp(UPGRADE_TYPE i_powType);

	float GetCurrHealth();

	void CollideWithPainWall(PainWall * i_painWallPtr) override;

	void CollideWithDoor(Door* i_doorPtr) override;

	void CollideWithEndObject(EndObject* i_endPtr) override;

	void CollideWithPowUp(PowerUp* i_powUpPtr) override;

	void CollideWithProjectile(Projectile* i_projPtr) override;

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
	int projType;

	Projectile(Vector2f i_startPosition, int i_projType = 0, RigidBody i_rb = RigidBody(std::make_shared<Circle>(PROJECTILE_RADIUS), HEAVYBALL));
	~Projectile();
	//projtype 0 is sent from the player,
	//projType 1 is sent from an enemy

	void Update(float i_stepSize) override;

	void Destroy() override;

	void CollideWithPlayer(PlayerChar* i_playerPtr) override;

	void CollideWithProjectile(Projectile* i_projPtr) override;

	void CollideWithEnemy(Enemy* i_enemyPtr) override;

	void CollideWithWall(Wall* i_wallPtr) override;

	void CollideWithPainWall(PainWall * i_painWallPtr) override;

	void CollideWithDoor(Door* i_doorPtr) override;
};

class Blocker : public Entity
{
private:
	Sector* sectPtr;

public:
	Blocker(Vector2f i_startPosition, RigidBody i_rb = RigidBody(std::make_shared<Rectangle>(40, 270), HEAVYBALL));
	~Blocker();
};

class Blast : public Entity
{
private:
	Sector* sectPtr;

public:
	int blastType;
	float deathTimer;

	Blast(Sector* i_sectPtr, Vector2f i_startPosition, int i_blastType, RigidBody i_rb = RigidBody(std::make_shared<Circle>(175.0f), STATIC));
	~Blast();

	void Update(float i_stepSize) override;
};

class Enemy :
	public Entity
{
private:

protected:
	Sector* sectPtr;
	float maxHealth;
	float health;


public:
	std::shared_ptr<Entity> charPtr;
	float speed;
	float stunSecs;
	bool metal;

	Enemy(std::shared_ptr<Entity> i_charPtr, Sector* i_sectPtr, Vector2f i_startPosition, RigidBody i_rb, TypeID i_typeID = ENEMY_SEEK);
	~Enemy();

	void Update(float i_stepSize) override;

	Vector2f CreateRandomDir();

	void Destroy() override;

	void CollideWithPainWall(PainWall * i_painWallPtr) override;

	void CollideWithPlayer(PlayerChar* i_playerPtr) override;

	void CollideWithBlast(Blast* i_blastPtr) override;

	void CollideWithProjectile(Projectile* i_projPtr) override;

	void UpdateHealth(float i_stepSize);

	void TakeDamage(float i_dmg);

	void Stun(float i_stunTime);

	void TurnToMetal();
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
	CrazyBoi(std::shared_ptr<Entity> i_charPtr, Sector* i_sectPtr, Vector2f i_startPosition, RigidBody i_rb);
	~CrazyBoi();

	void Update(float i_stepSize) override;

	void CollideWithPainWall(PainWall * i_painWallPtr) override;
};

class ShootyBoi : public Enemy
{
private:
	Vector2f currDir;
	float sameDirTime;
	float timeTillDirSwitch;
	float numShots;
	hiRes_time_point lastShotFired;
	float shipRateOfFire;
	float weaponDelay;

public:
	ShootyBoi(std::shared_ptr<Entity> i_charPtr, Sector* i_sectPtr, Vector2f i_startPosition, RigidBody i_rb);

	void Update(float i_stepSize) override;

	void shootProj();
};

class Door :
	public Entity
{
private:
	Sector* sectPtr;
	Vector2f outPos;
	MapCoord outCoord;

public:
	Door(Sector* i_sectPtr, MapCoord i_outCoord, Vector2f i_startPos, Vector2f i_outPos, RigidBody i_rb );
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
	Wall(Vector2f i_startPosition, Sector* i_sectPtr, RigidBody i_rb);
	~Wall();
};

class PainWall :
	public Entity
{
private:
	Sector* sectPtr;

public:
	PainWall(Vector2f i_startPosition, Sector* i_sectPtr, RigidBody i_rb);
	~PainWall();
};

class EndObject :
	public Entity
{
private:
	Sector* sectPtr;

public:
	bool active;

	EndObject(Sector* i_sectPtr, Vector2f i_startPosition, RigidBody i_rb = 
		RigidBody(std::make_shared<Rectangle>(100.0f, 100.0f), STATIC));
	~EndObject();


	void Update(float i_stepSize) override;
};

class PowerUp :
	public Entity
{
private:
	Sector* sectPtr;

public:
	UPGRADE_TYPE powType;

	PowerUp(Sector * i_sectPtr, Vector2f i_startPosition, UPGRADE_TYPE i_powType, RigidBody i_rb =
		RigidBody(std::make_shared<Rectangle>(100.0f, 100.0f), STATIC));
	~PowerUp();

	void Update(float i_stepSize) override;
};