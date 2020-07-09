//I have to include a bunch of header files or else I get the circular header file reference issue,
// because my Entities and Sector and Level and Game all reference each other whoopsidasies
#include "Entity.h"
#include "Game.h"
#include "Level.h"
#include "Sector.h"

///////////////////////Entity///////////////////////
Entity::Entity() :
	rb { RigidBody(std::make_shared<Rectangle>(100.0f, 100.0f)) }, 
	typeID{ ENTITY_VIRTUAL }
{
	Material defMaterial(0.6f, 0.1f, 0.6f, 0.3f);
	rb.mat = defMaterial;
	rb.SetMassData();
	fillColor = sf::Color::Black;
	outlineColor = sf::Color::White;
	fixedPosition = false;
}

Entity::Entity(Vector2f i_startPosition, RigidBody i_rb, TypeID i_typeID) :
	rb { i_rb }, typeID{ i_typeID }
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

const TypeID Entity::GetTypeID()
{
	return typeID;
}

void Entity::CollideWith(Entity & i_other)
{
	if (i_other.GetTypeID() == PLAYER) {
		auto player = dynamic_cast<PlayerChar*>(&i_other);
		CollideWithPlayer(player);
	}
	else if (i_other.GetTypeID() == PROJ_BASIC) {
		auto projectile = dynamic_cast<Projectile*>(&i_other);
		CollideWithProjectile(projectile);
	}
	if (i_other.GetTypeID() == ENEMY_SEEK) {
		auto enemy = dynamic_cast<Enemy*>(&i_other);
		CollideWithEnemy(enemy);
	}
	if (i_other.GetTypeID() == ENEMY_RAND) {
		auto enemy = dynamic_cast<Enemy*>(&i_other);
		CollideWithEnemy(enemy);
	}
	else if (i_other.GetTypeID() == WALL_BASIC) {
		auto wall = dynamic_cast<Wall*>(&i_other);
		CollideWithWall(wall);
	}
	else if (i_other.GetTypeID() == WALL_FIRE) {
		auto painWall = dynamic_cast<PainWall*>(&i_other);
		CollideWithPainWall(painWall);
	}
	else if (i_other.GetTypeID() == DOOR_LOCKED) {
		auto door = dynamic_cast<Door*>(&i_other);
		CollideWithDoor(door);
	}
	else if (i_other.GetTypeID() == END_LEVEL) {
		auto endObjPtr = dynamic_cast<EndObject*>(&i_other);
		CollideWithEndObject(endObjPtr);
	}

	else if (i_other.GetTypeID() == UPGRADE) {
		auto powUp = dynamic_cast<PowerUp*>(&i_other);
		CollideWithPowUp(powUp);
	}

	else if (i_other.GetTypeID() == BLAST_STUN) {
		auto blast = dynamic_cast<Blast*>(&i_other);
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
PlayerChar::PlayerChar(Game* i_gamePtr, int i_strtHealth, Vector2f i_startPosition, RigidBody i_rb) :
	Entity(i_startPosition, i_rb, PLAYER), gamePtr{ i_gamePtr },
	pController{ PlayerController(i_gamePtr->renderWindow) }
{
	numShots = 1;
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
	if (!leftMousePos.isZero()) {
		ShootBasic(leftMousePos);
	}
	Vector2f rightMousePos = pController.RightClick();
	if (!rightMousePos.isZero()) {
		ShootWall(rightMousePos);
	}
	bool scrollClick = pController.ScrollClick();
	if (scrollClick) {
		ShootAOE();
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
	case (1): { //number of shots
		numShots += 1;
		break;
	}
	}
	i_powUpPtr->Destroy();
}

void PlayerChar::ShootBasic(Vector2f i_mousePos)
{
	weaponDelay = (std::chrono::duration_cast<std::chrono::microseconds>(hiResTime::now() - lastShotFired)).count() / 1000000.0f;
	if (weaponDelay >= shipRateOfFire) {
		lastShotFired = hiResTime::now();
		Vector2f projDir = i_mousePos - rb.transform.pos;
		projDir.normalize();
		float prevAngleRads;
		Vector2f prevDirVect;
		float mouseAngleRad = Math::VectToAngle(projDir);
		float currAngleRads = mouseAngleRad - (0.0872665 * (numShots - 1));
		//Original angle is shifted back so cluster is still centered on cursor
		Vector2f currDirVect = Math::AngleToVect(currAngleRads);
		int i = 0;
		while (i < numShots) {
			std::shared_ptr<Entity> projectile = std::make_shared<Projectile>(
								rb.transform.pos + (currDirVect * (100.0f + (i * 15.0f))));
			projectile->rb.ApplyImpulse((currDirVect * 3000.0f), NULL_VECTOR);
			gamePtr->levels[gamePtr->GetCurrLvl()]->GetSector(gamePtr->currSector)->AddEntPtrToSector(projectile);
			prevAngleRads = currAngleRads;
			prevDirVect = currDirVect;
			currAngleRads = prevAngleRads + 0.174533;
			currDirVect = Math::AngleToVect(currAngleRads);
			i++;
		}
	}
}

void PlayerChar::ShootWall(Vector2f i_mousePos)
{
	weaponDelay = (std::chrono::duration_cast<std::chrono::microseconds>(hiResTime::now() - lastShotFired)).count() / 1000000.0f;
	if (weaponDelay >= shipRateOfFire) {
		lastShotFired = hiResTime::now();
		Vector2f projectileDir = i_mousePos - rb.transform.pos;
		projectileDir.normalize();
		ShootWall(projectileDir);
		std::shared_ptr<Entity> projectile = std::make_shared<Blocker>(
								rb.transform.pos + (projectileDir * 100.0f));
		projectile->rb.ApplyImpulse((projectileDir * 7000.0f), NULL_VECTOR);
		projectile->rb.ResetOrientation(projectileDir);
		gamePtr->levels[gamePtr->GetCurrLvl()]->GetSector(gamePtr->currSector)->AddEntPtrToSector(projectile);
	}
}

void PlayerChar::ShootAOE()
{
	weaponDelay = (std::chrono::duration_cast<std::chrono::microseconds>(hiResTime::now() - lastShotFired)).count() / 1000000.0f;
	if ((weaponDelay >= shipRateOfFire) && (specialAmmo > 0)) {
		--specialAmmo;
		lastShotFired = hiResTime::now();
		std::shared_ptr<Entity> projectile = std::make_shared<Blast>(
					gamePtr->levels[gamePtr->GetCurrLvl()]->GetSector(gamePtr->currSector).get(),
					rb.transform.pos, 0);
		gamePtr->levels[gamePtr->GetCurrLvl()]->GetSector(gamePtr->currSector)->AddEntPtrToSector(projectile);
	}
}

///////////////////////Basic Projectile Class ///////////////////////
Projectile::Projectile(Vector2f i_startPosition, RigidBody i_rb) :
	Entity(i_startPosition, i_rb, PROJ_BASIC)
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

/////////////////////// Blocker ///////////////////////
Blocker::Blocker(Vector2f i_startPosition, RigidBody i_rb) :
	Entity(i_startPosition, i_rb, PROJ_WALL)
{
	fillColor = sf::Color::Green;
	outlineColor = sf::Color::Green;
}

Blocker::~Blocker()
{
}

/////////////////////// Blast ///////////////////////
Blast::Blast(Sector* i_sectPtr, Vector2f i_startPosition, int i_blastType, RigidBody i_rb) :
	Entity(i_startPosition, i_rb, BLAST_STUN), sectPtr{ i_sectPtr }, blastType{ i_blastType }
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

///////////////////////Enemy Class///////////////////////
Enemy::Enemy(std::shared_ptr<Entity> i_charPtr, Sector* i_sectPtr, Vector2f i_startPosition, RigidBody i_rb, TypeID i_typeID) :
	Entity(i_startPosition, i_rb, i_typeID), charPtr { i_charPtr }, sectPtr{ i_sectPtr }
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
CrazyBoi::CrazyBoi(std::shared_ptr<Entity> i_charPtr, Sector* i_sectPtr, Vector2f i_startPosition, RigidBody i_rb) :
	Enemy(i_charPtr, i_sectPtr, i_startPosition, i_rb, ENEMY_RAND)
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

/////////////////////// Door Lockable class///////////////////////
Door::Door(Sector* i_sectPtr, MapCoord i_outCoord, Vector2f i_startPos, Vector2f i_outPos, RigidBody i_rb) :
	Entity(i_startPos, i_rb, DOOR_LOCKED), sectPtr{ i_sectPtr }, outCoord{ i_outCoord }, outPos{ i_outPos }
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
Wall::Wall(Vector2f i_startPosition, Sector* i_sectPtr, RigidBody i_rb) :
	Entity(i_startPosition, i_rb, WALL_BASIC), sectPtr{ i_sectPtr }
{
	fillColor = sf::Color::Black;
	outlineColor = sf::Color::White;
}

Wall::~Wall()
{
}

/////////////////////// Pain Wall Class ///////////////////////
PainWall::PainWall(Vector2f i_startPosition, Sector* i_sectPtr, RigidBody i_rb) :
	Entity(i_startPosition, i_rb, WALL_FIRE), sectPtr{ i_sectPtr }
{
	intangible = true;
	fillColor = sf::Color::Red;
	outlineColor = sf::Color::Yellow;
}

PainWall::~PainWall()
{
}

/////////////////////// End Object ///////////////////////
EndObject::EndObject(Sector* i_sectPtr, Vector2f i_startPosition, RigidBody i_rb) :
	Entity(i_startPosition, i_rb, END_LEVEL), sectPtr{ i_sectPtr }
{
	fillColor = sf::Color::Magenta;
	outlineColor = sf::Color::Magenta;
}

EndObject::~EndObject()
{
}


/////////////////////// Power Up ///////////////////////
PowerUp::PowerUp(Sector * i_sectPtr, Vector2f i_startPosition, int i_powType, RigidBody i_rb) :
	Entity(i_startPosition, i_rb, UPGRADE), sectPtr{ i_sectPtr }, powType { i_powType }
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

