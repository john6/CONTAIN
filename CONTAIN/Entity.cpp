//I have to include a bunch of header files or else I get the circular header file reference issue,
// because my Entities and Sector and Level and Game all reference each other whoopsidasies
#include "Entity.h"

#include "Projectile.h"
#include "Blast.h"
#include "PowerUp.h"
#include "PlayerChar.h"

//#include "Anim.h"
//#include "Blast.h"
//#include "Blocker.h"
//#include "BossRush.h"
//#include "BossSpawn.h"
//#include "BossSplit.h"
//#include "BossStream.h"
//#include "CrazyBoi.h"
//#include "Door.h"
//#include "EndObject.h"
//#include "Enemy.h"
//#include "PainWall.h"
//#include "PlayerChar.h"
//#include "PowerUp.h"
//#include "Projectile.h"
//#include "Scenery.h"
//#include "Wall.h"


Entity::Entity() :
	rb{ RigidBody(std::make_shared<Rectangle>(100.0f, 100.0f)) },
	typeID{ ENTITY_VIRTUAL }
{
	borderThickness = 3.0f;
	drawables = std::make_shared<std::vector<std::shared_ptr<sf::Drawable>>>();
	visuals = std::make_shared<std::vector<std::shared_ptr<sf::Drawable>>>();
	Material defMaterial(0.6f, 0.1f, 0.6f, 0.3f);
	rb.mat = defMaterial;
	rb.SetMassData();
	fillColor = sf::Color::Black;
	outlineColor = sf::Color::White;
	fixedPosition = false;
	canCollide = true;
	physicalObject = true;
	hasVisuals = false;
}

Entity::Entity(Vector2f i_startPosition, RigidBody i_rb, TypeID i_typeID) :
	rb{ i_rb }, typeID{ i_typeID }
{
	borderThickness = 3.0f;
	drawables = std::make_shared<std::vector<std::shared_ptr<sf::Drawable>>>();
	visuals = std::make_shared<std::vector<std::shared_ptr<sf::Drawable>>>();
	fillColor = sf::Color::Black;
	outlineColor = sf::Color::White;
	killMeNextLoop = false;
	rb.ResetPosition(i_startPosition);
	intangible = false;
	physicalObject = true;
	canCollide = true;
	hasVisuals = false;
}

const bool Entity::MarkedForDeath() {
	return killMeNextLoop;
}

const TypeID Entity::GetTypeID()
{
	return typeID;
}

void Entity::CollideWith(CollisionData* i_collision)
{
	Entity* other;
	if (i_collision->entA.get() == this) {
		other = i_collision->entB.get();
	}
	else {
		other = i_collision->entA.get();
	}

	switch (other->GetTypeID()) {
	case PLAYER:
	{
		auto player = reinterpret_cast<PlayerChar*>(other);
		CollideWithPlayer(player);
		break;
	}
	case PROJ_BASIC:
	{
		auto projectile = reinterpret_cast<Projectile*>(other);
		CollideWithProjectile(projectile);
		break;
	}
	case PROJ_WALL:
	{
		auto blocker = reinterpret_cast<Blocker*>(other);
		CollideWithBlocker(blocker);
		break;
	}
	case ENEMY_SEEK:
	{
		auto enemy = reinterpret_cast<Enemy*>(other);
		CollideWithEnemy(enemy, *i_collision);
		break; 
	}
	case ENEMY_RAND: {
		auto enemy = reinterpret_cast<Enemy*>(other);
		CollideWithEnemy(enemy, *i_collision);
		break; 
	}
	case ENEMY_BOSS: {
		auto enemy = reinterpret_cast<Enemy*>(other);
		CollideWithEnemy(enemy, *i_collision);
		break;
	}
	case WALL_BASIC: {
		auto wall = reinterpret_cast<Wall*>(other);
		CollideWithWall(wall);
		break; 
	}
	case WALL_FIRE: {
		auto painWall = reinterpret_cast<PainWall*>(other);
		CollideWithPainWall(painWall);
		break;
	}
	case DOOR: {
		auto door = reinterpret_cast<Door*>(other);
		CollideWithDoor(door);
		break; 
	}
	case END_LEVEL: {
		auto endObjPtr = reinterpret_cast<EndObject*>(other);
		CollideWithEndObject(endObjPtr);
		break; 
	}
	case UPGRADE: {
		auto powUp = reinterpret_cast<PowerUp*>(other);
		CollideWithPowUp(powUp);
		break; 
	}
	case BLAST_STUN: {
		auto blast = reinterpret_cast<Blast*>(other);
		CollideWithBlast(blast);
		break; 
	}
	default: {
		std::string str = "Entity type could not be found during collision\n";
		std::cout << str;
		break; 
	}
	}
}

drawablePtrVect Entity::CreateDrawables(float i_lerp_fraction)
{
	if (drawables->size() > 0) { drawables->clear(); }
	if (visuals->size() > 0) { visuals->clear(); }
	//for linear interpolation this will now use the previous position and orientation
	if (physicalObject) {
		std::shared_ptr<sf::Shape> drawShape = CreateDrawableRB(i_lerp_fraction);
		drawables->emplace_back(drawShape);
	}
	if (hasVisuals) {
		UpdateVisuals(i_lerp_fraction);
		auto visualIt = visuals->begin();
		while (visualIt != visuals->end()) {
			drawables->emplace_back(*visualIt);
			visualIt++;
		}
	}
	return drawables;
}

std::shared_ptr<sf::Shape> Entity::CreateDrawableRB(float i_lerp_fraction)
{
	Vector2f lerpPos = rb.GetLerpPosition(i_lerp_fraction);
	float lerpOrient = rb.GetLerpOrient(i_lerp_fraction);
	std::shared_ptr<sf::Shape> drawShape = rb.shape->GetSFMLRepr();
	drawShape->setOrigin(sf::Vector2f(rb.shape->GetSFMLOriginOffset()(0), rb.shape->GetSFMLOriginOffset()(1)));
	drawShape->setPosition(lerpPos(0), lerpPos(1));
	drawShape->setRotation((lerpOrient*180.0f) / PI);
	drawShape->setFillColor(fillColor);
	drawShape->setOutlineColor(outlineColor);
	drawShape->setOutlineThickness(borderThickness);
	return drawShape;
}

void Entity::CollideWithPlayer(PlayerChar* i_playerPtr) {}

void Entity::CollideWithProjectile(Projectile * i_projPtr) {}

void Entity::CollideWithBlocker(Blocker* i_blockPtr) {}

void Entity::CollideWithEnemy(Enemy* i_enemyPtr, CollisionData i_collisionCopy) {}

void Entity::CollideWithWall(Wall* i_wallPtr) {}

void Entity::CollideWithPainWall(PainWall * i_painWallPtr) {}

void Entity::CollideWithDoor(Door* i_doorPtr) {}

void Entity::CollideWithEndObject(EndObject * i_endPtr) {}

void Entity::CollideWithPowUp(PowerUp* i_powUpPtr) {}

void Entity::CollideWithBlast(Blast * i_blastPtr)
{
}

void Entity::UpdateVisuals(float i_stepSize)
{

}

void Entity::Destroy() {
	killMeNextLoop = true;
}

void Entity::Update(float i_stepSize)
{
}

Entity::~Entity()
{
}