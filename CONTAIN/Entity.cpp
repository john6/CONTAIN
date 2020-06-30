//I have to include a bunch of header files or else I get the circular header file reference issue,
// because my Entities and Sector and Level and Game all reference each other whoopsidasies
#include "Entity.h"
#include "Game.h"
#include "Level.h"
#include "Sector.h"

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

Entity::Entity(RigidBody i_rb, Vector2f i_startPosition) :
	rb { i_rb }
{
	/*Material defMaterial(0.6f, 0.1f, 0.6f, 0.3f);
	rb.mat = defMaterial;
	rb.SetMassData();*/
	fillColor = sf::Color::Black;
	outlineColor = sf::Color::White;
	killMeNextLoop = false;
	rb.ResetPosition(i_startPosition);
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
PlayerChar::PlayerChar(RigidBody i_rb, Vector2f i_startPosition, Game* i_gamePtr) : Entity(i_rb, i_startPosition), gamePtr{ i_gamePtr }, pController{ PlayerController(i_gamePtr->renderWindow) }
{
	fillColor = sf::Color::Yellow;
	outlineColor = sf::Color::Red;
	shipRateOfFire = 1;
	shipSpeed = 40;
	lastShotFired = std::chrono::high_resolution_clock::now();
	lastDamageReceived = std::chrono::high_resolution_clock::now();
	dmgRate = 1.0f;
	health = 5;
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
		if (door->open) {
			gamePtr->RequestTravelToSector(door->GetOutCoord());
			rb.ResetPosition(door->GetOutPos());
		}
	}
}

void PlayerChar::Update(float i_stepSize)
{
	UpdateHealth(i_stepSize);
	UpdateMovement(i_stepSize);
}

void PlayerChar::UpdateMovement(float i_stepSize)
{
	float moveDist = shipSpeed * i_stepSize;
	std::vector<PlayerController::Input> inputVect = pController.PollKeys();
	for (PlayerController::Input input : inputVect) {
		switch (input) {
		case PlayerController::Input::UP: {
			rb.ApplyImpulse(DOWN * moveDist, NULL_VECTOR);
			break;
		}
		case PlayerController::Input::LEFT: {
			rb.ApplyImpulse(LEFT * moveDist, NULL_VECTOR);
			break;
		}
		case PlayerController::Input::DOWN: {
			rb.ApplyImpulse(UP * moveDist, NULL_VECTOR);
			break;
		}
		case PlayerController::Input::RIGHT: {
			rb.ApplyImpulse(RIGHT * moveDist, NULL_VECTOR);
			break;
		}
		}
	}
	Vector2f mousePos = pController.PollMouse();
	auto timeSinceFired = std::chrono::duration_cast<std::chrono::seconds>(hiResTime::now() - lastShotFired);
	if (!mousePos.isZero() && (timeSinceFired.count() >= shipRateOfFire)) {
		lastShotFired = hiResTime::now();
		Vector2f projectileDir = mousePos - rb.transform.pos;
		projectileDir.normalize();
		std::shared_ptr<Shape> projectileShape = std::make_shared<Circle>(PROJECTILE_RADIUS);
		Material HeavyBall = Material(0.9f, 0.95f, 0.5f, 0.25f);
		RigidBody projBody = RigidBody(projectileShape, HeavyBall);
		projBody.ApplyImpulse((projectileDir * 3000.0f), NULL_VECTOR);
		std::shared_ptr<Entity> projectile = std::make_shared<Projectile>(projBody,
			rb.transform.pos + (projectileDir * 100.0f));
		gamePtr->levels[gamePtr->GetCurrLvl()]->GetSector(gamePtr->currSector)->AddEntPtrToSector(projectile);
	}
}

void PlayerChar::UpdateHealth(float i_stepSize)
{
	if (health <= 0.0f) {
		fillColor = sf::Color::Red;
		outlineColor = sf::Color::Red;
	}
}

