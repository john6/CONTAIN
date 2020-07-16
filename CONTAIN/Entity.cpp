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
	else if (i_other.GetTypeID() == ENEMY_SEEK) {
		auto enemy = dynamic_cast<Enemy*>(&i_other);
		CollideWithEnemy(enemy);
	}
	else if (i_other.GetTypeID() == ENEMY_RAND) {
		auto enemy = dynamic_cast<Enemy*>(&i_other);
		CollideWithEnemy(enemy);
	}
	else if (i_other.GetTypeID() == ENEMY_BOSS) {
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
	shipRateOfFire = 0.9f;
	shipSpeed = 100;
	lastShotFired = std::chrono::high_resolution_clock::now();
	lastDamageReceived = std::chrono::high_resolution_clock::now();
	dmgRate = 1.0f;
	maxHealth = i_strtHealth;
	health = maxHealth;
	currSpecialAmmo = 3;
	weaponDelay = shipRateOfFire;
	wallWidth = 40;
	wallHeight = 270;
	BlastRadius = 175;
	maxSpecialAmmo = 3;
	weapSpeed = 3000.0f;
	InitLvl();
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

void PlayerChar::ResetSpecialAmmo()
{
	currSpecialAmmo = maxSpecialAmmo;
}

void PlayerChar::AddSpecialAmmo(int i_ammo)
{
	currSpecialAmmo = std::min(currSpecialAmmo + i_ammo, maxSpecialAmmo);
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
		gamePtr->resources->PlaySound(3);
		lastDamageReceived = hiResTime::now();
		health -= i_dmg;
	}
}

void PlayerChar::ReceivePowerUp(UPGRADE_TYPE i_powType)
{
	gamePtr->levels[gamePtr->currLvl]->GetSector(gamePtr->currSector)->PlaySound(RESOURCES::MENUACCEPT6);
	switch (i_powType) {
	case (RATE_OF_FIRE): { //rate of fire
		if (shipLvl.at(RATE_OF_FIRE) <= GLBVRS::GetUpgradeMax(RATE_OF_FIRE)) {
			shipLvl[RATE_OF_FIRE]++;
			shipRateOfFire *= 0.5f;
		}
		break;
	}
	case (WEAP_SPEED): { //number of shots
		if (shipLvl.at(WEAP_SPEED) <= GLBVRS::GetUpgradeMax(WEAP_SPEED)) {
			shipLvl[WEAP_SPEED]++;
			weapSpeed *= 1.5;
		}
		break;
	}
	case (SCATTER): { //number of shots
		if (shipLvl.at(SCATTER) <= GLBVRS::GetUpgradeMax(SCATTER)) {
			shipLvl[SCATTER]++;
			numShots += 1;
		}
		break;
	}
	case (SMALL_SHIP): { //number of shots
		if (shipLvl.at(SMALL_SHIP) <= GLBVRS::GetUpgradeMax(SMALL_SHIP)) {
			shipLvl[SMALL_SHIP]++;
			auto playerRect = dynamic_cast<Rectangle*>(rb.shape.get());
			rb.ChangeSizeOfShape(playerRect->GetWidth() - 15.0f, playerRect->GetHeight() - 15.0f);
		}
		break;
	}
	case (BIG_SHIP): { //number of shots
		if (shipLvl.at(BIG_SHIP) <= GLBVRS::GetUpgradeMax(BIG_SHIP)) {
			shipLvl[BIG_SHIP]++;
		}
		auto playerRect = dynamic_cast<Rectangle*>(rb.shape.get());
		rb.ChangeSizeOfShape(playerRect->GetWidth() + 15.0f, playerRect->GetHeight() + 15.0f);
		maxHealth += 5;
		shipSpeed += 15;
		break;
	}
	case (BLAST): { //number of shots
		if (shipLvl.at(BLAST) <= GLBVRS::GetUpgradeMax(BLAST)) {
			shipLvl[BLAST]++;
		}
		BlastRadius += 50;
		break;
	}
	case (WALL_BIG): { //number of shots
		if (shipLvl.at(WALL_BIG) <= GLBVRS::GetUpgradeMax(WALL_BIG)) {
			shipLvl[WALL_BIG]++;
		}
		wallWidth += 10;
		wallHeight += 50;
		break;
	}
	}
}

float PlayerChar::GetCurrHealth()
{
	return health;
}

void PlayerChar::CollideWithPainWall(PainWall * i_painWallPtr)
{
	gamePtr->levels[gamePtr->currLvl]->GetSector(gamePtr->currSector)->PlaySound(RESOURCES::FIRE5);
	TakeDamage(1.0f);
}

