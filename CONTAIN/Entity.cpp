#include "Entity.h"

//Entity
Entity::Entity() :
	rb { RigidBody(std::make_shared<Rectangle>(100.0f, 100.0f)) }
{
	Material defMaterial(0.6f, 0.1f, 0.6f, 0.3f);
	rb.mat = defMaterial;
	rb.SetMassData();
	fillColor = sf::Color::Black;
	outlineColor = sf::Color::White;
}

Entity::Entity(RigidBody i_rb) :
	rb { i_rb }
{
	/*Material defMaterial(0.6f, 0.1f, 0.6f, 0.3f);
	rb.mat = defMaterial;
	rb.SetMassData();*/
	fillColor = sf::Color::Black;
	outlineColor = sf::Color::White;
	killMeNextLoop = false;
}

const bool Entity::MarkedForDeath() {
	return killMeNextLoop;
}

std::unique_ptr<sf::Shape> Entity::CreateDrawable(float i_lerp_fraction)
{
	//for linear interpolation this will now use the previous position and orientation
	Vector2f lerpPos = rb.GetLerpPosition(i_lerp_fraction);
	float lerpOrient = rb.GetLerpOrient(i_lerp_fraction);
	std::unique_ptr<sf::Shape> drawShape = rb.shape->GetSFMLRepr();
	drawShape->setOrigin(sf::Vector2f(rb.shape->GetSFMLOriginOffset()(0), rb.shape->GetSFMLOriginOffset()(1)));
	drawShape->setPosition(lerpPos(0), lerpPos(1));
	drawShape->setRotation((lerpOrient*180.0f) / PI);
	drawShape->setFillColor(fillColor);
	drawShape->setOutlineColor(outlineColor);
	drawShape->setOutlineThickness(3.0f);
	return drawShape;
}

Entity::~Entity()
{
}

//PlayerChar class 

PlayerChar::PlayerChar(RigidBody i_rb) :
	Entity(i_rb)
{
	fillColor = sf::Color::Yellow;
	outlineColor = sf::Color::Red;
}


PlayerChar::~PlayerChar()
{
}

void PlayerChar::CollideWith(Entity& i_other) 
{
	if (auto enemy = dynamic_cast<PlayerChar*>(&i_other)) {

	}
	else if (auto projectile = dynamic_cast<Projectile*>(&i_other)) {

	}
	if (auto enemy = dynamic_cast<Enemy*>(&i_other)) {

	}
	else if (auto wall = dynamic_cast<Wall*>(&i_other)) {

	}
	else if (auto door = dynamic_cast<Door*>(&i_other)) {

	}
}

void PlayerChar::Destroy() {
	killMeNextLoop = true;
}

//Projectile Class 

Projectile::Projectile(RigidBody i_rb) :
	Entity{ i_rb }
{
	fillColor = sf::Color::Red;
	outlineColor = sf::Color::White;
}


Projectile::~Projectile()
{
}

void Projectile::CollideWith(Entity& i_other)
{
	if (auto enemy = dynamic_cast<PlayerChar*>(&i_other)) {
		Destroy();
	}
	else if (auto projectile = dynamic_cast<Projectile*>(&i_other)) {
		Destroy();
	}
	if (auto enemy = dynamic_cast<Enemy*>(&i_other)) {
		Destroy();
	}
	else if (auto wall = dynamic_cast<Wall*>(&i_other)) {
		Destroy();
	}
	else if (auto door = dynamic_cast<Door*>(&i_other)) {
		Destroy();
	}
}

void Projectile::Destroy() {
	killMeNextLoop = true;
}

//Enemy Class

Enemy::Enemy(RigidBody i_rb) :
	Entity(i_rb)
{
	fillColor = sf::Color::Magenta;
	outlineColor = sf::Color::White;
}


Enemy::~Enemy()
{
}

void Enemy::CollideWith(Entity& i_other)
{
	if (auto enemy = dynamic_cast<PlayerChar*>(&i_other)) {

	}
	else if (auto projectile = dynamic_cast<Projectile*>(&i_other)) {
		Destroy();
	}
	if (auto enemy = dynamic_cast<Enemy*>(&i_other)) {

	}
	else if (auto wall = dynamic_cast<Wall*>(&i_other)) {

	}
	else if (auto door = dynamic_cast<Door*>(&i_other)) {

	}
}

void Enemy::Destroy() {
	killMeNextLoop = true;
}

//Door class

Door::Door(RigidBody i_rb) :
	Entity(i_rb)
{
	fillColor = sf::Color::Cyan;
	outlineColor = sf::Color::White;
}


Door::~Door()
{
}

void Door::CollideWith(Entity& i_other)
{
	if (auto enemy = dynamic_cast<PlayerChar*>(&i_other)) {

	}
	else if (auto projectile = dynamic_cast<Projectile*>(&i_other)) {

	}
	if (auto enemy = dynamic_cast<Enemy*>(&i_other)) {

	}
	else if (auto wall = dynamic_cast<Wall*>(&i_other)) {

	}
	else if (auto door = dynamic_cast<Door*>(&i_other)) {

	}
}

void Door::Destroy() {
	killMeNextLoop = true;
}

//Wall Class 

Wall::Wall(RigidBody i_rb) :
	Entity(i_rb)
{
	fillColor = sf::Color::White;
	outlineColor = sf::Color::White;
}


Wall::~Wall()
{
}

void Wall::CollideWith(Entity& i_other)
{
	if (auto enemy = dynamic_cast<PlayerChar*>(&i_other)) {

	}
	else if (auto projectile = dynamic_cast<Projectile*>(&i_other)) {

	}
	if (auto enemy = dynamic_cast<Enemy*>(&i_other)) {

	}
	else if (auto wall = dynamic_cast<Wall*>(&i_other)) {

	}
	else if (auto door = dynamic_cast<Door*>(&i_other)) {

	}
}

void Wall::Destroy() {
	killMeNextLoop = true;
}