void PlayerChar::TakeDamage(float i_dmg)
{
	auto timeSinceDamaged = std::chrono::duration_cast<std::chrono::seconds>(hiResTime::now() - lastDamageReceived);
	if (timeSinceDamaged.count() >= dmgRate) {
		lastDamageReceived = hiResTime::now();
		health -= i_dmg;
	}
}

void PlayerChar::Destroy() {
	killMeNextLoop = true;
}

//Projectile Class 

Projectile::Projectile(RigidBody i_rb, Vector2f i_startPosition) :
	Entity{ i_rb, i_startPosition}
{
	fillColor = sf::Color::Red;
	outlineColor = sf::Color::White;
}


Projectile::~Projectile()
{
}

void Projectile::CollideWith(Entity& i_other)
{
	if (auto player = dynamic_cast<PlayerChar*>(&i_other)) {
		Destroy();
	}
	else if (auto projectile = dynamic_cast<Projectile*>(&i_other)) {
		Destroy();
	}
	if (auto enemy = dynamic_cast<Enemy*>(&i_other)) {
		Destroy();
		enemy->Destroy();
	}
	else if (auto wall = dynamic_cast<Wall*>(&i_other)) {
		Destroy();
	}
	else if (auto door = dynamic_cast<Door*>(&i_other)) {
		Destroy();
	}
}

void Projectile::Update(float i_stepSize)
{
}

void Projectile::Destroy() {
	killMeNextLoop = true;
}

//Enemy Class

Enemy::Enemy(RigidBody i_rb, Vector2f i_startPosition, std::shared_ptr<Entity> i_charPtr, Sector* i_sectPtr) :
	Entity(i_rb, i_startPosition), charPtr { i_charPtr }, sectPtr{ i_sectPtr }
{
	fillColor = sf::Color::Magenta;
	outlineColor = sf::Color::White;
	speed = 15;
}


Enemy::~Enemy()
{
}

void Enemy::CollideWith(Entity& i_other)
{
	if (auto player = dynamic_cast<PlayerChar*>(&i_other)) {
		player->TakeDamage(1.0f);
	}
	else if (auto projectile = dynamic_cast<Projectile*>(&i_other)) {

	}
	else if (auto enemy = dynamic_cast<Enemy*>(&i_other)) {

	}
	else if (auto wall = dynamic_cast<Wall*>(&i_other)) {

	}
	else if (auto door = dynamic_cast<Door*>(&i_other)) {

	}
}

void Enemy::Update(float i_stepSize)
{
	Vector2f playerDir = charPtr->rb.transform.pos - rb.transform.pos;
	playerDir.normalize();
	float moveDist = speed * i_stepSize;
	rb.ApplyImpulse(playerDir * moveDist, NULL_VECTOR);
}

void Enemy::Destroy() {
	sectPtr->sectEnemyNum -= 1;
	killMeNextLoop = true;
}

//Door class

Door::Door(RigidBody i_rb, Vector2f i_startPosition, Sector* i_sectPtr, MapCoord i_outSect, Vector2f i_outCoord) :
	Entity(i_rb, i_startPosition), sectPtr{ i_sectPtr }, outCoord{ i_outSect }, outPos{ i_outCoord }
{
	open = false;
	fillColor = sf::Color::Black;
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

const MapCoord Door::GetOutCoord()
{
	return outCoord;
}

const Vector2f Door::GetOutPos()
{
	return outPos;
}

void Door::Update(float i_stepSize)
{
	if (sectPtr->sectEnemyNum == 0) {
		open = true;
		fillColor = sf::Color::Cyan;
	}
}

void Door::Destroy() {
	killMeNextLoop = true;
}

//Wall Class 

Wall::Wall(RigidBody i_rb, Vector2f i_startPosition, Sector* i_sectPtr) :
	Entity(i_rb, i_startPosition), sectPtr{ i_sectPtr }
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

void Wall::Update(float i_stepSize)
{
}

void Wall::Destroy() {
	killMeNextLoop = true;
}