void PlayerChar::CollideWithDoor(Door * i_doorPtr)
{
	if (i_doorPtr->open) {
		gamePtr->levels[gamePtr->currLvl]->GetSector(gamePtr->currSector)->PlaySound(RESOURCES::MAGIC7);
		rb.ignoreForcesThisStep = true;
		gamePtr->RequestTravelToSector(i_doorPtr->GetOutCoord());
		rb.ResetPosition(i_doorPtr->GetOutPos());
	}
}

void PlayerChar::CollideWithEndObject(EndObject * i_endPtr)
{
	if (i_endPtr->active) {
		gamePtr->RequestGoToNextLvl();
	}
}

void PlayerChar::CollideWithPowUp(PowerUp* i_powUpPtr)
{
	ReceivePowerUp(i_powUpPtr->powType);
	i_powUpPtr->Destroy();
}

void PlayerChar::CollideWithProjectile(Projectile * i_projPtr)
{
	if (i_projPtr->projType == 1) {
		TakeDamage(1.0f);
	}
}

void PlayerChar::ShootBasic(Vector2f i_mousePos)
{
	weaponDelay = (std::chrono::duration_cast<std::chrono::microseconds>(hiResTime::now() - lastShotFired)).count() / 1000000.0f;
	if (weaponDelay >= shipRateOfFire) {
		gamePtr->levels[gamePtr->currLvl]->GetSector(gamePtr->currSector)->PlaySound(RESOURCES::SHOT2);
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
			projectile->rb.ApplyImpulse((currDirVect * weapSpeed), NULL_VECTOR);
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
		gamePtr->levels[gamePtr->currLvl]->GetSector(gamePtr->currSector)->PlaySound(RESOURCES::SHOOT14);
		lastShotFired = hiResTime::now();
		Vector2f projectileDir = i_mousePos - rb.transform.pos;
		projectileDir.normalize();
		ShootWall(projectileDir);
		std::shared_ptr<Entity> projectile = std::make_shared<Blocker>(
								rb.transform.pos + (projectileDir * 100.0f),
								RigidBody(std::make_shared<Rectangle>(wallWidth, wallHeight), HEAVYBALL));
		projectile->rb.ApplyImpulse((projectileDir * 7000.0f), NULL_VECTOR);
		projectile->rb.ResetOrientation(projectileDir);
		gamePtr->levels[gamePtr->GetCurrLvl()]->GetSector(gamePtr->currSector)->AddEntPtrToSector(projectile);
	}
}

void PlayerChar::InitLvl()
{
	for (int upInt = 0; upInt != WALL_BIG+1; upInt++) {
		shipLvl.emplace((UPGRADE_TYPE)upInt, 0);
	}
}

std::map<UPGRADE_TYPE, int>* PlayerChar::GetLvl()
{

	return &shipLvl;
}

void PlayerChar::ShootAOE()
{
	weaponDelay = (std::chrono::duration_cast<std::chrono::microseconds>(hiResTime::now() - lastShotFired)).count() / 1000000.0f;
	if ((weaponDelay >= shipRateOfFire) && (currSpecialAmmo > 0)) {
		gamePtr->levels[gamePtr->currLvl]->GetSector(gamePtr->currSector)->PlaySound(RESOURCES::MAGIC10);
		--currSpecialAmmo;
		lastShotFired = hiResTime::now();
		std::shared_ptr<Entity> projectile = std::make_shared<Blast>(
					gamePtr->levels[gamePtr->GetCurrLvl()]->GetSector(gamePtr->currSector).get(),
					rb.transform.pos, 0, RigidBody(std::make_shared<Circle>(BlastRadius), STATIC));
		gamePtr->levels[gamePtr->GetCurrLvl()]->GetSector(gamePtr->currSector)->AddEntPtrToSector(projectile);
	}
}

