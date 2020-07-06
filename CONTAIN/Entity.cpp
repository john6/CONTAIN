//I have to include a bunch of header files or else I get the circular header file reference issue,
// because my Entities and Sector and Level and Game all reference each other whoopsidasies
#include "Entity.h"
#include "Game.h"
#include "Level.h"
#include "Sector.h"

///////////////////////Entity///////////////////////
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

void Entity::CollideWith(Entity & i_other)
{
	if (auto player = dynamic_cast<PlayerChar*>(&i_other)) {
		CollideWithPlayer(player);
	}
	else if (auto projectile = dynamic_cast<Projectile*>(&i_other)) {
		CollideWithProjectile(projectile);
	}
	if (auto enemy = dynamic_cast<Enemy*>(&i_other)) {
		CollideWithEnemy(enemy);
	}
	else if (auto wall = dynamic_cast<Wall*>(&i_other)) {
		CollideWithWall(wall);
	}
	else if (auto painWall = dynamic_cast<PainWall*>(&i_other)) {
		CollideWithPainWall(painWall);
	}
	else if (auto door = dynamic_cast<Door*>(&i_other)) {
		CollideWithDoor(door);
	}
	else if (auto endObjPtr= dynamic_cast<EndObject*>(&i_other)) {
		CollideWithEndObject(endObjPtr);
	}
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

void Entity::CollideWithPlayer(PlayerChar* i_playerPtr) {}

void Entity::CollideWithProjectile(Projectile * i_projPtr) {}

void Entity::CollideWithEnemy(Enemy* i_enemyPtr) {}

void Entity::CollideWithWall(Wall* i_wallPtr) {}

void Entity::CollideWithPainWall(PainWall * i_painWallPtr) {}

void Entity::CollideWithDoor(Door* i_doorPtr) {}

void Entity::CollideWithEndObject(EndObject * i_endPtr) {}

void Entity::Destroy() {
	killMeNextLoop = true;
}

void Entity::Update(float i_stepSize)
{
}

Entity::~Entity()
{
}

///////////////////////PlayerChar class ///////////////////////
PlayerChar::PlayerChar(RigidBody i_rb, Vector2f i_startPosition, Game* i_gamePtr, int i_strtHealth) :
	Entity(i_rb, i_startPosition), gamePtr{ i_gamePtr },
	pController{ PlayerController(i_gamePtr->renderWindow) }
{
	fillColor = sf::Color::Yellow;
	outlineColor = sf::Color::Red;
	shipRateOfFire = 0.8f;
	shipSpeed = 100;
	lastShotFired = std::chrono::high_resolution_clock::now();
	lastDamageReceived = std::chrono::high_resolution_clock::now();
	dmgRate = 1.0f;
	maxHealth = i_strtHealth;
	health = maxHealth;
	weaponDelay = shipRateOfFire;
}

PlayerChar::~PlayerChar()
{
}

void PlayerChar::Update(float i_stepSize)
{
	UpdateHealth(i_stepSize);
	UpdateMovement(i_stepSize);
	AcceptWeaponInput(i_stepSize);
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
}

void PlayerChar::UpdateHealth(float i_stepSize)
{
	if (health <= 0.0f) {
		fillColor = sf::Color::Red;
		outlineColor = sf::Color::Red;
	}
}

void PlayerChar::ResetHealth()
{
	health = maxHealth;
}

void PlayerChar::AddHealth(int i_healthUp)
{
	health = std::max(health + i_healthUp, maxHealth);
}

void PlayerChar::AcceptWeaponInput(float i_stepSize)
{
	Vector2f leftMousePos = pController.LeftClick();
	weaponDelay = shipRateOfFire - (std::chrono::duration_cast<std::chrono::seconds>(hiResTime::now() - lastShotFired)).count();
	if (!leftMousePos.isZero() && (weaponDelay <= 0.0f)) {
		lastShotFired = hiResTime::now();
		Vector2f projectileDir = leftMousePos - rb.transform.pos;
		projectileDir.normalize();
		std::shared_ptr<Shape> projectileShape = std::make_shared<Circle>(PROJECTILE_RADIUS);
		Material HeavyBall = Material(0.9f, 0.95f, 0.5f, 0.25f);
		RigidBody projBody = RigidBody(projectileShape, HeavyBall);
		projBody.ApplyImpulse((projectileDir * 3000.0f), NULL_VECTOR);
		std::shared_ptr<Entity> projectile = std::make_shared<Projectile>(projBody,
			rb.transform.pos + (projectileDir * 100.0f));
		gamePtr->levels[gamePtr->GetCurrLvl()]->GetSector(gamePtr->currSector)->AddEntPtrToSector(projectile);
	}

	Vector2f rightMousePos = pController.RightClick();
	weaponDelay = shipRateOfFire - (std::chrono::duration_cast<std::chrono::seconds>(hiResTime::now() - lastShotFired)).count();
	if (!rightMousePos.isZero() && (weaponDelay <= 0.0f)) {
		lastShotFired = hiResTime::now();
		Vector2f projectileDir = rightMousePos - rb.transform.pos;
		projectileDir.normalize();
		std::shared_ptr<Shape> projectileShape = std::make_shared<Rectangle>(40, 270);
		Material HeavyBall = Material(0.9f, 0.95f, 0.5f, 0.25f);
		RigidBody projBody = RigidBody(projectileShape, HeavyBall);
		projBody.ApplyImpulse((projectileDir * 7000.0f), NULL_VECTOR);
		projBody.ResetOrientation(projectileDir);
		std::shared_ptr<Entity> projectile = std::make_shared<Blocker>(projBody,
			rb.transform.pos + (projectileDir * 100.0f));
		gamePtr->levels[gamePtr->GetCurrLvl()]->GetSector(gamePtr->currSector)->AddEntPtrToSector(projectile);
	}
}

void PlayerChar::TakeDamage(float i_dmg)
{
	auto timeSinceDamaged = std::chrono::duration_cast<std::chrono::seconds>(hiResTime::now() - lastDamageReceived);
	if (timeSinceDamaged.count() >= dmgRate) {
		gamePtr->resources->PlaySound(1);
		lastDamageReceived = hiResTime::now();
		health -= i_dmg;
	}
}

float PlayerChar::GetCurrHealth()
{
	return health;
}

void PlayerChar::CollideWithPainWall(PainWall * i_painWallPtr)
{
	TakeDamage(1.0f);
}

void PlayerChar::CollideWithDoor(Door * i_doorPtr)
{
	if (i_doorPtr->open) {
		gamePtr->RequestTravelToSector(i_doorPtr->GetOutCoord());
		rb.ResetPosition(i_doorPtr->GetOutPos());
	}
}

void PlayerChar::CollideWithEndObject(EndObject * i_endPtr)
{
	gamePtr->RequestGoToNextLvl();
}

///////////////////////Projectile Class ///////////////////////

Projectile::Projectile(RigidBody i_rb, Vector2f i_startPosition) :
	Entity{ i_rb, i_startPosition}
{
	fillColor = sf::Color::Red;
	outlineColor = sf::Color::White;
}

Projectile::~Projectile()
{
}

void Projectile::Update(float i_stepSize)
{
}

void Projectile::Destroy() {
	killMeNextLoop = true;
}

void Projectile::CollideWithPlayer(PlayerChar * i_playerPtr)
{
	Destroy();
}

void Projectile::CollideWithProjectile(Projectile * i_projPtr)
{
	Destroy();
}

void Projectile::CollideWithEnemy(Enemy * i_enemyPtr)
{
	Destroy();
	i_enemyPtr->Destroy();
}

void Projectile::CollideWithWall(Wall * i_wallPtr)
{
	Destroy();
}

void Projectile::CollideWithPainWall(PainWall * i_painWallPtr)
{
	Destroy();
}

void Projectile::CollideWithDoor(Door * i_doorPtr)
{
	Destroy();
}

///////////////////////Enemy Class///////////////////////

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

void Enemy::Update(float i_stepSize)
{
	Vector2f playerDir = charPtr->rb.transform.pos - rb.transform.pos;
	playerDir.normalize();
	float moveDist = speed * i_stepSize;
	rb.ApplyImpulse(playerDir * moveDist, NULL_VECTOR);
}

void Enemy::Destroy() {
	sectPtr->PlaySound(0);
	sectPtr->sectEnemyNum -= 1;
	killMeNextLoop = true;
}

void Enemy::CollideWithPainWall(PainWall * i_painWallPtr)
{
	Destroy();
}

void Enemy::CollideWithPlayer(PlayerChar * i_playerPtr)
{
	i_playerPtr->TakeDamage(1.0f);
}

///////////////////////Door class///////////////////////
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

///////////////////////Wall Class ///////////////////////

Wall::Wall(RigidBody i_rb, Vector2f i_startPosition, Sector* i_sectPtr) :
	Entity(i_rb, i_startPosition), sectPtr{ i_sectPtr }
{
	fillColor = sf::Color::Black;
	outlineColor = sf::Color::White;
}

Wall::~Wall()
{
}

/////////////////////// Pain Wall Class ///////////////////////

PainWall::PainWall(RigidBody i_rb, Vector2f i_startPosition, Sector* i_sectPtr) :
	Entity(i_rb, i_startPosition), sectPtr{ i_sectPtr }
{
	fillColor = sf::Color::Red;
	outlineColor = sf::Color::Yellow;
}

PainWall::~PainWall()
{
}




/////////////////////// End Object ///////////////////////

EndObject::EndObject(RigidBody i_rb, Vector2f i_startPosition, Sector * i_sectPtr) :
	Entity(i_rb, i_startPosition), sectPtr{ i_sectPtr }
{
	fillColor = sf::Color::Magenta;
	outlineColor = sf::Color::Magenta;
}

EndObject::~EndObject()
{
}


/////////////////////// Blocker ///////////////////////

Blocker::Blocker(RigidBody i_rb, Vector2f i_startPosition) :
Entity(i_rb, i_startPosition)
{
	fillColor = sf::Color::Green;
	outlineColor = sf::Color::Green;
}

Blocker::~Blocker()
{
}