#pragma once
#include "GLOBAL_CONSTANTS.h"
#include "RigidBody.h"
#include "PlayerController.h"
#include <random>
#include "Math.h"
#include <map>

class Game;
class Level;
class Sector;
class PlayerChar;
class Projectile;
class Blocker;
class Enemy;
class Door;
class Wall;
class EndObject;
class PainWall;
class PowerUp;
class Blast;
class CrazyBoi;
class BossBurst;
class BossStream;
class BossRush;
class BossSpawn;
class BossSplit;
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

	virtual void CollideWithBlocker(Blocker* i_blockPtr);

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
	float dmgRate;
	hiRes_time_point lastDamageReceived;
	float BlastRadius;
	float blastStrength;
	float blastStunTime;
	float wallWidth;
	float wallHeight;
	std::map<UPGRADE_TYPE, int> shipLvl;
	Rectangle* rectPtr;



public:
	hiRes_time_point lastAOEFired;
	float shipRateOfAOE;

	int maxHealth;
	int health;

	float weaponDelay;
	int maxSpecialAmmo;
	int currSpecialAmmo;
	int numShots;
	float weapSpeed;

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

	void InitLvl();

	std::map<UPGRADE_TYPE, int>* GetLvl();

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

	void CollideWithBlocker(Blocker* i_blockPtr) override;

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
	PlayerChar* charP;

public:
	int blastType;
	float deathTimer;
	float strength;
	float stunTime;

	Blast(Sector* i_sectPtr, PlayerChar* charP, Vector2f i_startPosition, int i_blastType, float i_strength, float i_stunTime,
			RigidBody i_rb = RigidBody(std::make_shared<Circle>(150.0f), STATIC));
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

	Enemy(std::shared_ptr<Entity> i_charPtr, Sector* i_sectPtr, DIFFICULTY i_diff, Vector2f i_startPosition, RigidBody i_rb, TypeID i_typeID = ENEMY_SEEK);
	~Enemy();

	void Update(float i_stepSize) override;

	Vector2f CreateRandomDir();

	void Destroy() override;

	void CollideWithPainWall(PainWall * i_painWallPtr) override;

	void CollideWithPlayer(PlayerChar* i_playerPtr) override;

	void CollideWithBlast(Blast* i_blastPtr) override;

	void CollideWithProjectile(Projectile* i_projPtr) override;

	void UpdateHealth(float i_stepSize);

	virtual void TakeDamage(float i_dmg);

	virtual void Stun(float i_stunTime);

	virtual void SetDiffVars(int i_diff);

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
	CrazyBoi(std::shared_ptr<Entity> i_charPtr, Sector* i_sectPtr, DIFFICULTY i_diff, Vector2f i_startPosition, RigidBody i_rb);
	~CrazyBoi();

	void Update(float i_stepSize) override;

	void CollideWithPainWall(PainWall * i_painWallPtr) override;

	void SetDiffVars(int i_diff) override;
};

class BossBurst : public Enemy
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
	BossBurst(std::shared_ptr<Entity> i_charPtr, Sector* i_sectPtr, DIFFICULTY i_diff, Vector2f i_startPosition, RigidBody i_rb);

	void Update(float i_stepSize) override;

	void Destroy() override;

	void TakeDamage(float i_dmg);

	void shootProj();

	void Stun(float i_stunTime) override;

	void SetDiffVars(int i_diff, int i_lvlNum);
};

class BossStream : public Enemy
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
	BossStream(std::shared_ptr<Entity> i_charPtr, Sector* i_sectPtr, DIFFICULTY i_diff, Vector2f i_startPosition, RigidBody i_rb);

	void Update(float i_stepSize) override;

	void Destroy() override;

	void TakeDamage(float i_dmg);

	void shootProj();

	void Stun(float i_stunTime) override;

	void SetDiffVars(int i_diff, int i_lvlNum);
};

class BossRush : public Enemy
{
private:
	Vector2f currDir;
	bool rushing;
	float timeTillSwitch;
	float rushTime;
	float waitTime;
	float numShots;
	hiRes_time_point lastShotFired;
	float shipRateOfFire;
	float weaponDelay;

public:
	BossRush(std::shared_ptr<Entity> i_charPtr, Sector* i_sectPtr, DIFFICULTY i_diff, Vector2f i_startPosition,
		RigidBody i_rb = RigidBody(std::make_shared<Circle>(75), LESSBOUNCYBALL));

	void Update(float i_stepSize) override;

	void Destroy() override;

	void TakeDamage(float i_dmg);

	void shootProj();

	void Stun(float i_stunTime) override;

	void SetDiffVars(int i_diff, int i_lvlNum);

	void CollideWithPainWall(PainWall * i_painWallPtr) override;

	void CollideWithPlayer(PlayerChar* i_playerPtr) override;
};

class BossSplit : public Enemy
{
private:
	bool crazy;
	int splitsLeft;
	DIFFICULTY diff;
	Vector2f currDir;
	float sameDirTime;
	float timeTillDirSwitch;
	float numShots;
	hiRes_time_point lastShotFired;
	float shipRateOfFire;
	float weaponDelay;

public:
	BossSplit(std::shared_ptr<Entity> i_charPtr, Sector* i_sectPtr, DIFFICULTY i_diff, int i_splitsLeft, float i_spdFct, bool i_crazy,
			Vector2f i_startPosition, RigidBody i_rb = RigidBody(std::make_shared<Circle>(150), BOUNCYBALL));

	void Update(float i_stepSize) override;

	void Destroy() override;

	void TakeDamage(float i_dmg);

	void shootProj();

	void Stun(float i_stunTime) override;

	void SetDiffVars(int i_diff, int i_lvlNum);
};

class BossSpawn : public Enemy
{
private:
	DIFFICULTY diff;
	Vector2f currDir;
	float sameDirTime;
	float timeTillDirSwitch;
	float numShots;
	hiRes_time_point lastShotFired;
	float shipRateOfFire;
	float weaponDelay;

public:
	bool invulnerable;

	BossSpawn(std::shared_ptr<Entity> i_charPtr, Sector* i_sectPtr, DIFFICULTY i_diff, Vector2f i_startPosition,
		RigidBody i_rb = RigidBody(std::make_shared<Rectangle>(100, 100), STATIC));

	void Update(float i_stepSize) override;

	void Destroy() override;

	void TakeDamage(float i_dmg);

	void shootProj();

	void Stun(float i_stunTime) override;

	void SetDiffVars(int i_diff, int i_lvlNum);
};


class Door :
	public Entity
{
private:
	Sector* sectPtr;
	Vector2f outPos;
	MapCoord outCoord;

public:
	SCREEN_SIDE side;

	Door(Sector* i_sectPtr, MapCoord i_outCoord, Vector2f i_startPos, Vector2f i_outPos,
		RigidBody i_rb, SCREEN_SIDE i_side);
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
	Wall(Vector2f i_startPosition, Sector* i_sectPtr, RigidBody i_rb,
		sf::Color i_fillCol = sf::Color::Black, sf::Color i_outCol = sf::Color::White);
	~Wall();
};

class PainWall :
	public Entity
{
private:
	float timeSinceColorSwitch;
	float colorSwitchRate;
	bool colorState;
	Sector* sectPtr;

public:

	void Update(float i_stepSize) override;

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