///////////////////////Basic Projectile Class ///////////////////////
Projectile::Projectile(Vector2f i_startPosition, int i_projType, RigidBody i_rb) :
	Entity(i_startPosition, i_rb, PROJ_BASIC), projType { i_projType }
{
	if (projType == 0) {
		fillColor = sf::Color::Red;
		outlineColor = sf::Color::White;
	}
	else if (projType == 1) {
		fillColor = sf::Color::Green;
		outlineColor = sf::Color::White;
	}
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
Enemy::Enemy(std::shared_ptr<Entity> i_charPtr, Sector* i_sectPtr, DIFFICULTY i_diff, Vector2f i_startPosition, RigidBody i_rb, TypeID i_typeID) :
	Entity(i_startPosition, i_rb, i_typeID), charPtr { i_charPtr }, sectPtr{ i_sectPtr }
{
	metal = false;
	fillColor = MOSS;
	outlineColor = sf::Color::White;
	SetDiffVars(i_diff);
	Stun(0.3f);
	maxHealth = 1;
	health = maxHealth;
}

Enemy::~Enemy()
{
}

void Enemy::Update(float i_stepSize)
{ //enemies cant all access the player's location at the same time, so updates shouldnt be parallel
	UpdateHealth(i_stepSize);
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

Vector2f Enemy::CreateRandomDir()
{
	std::random_device rd1;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen1(rd1()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> distrib(1, 20); //both boundaries are inclusive
	float randX = (distrib(gen1) / 10.0f) - 1.0f;
	float randY = (distrib(gen1) / 10.0f) - 1.0f;
	return Vector2f(randX, randY);
}

void Enemy::Destroy() {
	sectPtr->PlaySound(RESOURCES::OUCH1);
	//sectPtr->sectEnemyNum -= 1;
	killMeNextLoop = true;
}

void Enemy::CollideWithPainWall(PainWall * i_painWallPtr)
{
	sectPtr->PlaySound(RESOURCES::FIRE5);
	TakeDamage(1);
}

void Enemy::CollideWithPlayer(PlayerChar * i_playerPtr)
{
	if (!metal) {
		i_playerPtr->TakeDamage(1.0f);
	}
}

void Enemy::CollideWithBlast(Blast * i_blastPtr)
{
	if (i_blastPtr->blastType == 0) {
		Stun(3.0f);
	}
}

void Enemy::CollideWithProjectile(Projectile * i_projPtr)
{
	if (i_projPtr->projType == 0) {
		TakeDamage(1);
	}
}

void Enemy::UpdateHealth(float i_stepSize)
{
	if (health <= 0.0f) {
		fillColor = sf::Color::Red;
		outlineColor = sf::Color::Red;
		Destroy();
	}
}

void Enemy::TakeDamage(float i_dmg)
{
		health -= i_dmg;
}

void Enemy::Stun(float i_stunTime)
{
	stunSecs = i_stunTime;
}

void Enemy::SetDiffVars(int i_diff)
{
	switch (i_diff) {
	case EASY: {
		speed = ENEMYSPEEDEASY;
		break;
	}
	case MEDIUM: {
		speed = ENEMYSPEEDMED;
		break;
	}
	case HARD: {
		speed = ENEMYSPEEDHARD;
		break;
	}
	}
}

void Enemy::TurnToMetal()
{
	metal = true;
	fillColor = sf::Color(128, 128, 128);
	outlineColor = sf::Color::White;
	rb.mat = DENSE_METAL;
	speed *= 3.0f;
	rb.SetMassData();
}

/////////////////////// CrazyBoi class ///////////////////////
CrazyBoi::CrazyBoi(std::shared_ptr<Entity> i_charPtr, Sector* i_sectPtr, DIFFICULTY i_diff, Vector2f i_startPosition, RigidBody i_rb) :
	Enemy(i_charPtr, i_sectPtr, i_diff, i_startPosition, i_rb, ENEMY_RAND)
{
	sameDirTime = 0.9;
	timeTillDirSwitch = 0.0f;
	fillColor = CHARTREUSE;
	outlineColor = sf::Color::White;
	currDir = CreateRandomDir();
	SetDiffVars(i_diff);
}

CrazyBoi::~CrazyBoi()
{
}

void CrazyBoi::Update(float i_stepSize)
{
	//std::string str = "crazyStepSize: " + std::to_string(i_stepSize);
	//std::cout << str;
	UpdateHealth(i_stepSize);
	if (stunSecs < 0) {
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
	else {
		float secsInUpdate = i_stepSize / 1000.0f;
		stunSecs -= secsInUpdate;
	}
}

void CrazyBoi::CollideWithPainWall(PainWall * i_painWallPtr)
{
}

void CrazyBoi::SetDiffVars(int i_diff)
{
	switch (i_diff) {
	case EASY: {
		speed = ENEMYSPEEDEASY;
		break;
	}
	case MEDIUM: {
		speed = ENEMYSPEEDMED;
		break;
	}
	case HARD: {
		speed = ENEMYSPEEDHARD;
		break;
	}
	}
	speed *= 2;
}

/////////////////////// ShootyBoi class ///////////////////////
ShootyBoi::ShootyBoi(std::shared_ptr<Entity> i_charPtr, Sector * i_sectPtr, DIFFICULTY i_diff, Vector2f i_startPosition, RigidBody i_rb) :
	Enemy(i_charPtr, i_sectPtr, i_diff, i_startPosition, i_rb, ENEMY_BOSS)
{
	Stun(0.3f);
	sameDirTime = 0.3;
	timeTillDirSwitch = 0.0f;
	lastShotFired = hiResTime::now();
	shipRateOfFire = 1.0f;
	currDir = CreateRandomDir();
	SetDiffVars(i_diff);
}

void ShootyBoi::Update(float i_stepSize)
{
	UpdateHealth(i_stepSize);
	if (stunSecs < 0) {
		//apply impulse towards player
		Vector2f playerDir = charPtr->rb.transform.pos - rb.transform.pos;
		playerDir.normalize();
		float moveDist = (speed / 2.0f) * i_stepSize;
		rb.ApplyImpulse(playerDir * moveDist, NULL_VECTOR);
		if (timeTillDirSwitch < 0) {
			// also apply random impulse, lets see what happens lul
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
		//attempt to shoot projectiles all over the place
		shootProj();
	}
	else {
		float secsInUpdate = i_stepSize / 1000.0f;
		stunSecs -= secsInUpdate;
	}
}

void ShootyBoi::TakeDamage(float i_dmg)
{
	sectPtr->PlaySound(RESOURCES::OUCH3);
	health -= i_dmg;
}

void ShootyBoi::Destroy()
{
	sectPtr->PlaySound(RESOURCES::BOSSEXPLODE);
	killMeNextLoop = true;
}

void ShootyBoi::shootProj()
{
	weaponDelay = (std::chrono::duration_cast<std::chrono::microseconds>(hiResTime::now() - lastShotFired)).count() / 1000000.0f;
	if (weaponDelay >= shipRateOfFire) {
		sectPtr->PlaySound(RESOURCES::SWORD5);
		lastShotFired = hiResTime::now();
		int i = 0;
		while (i < numShots) {
		Vector2f projDir = CreateRandomDir();
		projDir.normalize();
		std::shared_ptr<Entity> projectile = std::make_shared<Projectile>(
			rb.transform.pos + (projDir * (100.0f + (i * 15.0f))), 1);
		projectile->rb.ApplyImpulse((projDir * 3000.0f), NULL_VECTOR);
		sectPtr->AddEntPtrToSector(projectile);
		i++;
		}
	}
}

void ShootyBoi::SetDiffVars(int i_diff)
{
	switch (i_diff) {
	case EASY: {
		speed = ENEMYSPEEDEASY;
		numShots = 6;
		maxHealth = 4;
		health = maxHealth;
		break;
	}
	case MEDIUM: {
		speed = ENEMYSPEEDMED;
		numShots = 7;
		maxHealth = 5;
		health = maxHealth;
		break;
	}
	case HARD: {
		speed = ENEMYSPEEDHARD;
		numShots = 8;
		maxHealth = 6;
		health = maxHealth;
		break;
	}
	}
	speed *= 3;
}


/////////////////////// Door Lockable class///////////////////////
Door::Door(Sector* i_sectPtr, MapCoord i_outCoord, Vector2f i_startPos, Vector2f i_outPos, RigidBody i_rb, SCREEN_SIDE i_side) :
	Entity(i_startPos, i_rb, DOOR_LOCKED), sectPtr{ i_sectPtr }, outCoord{ i_outCoord }, outPos{ i_outPos }, side { i_side }
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
Wall::Wall(Vector2f i_startPosition, Sector* i_sectPtr, RigidBody i_rb,
			sf::Color i_fillCol, sf::Color i_outCol) :
	Entity(i_startPosition, i_rb, WALL_BASIC), sectPtr{ i_sectPtr }
{
	fillColor = i_fillCol;
	outlineColor = i_outCol;
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
	fillColor = sf::Color::Black;
	outlineColor = sf::Color::White;
	active = false;
}

EndObject::~EndObject()
{
}

void EndObject::Update(float i_stepSize)
{
	if (sectPtr->firstPhase) {
		fillColor = sf::Color::Black;
		outlineColor = sf::Color::White;
		active = false;
	}
	else {
		fillColor = sf::Color::Magenta;
		outlineColor = sf::Color::Magenta;
		active = true;
	}
}


/////////////////////// Power Up ///////////////////////
PowerUp::PowerUp(Sector * i_sectPtr, Vector2f i_startPosition, UPGRADE_TYPE i_powType, RigidBody i_rb) :
	Entity(i_startPosition, i_rb, UPGRADE), sectPtr{ i_sectPtr }, powType { i_powType }
{
	switch (i_powType) {
	case (RATE_OF_FIRE): {
		fillColor = sf::Color(255, 128, 128);
		outlineColor = sf::Color::Magenta;
		break;
	}
	case (SCATTER): {
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