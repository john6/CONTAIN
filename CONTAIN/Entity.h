#pragma once
#include <random>
#include <map>
#include <list>
#include "GLOBAL_CONSTANTS.h"
#include "RESOURCES.h"
#include "Math.h"
#include "Physics.h"
#include "RigidBody.h"
#include "BusNode.h"
#include "MessageBus.h"
#include "CollisionData.h"

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
	std::shared_ptr <sf::Texture> texturePtr;

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

	float textRectWidth;
	float textRectHeight;	
	float textStretchWidth;
	float textStretchHeight;

	Entity();
	Entity(Vector2f i_startPosition, RigidBody i_rb, TypeID i_typeID);
	~Entity();

	RigidBody rb;

	const bool MarkedForDeath();

	const TypeID GetTypeID();

	void CollideWith(CollisionData* i_collision);

	virtual void Update(float i_stepSize);

	virtual void Destroy();

	virtual drawablePtrVect CreateDrawables(float i_lerp_fraction);

	virtual std::shared_ptr<sf::Shape> CreateDrawableRB(float i_lerp_fraction);

	virtual void CollideWithPlayer(CollisionData i_collisionCopy);

	virtual void CollideWithProjectile(CollisionData i_collisionCopy);

	virtual void CollideWithBlocker(CollisionData i_collisionCopy);

	virtual void CollideWithEnemy(CollisionData i_collisionCopy);

	virtual void CollideWithWall(CollisionData i_collisionCopy);

	virtual void CollideWithPainWall(CollisionData i_collisionCopy);

	virtual void CollideWithDoor(CollisionData i_collisionCopy);

	virtual void CollideWithEndObject(CollisionData i_collisionCopy);

	virtual void CollideWithPowUp(CollisionData i_collisionCopy);

	virtual void CollideWithBlast(CollisionData i_collisionCopy);

	virtual void UpdateVisuals(float i_stepSize);
};