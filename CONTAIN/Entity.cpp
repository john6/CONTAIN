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
	texturePtr = NULL;

	if (rb.shape->GetType() == Shape::ShapeType::RECTANGLE) {
		std::shared_ptr<Rectangle> rectPtr = std::dynamic_pointer_cast<Rectangle>(rb.shape);
		textRectWidth = rectPtr->GetWidth();
		textRectHeight = rectPtr->GetHeight();
	}
	else {
		std::shared_ptr<Rectangle> rectPtr = std::dynamic_pointer_cast<Rectangle>(rb.shape);
		textRectWidth = rb.shape->GetDistToEdge() * 2;
		textRectHeight = rb.shape->GetDistToEdge() * 2;
	}

	textStretchWidth = 1;
	textStretchHeight = 1;
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
	texturePtr = NULL;


	if (rb.shape->GetType() == Shape::ShapeType::RECTANGLE) {
		std::shared_ptr<Rectangle> rectPtr = std::dynamic_pointer_cast<Rectangle>(rb.shape);
		textRectWidth = rectPtr->GetWidth();
		textRectHeight = rectPtr->GetHeight();
	}
	else {
		std::shared_ptr<Rectangle> rectPtr = std::dynamic_pointer_cast<Rectangle>(rb.shape);
		textRectWidth = rb.shape->GetDistToEdge() * 2;
		textRectHeight = rb.shape->GetDistToEdge() * 2;
	}

	textStretchWidth = 1;
	textStretchHeight = 1;
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
	//I dont want to mess with collisions in the middle of the loop, Im gonna operate on a copy of the collision object
	CollisionData collisionCopy = CollisionData(*i_collision);

	//entA will always be equal to 'this' in the collision functions
	if (collisionCopy.entB.get() == this) {
		std::shared_ptr<Entity> myPtr = collisionCopy.entB;
		collisionCopy.entB = i_collision->entA;
		collisionCopy.entA = myPtr;
	}

	switch (collisionCopy.entB->GetTypeID()) {
	case PLAYER:
	{
		CollideWithPlayer(collisionCopy);
		break;
	}
	case PROJ_BASIC:
	{
		CollideWithProjectile(collisionCopy);
		break;
	}
	case PROJ_WALL:
	{
		CollideWithBlocker(collisionCopy);
		break;
	}
	case ENEMY_SEEK:
	{
		CollideWithEnemy(collisionCopy);
		break; 
	}
	case ENEMY_RAND: {
		CollideWithEnemy(collisionCopy);
		break; 
	}
	case ENEMY_BOSS: {
		CollideWithEnemy(collisionCopy);
		break;
	}
	case WALL_BASIC: {
		CollideWithWall(collisionCopy);
		break; 
	}
	case WALL_FIRE: {
		CollideWithPainWall(collisionCopy);
		break;
	}
	case DOOR: {
		CollideWithDoor(collisionCopy);
		break; 
	}
	case END_LEVEL: {
		CollideWithEndObject(collisionCopy);
		break; 
	}
	case UPGRADE: {
		CollideWithPowUp(collisionCopy);
		break; 
	}
	case BLAST_STUN: {
		CollideWithBlast(collisionCopy);
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

	//if (typeID == TypeID::WALL_FIRE) {
	//	std::cout << "yo";
	//}


	if (texturePtr != NULL) {
		texturePtr->setRepeated(true);
		//texturePtr->setSmooth(true);
		drawShape->setTexture(texturePtr.get());
		drawShape->setTextureRect(sf::IntRect(0, 0, textRectWidth * textStretchWidth, textRectHeight * textStretchHeight));
	}
	else {

	}
	drawShape->setFillColor(fillColor);
	drawShape->setOutlineColor(outlineColor);
	drawShape->setOutlineThickness(borderThickness);
	return drawShape;
}

void Entity::CollideWithPlayer(CollisionData i_collisionCopy) {}

void Entity::CollideWithProjectile(CollisionData i_collisionCopy) {}

void Entity::CollideWithBlocker(CollisionData i_collisionCopy) {}

void Entity::CollideWithEnemy(CollisionData i_collisionCopy) {}

void Entity::CollideWithWall(CollisionData i_collisionCopy) {}

void Entity::CollideWithPainWall(CollisionData i_collisionCopy) {}

void Entity::CollideWithDoor(CollisionData i_collisionCopy) {}

void Entity::CollideWithEndObject(CollisionData i_collisionCopy) {}

void Entity::CollideWithPowUp(CollisionData i_collisionCopy) {}

void Entity::CollideWithBlast(CollisionData i_collisionCopy)
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