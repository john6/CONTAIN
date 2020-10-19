#pragma once
#include <random>
#include <map>
#include "GLOBAL_CONSTANTS.h"
#include "RESOURCES.h"
#include "Math.h"
#include "Physics.h"
#include "RigidBody.h"
#include "BusNode.h"
#include "MessageBus.h"

class Game;
class Level;
class Sector;

class PlayerChar;
class Projectile;
class Blocker;
class Door;
class Wall;
class EndObject;
class PainWall;
class PowerUp;
class Blast;

class Enemy;
class CrazyBoi;
class BossBurst;
class BossStream;
class BossRush;
class BossSpawn;
class BossSplit;
class Anim;
class Scenery;

class Entity
{
protected:
	bool killMeNextLoop;
	const TypeID typeID;

public:

	std::list<std::shared_ptr<Entity>> spawnVect;
	sf::Color fillColor;
	sf::Color outlineColor;
	bool intangible;
	bool fixedPosition;
	bool physicalObject;
	bool canCollide;
	bool hasVisuals;
	float borderThickness;
	drawablePtrVect drawables;
	drawablePtrVect visuals;

	Entity();
	Entity(Vector2f i_startPosition, RigidBody i_rb, TypeID i_typeID);
	~Entity();

	RigidBody rb;

	const bool MarkedForDeath();

	const TypeID GetTypeID();

	void CollideWith(Entity& i_other);

	virtual void Update(float i_stepSize);

	virtual void Destroy();

	virtual drawablePtrVect CreateDrawables(float i_lerp_fraction);

	virtual std::shared_ptr<sf::Shape> CreateDrawableRB(float i_lerp_fraction);

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

	virtual void UpdateVisuals(float i_stepSize);
};