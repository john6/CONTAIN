#pragma once
#include "PlayerController.h"
#include "Entity.h"


class Game;
class PainWall;
class Door;
class EndObject;
class PowerUp;
class Projectile;

class PlayerChar :
	public Entity
{
private:
	PlayerController pController;
	hiRes_time_point lastShotFired;
	float shipRateOfFire;
	hiRes_time_point lastWallFired;
	float shipRateOfFireWall;
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
	float wallDelay;
	int maxSpecialAmmo;
	int currSpecialAmmo;
	int numShots;
	float weapSpeed;

	PlayerChar(int i_strtHealth, Vector2f i_startPosition, 
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

	void TakeDamage(float i_dmg, CollisionData i_coll);

	void TakeDamage(float i_dmg);

	void GenerateDamageEffects(CollisionData i_collisionCopy);

	void ReceivePowerUp(UPGRADE_TYPE i_powType);

	float GetCurrHealth();

	void CollideWithEnemy(CollisionData i_coll) override;

	void CollideWithPainWall(CollisionData i_coll) override;

	void CollideWithDoor(CollisionData i_coll) override;

	void CollideWithEndObject(CollisionData i_coll) override;

	void CollideWithPowUp(CollisionData i_coll) override;

	void CollideWithProjectile(CollisionData i_coll) override;

	void UpdateVisuals(float i_stepSize) override;

	void ShootBasic(Vector2f i_mousePos);

	void ShootWall(Vector2f i_mousePos);

	void InitLvl();

	std::map<UPGRADE_TYPE, int>* GetLvl();

	void ShootAOE();
};


