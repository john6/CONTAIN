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
	fixedPosition = false;
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
	intangible = false;
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

	else if (auto powUp = dynamic_cast<PowerUp*>(&i_other)) {
		CollideWithPowUp(powUp);
	}

	else if (auto blast = dynamic_cast<Blast*>(&i_other)) {
		CollideWithBlast(blast);
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

void Entity::CollideWithPowUp(PowerUp* i_powUpPtr) {}

void Entity::CollideWithBlast(Blast * i_blastPtr)
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

///////////////////////PlayerChar class ///////////////////////
PlayerChar::PlayerChar(RigidBody i_rb, Vector2f i_startPosition, Game* i_gamePtr, int i_strtHealth) :
	Entity(i_rb, i_startPosition), gamePtr{ i_gamePtr },
	pController{ PlayerController(i_gamePtr->renderWindow) }
{
	scatterShot = false;
	fillColor = sf::Color::Yellow;
	outlineColor = sf::Color::Red;
	shipRateOfFire = 1.0f;
	shipSpeed = 100;
	lastShotFired = std::chrono::high_resolution_clock::now();
	lastDamageReceived = std::chrono::high_resolution_clock::now();
	dmgRate = 1.0f;
	maxHealth = i_strtHealth;
	health = maxHealth;
	specialAmmo = 3;
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
	weaponDelay = (std::chrono::duration_cast<std::chrono::microseconds>(hiResTime::now() - lastShotFired)).count() / 1000000.0f;
	if (!leftMousePos.isZero() && (weaponDelay >= shipRateOfFire)) {
		lastShotFired = hiResTime::now();
		Vector2f projectileDir = leftMousePos - rb.transform.pos;
		projectileDir.normalize();
		if (scatterShot) { ScatterShot(projectileDir); }
		else {
			std::shared_ptr<Shape> projectileShape = std::make_shared<Circle>(PROJECTILE_RADIUS);
			Material HeavyBall = Material(0.9f, 0.95f, 0.5f, 0.25f);
			RigidBody projBody = RigidBody(projectileShape, HeavyBall);
			projBody.ApplyImpulse((projectileDir * 3000.0f), NULL_VECTOR);
			std::shared_ptr<Entity> projectile = std::make_shared<Projectile>(projBody,
				rb.transform.pos + (projectileDir * 100.0f));
			gamePtr->levels[gamePtr->GetCurrLvl()]->GetSector(gamePtr->currSector)->AddEntPtrToSector(projectile);
		}
	}

	Vector2f rightMousePos = pController.RightClick();
	weaponDelay = (std::chrono::duration_cast<std::chrono::microseconds>(hiResTime::now() - lastShotFired)).count() / 1000000.0f;
	if (!rightMousePos.isZero() && (weaponDelay >= shipRateOfFire)) {
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

	bool scrollClick = pController.ScrollClick();
	weaponDelay = (std::chrono::duration_cast<std::chrono::microseconds>(hiResTime::now() - lastShotFired)).count() / 1000000.0f;
	if (scrollClick && (weaponDelay >= shipRateOfFire) && (specialAmmo > 0)) {
		--specialAmmo;
		lastShotFired = hiResTime::now();
		Vector2f projectileDir = rightMousePos - rb.transform.pos;
		projectileDir.normalize();
		std::shared_ptr<Shape> projectileShape = std::make_shared<Circle>(175);
	Material Static = Material(0.0f, 0.0f, 0.4f, 0.2f);
		RigidBody projBody = RigidBody(projectileShape, Static);
		std::shared_ptr<Entity> projectile = std::make_shared<Blast>(gamePtr->levels[gamePtr->GetCurrLvl()]->GetSector(gamePtr->currSector).get(),
			0, projBody, rb.transform.pos);
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

void PlayerChar::CollideWithPowUp(PowerUp* i_powUpPtr)
{
	switch (i_powUpPtr->powType) {
	case (0): { //rate of fire
		shipRateOfFire *= 0.5f;
		break;
	}
	case (1): { //rate of fire
		scatterShot = true;
		break;
	}
	}
	i_powUpPtr->Destroy();
}

void PlayerChar::ScatterShot(Vector2f i_dir)
{
	float radAngle = Math::VectToAngle(i_dir);
	float radAngle2 = radAngle + 0.174533;
	float radAngle3 = radAngle - 0.174533;
	Vector2f dir2 = Math::AngleToVect(radAngle2);
	Vector2f dir3 = Math::AngleToVect(radAngle3);

	std::shared_ptr<Shape> projectileShape = std::make_shared<Circle>(PROJECTILE_RADIUS);
	Material HeavyBall = Material(0.9f, 0.95f, 0.5f, 0.25f);
	RigidBody projBody = RigidBody(projectileShape, HeavyBall);
	projBody.ApplyImpulse((i_dir * 3000.0f), NULL_VECTOR);
	std::shared_ptr<Entity> projectile = std::make_shared<Projectile>(projBody,
		rb.transform.pos + (i_dir * 140.0f));
	gamePtr->levels[gamePtr->GetCurrLvl()]->GetSector(gamePtr->currSector)->AddEntPtrToSector(projectile);

	std::shared_ptr<Shape> projectileShape2 = std::make_shared<Circle>(PROJECTILE_RADIUS);
	Material HeavyBall2 = Material(0.9f, 0.95f, 0.5f, 0.25f);
	RigidBody projBody2 = RigidBody(projectileShape2, HeavyBall2);
	projBody2.ApplyImpulse((dir2 * 3000.0f), NULL_VECTOR);
	std::shared_ptr<Entity> projectile2 = std::make_shared<Projectile>(projBody2,
		rb.transform.pos + (dir2 * 120.0f));
	gamePtr->levels[gamePtr->GetCurrLvl()]->GetSector(gamePtr->currSector)->AddEntPtrToSector(projectile2);

	std::shared_ptr<Shape> projectileShape3 = std::make_shared<Circle>(PROJECTILE_RADIUS);
	Material HeavyBall3 = Material(0.9f, 0.95f, 0.5f, 0.25f);
	RigidBody projBody3 = RigidBody(projectileShape3, HeavyBall3);
	projBody3.ApplyImpulse((dir3 * 3000.0f), NULL_VECTOR);
	std::shared_ptr<Entity> projectile3 = std::make_shared<Projectile>(projBody3,
		rb.transform.pos + (dir3 * 100.0f));
	gamePtr->levels[gamePtr->GetCurrLvl()]->GetSector(gamePtr->currSector)->AddEntPtrToSector(projectile3);
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
	Stun(0.3f);
}

Enemy::~Enemy()
{
}

void Enemy::Update(float i_stepSize)
{ //enemies cant all access the player's location at the same time, so updates shouldnt be parallel
	if (stunSecs < 0) {
		Vector2f playerDir = charPtr->rb.transform.pos - rb.transform.pos;
		playerDir.normalize();
		float moveDist = speed * i_stepSize;
		rb.ApplyImpulse(playerDir * moveDist, NULL_VECTOR);
	}
	else {
		float secsInUpdate = i_stepSize / 1000.0f;
		stunSecs -= secsInUpdate;
	}
}

void Enemy::Destroy() {
	sectPtr->PlaySound(0);
	//sectPtr->sectEnemyNum -= 1;
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

void Enemy::CollideWithBlast(Blast * i_blastPtr)
{
	if (i_blastPtr->blastType == 0) {
		Stun(3.0f);
	}
}

void Enemy::Stun(float i_stunTime)
{
	stunSecs = i_stunTime;
}

/////////////////////// CrazyBoi class ///////////////////////

CrazyBoi::CrazyBoi(RigidBody i_rb, Vector2f i_startPosition, std::shared_ptr<Entity> i_charPtr, Sector * i_sectPtr) :
	Enemy(i_rb, i_startPosition, i_charPtr, i_sectPtr)
{
	sameDirTime = 0.9;
	speed = 30;
	timeTillDirSwitch = 0.0f;
	currDir = CreateRandomDir();
}

CrazyBoi::~CrazyBoi()
{
}

Vector2f CrazyBoi::CreateRandomDir()
{
	std::random_device rd1;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen1(rd1()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> distrib(1, 20); //both boundaries are inclusive
	float randX = (distrib(gen1) / 10.0f) - 1.0f;
	float randY = (distrib(gen1) / 10.0f) - 1.0f ;
	return Vector2f(randX, randY);
}

void CrazyBoi::Update(float i_stepSize)
{
	std::string str = "crazyStepSize: " + std::to_string(i_stepSize);
	std::cout << str;
	if (timeTillDirSwitch < 0) {
		timeTillDirSwitch = sameDirTime;
		currDir = CreateRandomDir();
		float moveDist = speed * i_stepSize;
		rb.ApplyImpulse(currDir * moveDist, NULL_VECTOR);
	}
	else {
		float secsInUpdate = i_stepSize / 1000.0f;
		timeTillDirSwitch -= secsInUpdate;
		float moveDist = speed * i_stepSize;
		rb.ApplyImpulse(currDir * moveDist, NULL_VECTOR);
	}
}

void CrazyBoi::CollideWithPainWall(PainWall * i_painWallPtr)
{
}

///////////////////////Door class///////////////////////

Door::Door(RigidBody i_rb, Vector2f i_startPosition, Sector* i_sectPtr, MapCoord i_outSect, Vector2f i_outCoord) :
	Entity(i_rb, i_startPosition), sectPtr{ i_sectPtr }, outCoord{ i_outSect }, outPos{ i_outCoord }
{
	open = false;
	intangible = true;
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
	if (sectPtr->sectEnemyNum <= 0) {
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
	intangible = true;
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


/////////////////////// Power Up ///////////////////////


PowerUp::PowerUp(Sector * i_sectPtr, int i_powType, RigidBody i_rb, Vector2f i_startPosition) :
	Entity(i_rb, i_startPosition), sectPtr{ i_sectPtr }, powType { i_powType }
{
	switch (i_powType) {
	case (0): {
		fillColor = sf::Color(255, 128, 128);
		outlineColor = sf::Color::Magenta;
		break;
	}
	case (1): {
		fillColor = sf::Color(51, 153, 102);
		outlineColor = sf::Color::Magenta;
		break;
	}
	}


}

PowerUp::~PowerUp()
{
	fillColor = sf::Color::Magenta;
	outlineColor = sf::Color::Magenta;
}

void PowerUp::Update(float i_stepSize)
{
	rb.angVel = 0.03f;
}


/////////////////////// Blast ///////////////////////


Blast::Blast(Sector* i_sectPtr, int i_blastType, RigidBody i_rb, Vector2f i_startPosition) :
	Entity(i_rb, i_startPosition), sectPtr{ i_sectPtr }, blastType{ i_blastType }
{
	fillColor = sf::Color::Black;
	outlineColor = sf::Color::Cyan;
	intangible = true;
	deathTimer = 1.0f;
}

Blast::~Blast()
{
}

void Blast::Update(float i_stepSize)
{
	float secsInUpdate = i_stepSize / 1000.0f;
	deathTimer -= secsInUpdate;
	if (deathTimer <= 0) { Destroy(); }
}


