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
	else if (i_other.GetTypeID() == PROJ_WALL) {
		auto blocker = dynamic_cast<Blocker*>(&i_other);
		CollideWithBlocker(blocker);
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

void Entity::CollideWithBlocker(Blocker* i_blockPtr) {}

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
	shipRateOfAOE = 6.0f;
	shipSpeed = 100;
	lastShotFired = std::chrono::high_resolution_clock::now();
	lastAOEFired = std::chrono::high_resolution_clock::now();
	lastDamageReceived = std::chrono::high_resolution_clock::now();
	dmgRate = 1.0f;
	maxHealth = i_strtHealth;
	health = maxHealth;
	currSpecialAmmo = 3;
	weaponDelay = shipRateOfFire;
	wallWidth = 40;
	wallHeight = 270;
	BlastRadius = 185;
	blastStrength = 600.0f;
	blastStunTime = 4.0f;
	maxSpecialAmmo = 4;
	currSpecialAmmo = maxSpecialAmmo;
	weapSpeed = 3000.0f;
	InitLvl();
	rectPtr = dynamic_cast<Rectangle*>(rb.shape.get());
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
		if (shipLvl.at(RATE_OF_FIRE) < GLBVRS::GetUpgradeMax(RATE_OF_FIRE)) {
			shipLvl[RATE_OF_FIRE]++;
			shipRateOfFire *= 0.65f;
		}
		break;
	}
	case (WEAP_SPEED): { //number of shots
		if (shipLvl.at(WEAP_SPEED) < GLBVRS::GetUpgradeMax(WEAP_SPEED)) {
			shipLvl[WEAP_SPEED]++;
			weapSpeed *= 1.5;
		}
		break;
	}
	case (SCATTER): { //number of shots
		if (shipLvl.at(SCATTER) < GLBVRS::GetUpgradeMax(SCATTER)) {
			shipLvl[SCATTER]++;
			numShots += 1;
		}
		break;
	}
	case (SMALL_SHIP): { //number of shots
		if (shipLvl.at(SMALL_SHIP) < GLBVRS::GetUpgradeMax(SMALL_SHIP)) {
			shipLvl[BIG_SHIP] = 3;
			shipLvl[SMALL_SHIP]++;
			auto playerRect = dynamic_cast<Rectangle*>(rb.shape.get());
			rb.ChangeSizeOfShape(playerRect->GetWidth() - 15.0f, playerRect->GetHeight() - 15.0f);
		}
		break;
	}
	case (BIG_SHIP): { //number of shots
		if (shipLvl.at(BIG_SHIP) < GLBVRS::GetUpgradeMax(BIG_SHIP)) {
			shipLvl[SMALL_SHIP] = 3;
			shipLvl[BIG_SHIP]++;
			auto playerRect = dynamic_cast<Rectangle*>(rb.shape.get());
			rb.ChangeSizeOfShape(playerRect->GetWidth() + 15.0f, playerRect->GetHeight() + 15.0f);
			maxHealth += 6;
			AddHealth(6);
			shipSpeed += 20;
		}
		break;
	}
	case (BLAST): { //number of shots
		if (shipLvl.at(BLAST) < GLBVRS::GetUpgradeMax(BLAST)) {
			shipLvl[BLAST]++;
			BlastRadius *= 1.3f;
			blastStrength *= 1.3f;
			blastStunTime *= 1.3f;
			//maxSpecialAmmo += 2;
			shipRateOfAOE *= 0.70;
		}
		break;
	}
	case (WALL_BIG): { //number of shots
		if (shipLvl.at(WALL_BIG) < GLBVRS::GetUpgradeMax(WALL_BIG)) {
			shipLvl[WALL_BIG]++;
			wallWidth += 10;
			wallHeight += 60;
		}
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
	gamePtr->levels[gamePtr->currLvl]->GetSector(gamePtr->currSector)->PlaySound(RESOURCES::FIREBALL);
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
		auto width = rectPtr->GetWidth();
		float halfdiagLength = (SQRT_TWO * width) / 2.0f;
		float projCenterOffset = halfdiagLength + PROJECTILE_RADIUS;
		projDir.normalize();
		float prevAngleRads;
		Vector2f prevDirVect;
		float mouseAngleRad = Math::VectToAngle(projDir);
		float currAngleRads = mouseAngleRad - (0.0872665 * (numShots - 1));
		//Original angle is shifted back so cluster is still centered on cursor
		Vector2f currDirVect = Math::AngleToVect(currAngleRads);
		int i = 0;
		while (i < numShots) {
			std::shared_ptr<Entity> projectile = std::make_shared<Projectile>( // + (i * 15.0f)
					rb.transform.pos + (currDirVect * (projCenterOffset)));
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
		auto width = rectPtr->GetWidth();
		float halfdiagLength = (SQRT_TWO * width) / 2.0f;
		float projCenterOffset = halfdiagLength + (wallWidth / 2.0f) + 5.0f;
		gamePtr->levels[gamePtr->currLvl]->GetSector(gamePtr->currSector)->PlaySound(RESOURCES::SHOOT14);
		lastShotFired = hiResTime::now();
		Vector2f projectileDir = i_mousePos - rb.transform.pos;
		projectileDir.normalize();
		ShootWall(projectileDir);
		std::shared_ptr<Entity> projectile = std::make_shared<Blocker>(
								rb.transform.pos + (projectileDir * projCenterOffset),
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
{// AOE is a panic button it should not have a timer on it that kinda defeats the prupose,
	//JK everything needs a slight timer or else I'll fire them all at once
	weaponDelay = (std::chrono::duration_cast<std::chrono::microseconds>(hiResTime::now() - lastAOEFired)).count() / 1000000.0f;
	//if ((weaponDelay >= shipRateOfAOE) && (currSpecialAmmo > 0)) {
	if (weaponDelay >= shipRateOfAOE) {
	//if (currSpecialAmmo > 0) {
		gamePtr->levels[gamePtr->currLvl]->GetSector(gamePtr->currSector)->PlaySound(RESOURCES::MAGIC10);
		--currSpecialAmmo;
		lastAOEFired = hiResTime::now();
		std::shared_ptr<Entity> projectile = std::make_shared<Blast>(
					gamePtr->levels[gamePtr->GetCurrLvl()]->GetSector(gamePtr->currSector).get(), this,
					rb.transform.pos, 0, blastStrength, blastStunTime, RigidBody(std::make_shared<Circle>(BlastRadius), STATIC));
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
	if (i_projPtr->projType != projType) {
		Destroy();
	}
}

void Projectile::CollideWithBlocker(Blocker * i_blockPtr)
{
	if (projType == 1) {
		Destroy();
	}
}

void Projectile::CollideWithEnemy(Enemy * i_enemyPtr)
{
	if (i_enemyPtr->GetTypeID() == ENEMY_BOSS) {
		if (auto spawnerBoss = dynamic_cast<BossSpawn*>(i_enemyPtr)) {
			if (spawnerBoss->invulnerable) {

			}
			else {
				Destroy();
			}
		}
		else {
			Destroy();
		}
	}
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
Blast::Blast(Sector* i_sectPtr, PlayerChar* charP, Vector2f i_startPosition, int i_blastType, float i_strength, float i_stunTime, RigidBody i_rb) :
	Entity(i_startPosition, i_rb, BLAST_STUN),
	sectPtr{ i_sectPtr }, blastType{ i_blastType },
	strength { i_strength }, stunTime { i_stunTime },
	charP { charP }
{
	fillColor = sf::Color(0, 255, 255, 128);
	outlineColor = sf::Color::Cyan;
	intangible = true;
	deathTimer = 0.7f;
}

Blast::~Blast()
{
}

void Blast::Update(float i_stepSize)
{
	rb.transform.pos = charP->rb.transform.pos;
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
	Stun(0.6f);
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
	sectPtr->PlaySound(RESOURCES::FIREBALL);
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
	if ((i_blastPtr->blastType == 0) && (stunSecs <= 0.0)) {
		Stun(i_blastPtr->stunTime);
		//Vector2f blastPos = i_blastPtr->rb.transform.pos;
		Vector2f blastDir = rb.transform.pos - i_blastPtr->rb.transform.pos;
		float proximityBonus = std::abs(blastDir.norm()) * (i_blastPtr->strength * (1.0f / 4.0f));
		blastDir.normalize();
		rb.ApplyImpulse((blastDir * (i_blastPtr->strength + proximityBonus)), NULL_VECTOR);
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
	//sectPtr->PlaySound(RESOURCES::FIRE5);
	TakeDamage(1);
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

/////////////////////// Burst Boss class ///////////////////////
BossBurst::BossBurst(std::shared_ptr<Entity> i_charPtr, Sector * i_sectPtr, DIFFICULTY i_diff, Vector2f i_startPosition, RigidBody i_rb) :
	Enemy(i_charPtr, i_sectPtr, i_diff, i_startPosition, i_rb, ENEMY_BOSS)
{
	Stun(0.3f);
	sameDirTime = 0.3;
	timeTillDirSwitch = 0.0f;
	lastShotFired = hiResTime::now();
	shipRateOfFire = 1.0f;
	currDir = CreateRandomDir();
	SetDiffVars(i_diff, i_sectPtr->myLevel->m_lvl_num);
}

void BossBurst::Update(float i_stepSize)
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

void BossBurst::TakeDamage(float i_dmg)
{
	sectPtr->PlaySound(RESOURCES::OUCH3);
	health -= i_dmg;
}

void BossBurst::Destroy()
{
	sectPtr->PlaySound(RESOURCES::BOSSEXPLODE);
	killMeNextLoop = true;
}

void BossBurst::shootProj()
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

void BossBurst::Stun(float i_stunTime)
{
	stunSecs = i_stunTime / 2.0f;
}

void BossBurst::SetDiffVars(int i_diff, int i_lvlNum)
{
	switch (i_diff) {
	case EASY: {
		speed = ENEMYSPEEDEASY;
		numShots = 6;
		maxHealth = 7;
		health = maxHealth;
		break;
	}
	case MEDIUM: {
		speed = ENEMYSPEEDMED;
		numShots = 7;
		maxHealth = 8;
		health = maxHealth;
		break;
	}
	case HARD: {
		speed = ENEMYSPEEDHARD;
		numShots = 8;
		maxHealth = 9;
		health = maxHealth;
		break;
	}
	}
	speed *= 3;
}

/////////////////////// Stream Boss class ///////////////////////
BossStream::BossStream(std::shared_ptr<Entity> i_charPtr, Sector * i_sectPtr, DIFFICULTY i_diff, Vector2f i_startPosition, RigidBody i_rb) :
	Enemy(i_charPtr, i_sectPtr, i_diff, i_startPosition, i_rb, ENEMY_BOSS)
{
	Stun(0.3f);
	sameDirTime = 0.2;
	timeTillDirSwitch = 0.0f;
	lastShotFired = hiResTime::now();
	shipRateOfFire = 0.05f;
	currDir = CreateRandomDir();
	SetDiffVars(i_diff, i_sectPtr->myLevel->m_lvl_num);
}

void BossStream::Update(float i_stepSize)
{
	UpdateHealth(i_stepSize);
	if (stunSecs < 0) {
		rb.angVel = 0.03f;
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
		shootProj();
	}
	else {
		float secsInUpdate = i_stepSize / 1000.0f;
		stunSecs -= secsInUpdate;
	}
}

void BossStream::TakeDamage(float i_dmg)
{
	sectPtr->PlaySound(RESOURCES::OUCH3);
	health -= i_dmg;
}

void BossStream::Destroy()
{
	sectPtr->PlaySound(RESOURCES::BOSSEXPLODE);
	killMeNextLoop = true;
}

void BossStream::shootProj()
{
	weaponDelay = (std::chrono::duration_cast<std::chrono::microseconds>(hiResTime::now() - lastShotFired)).count() / 1000000.0f;
	if (weaponDelay >= shipRateOfFire) {
		sectPtr->PlaySound(RESOURCES::PLAYERSHOOT);
		lastShotFired = hiResTime::now();
			Vector2f projDir = Math::AngleToVect(rb.transform.orient);
			projDir.normalize();
			std::shared_ptr<Entity> projectile = std::make_shared<Projectile>(
				rb.transform.pos + (projDir * (100.0f)), 1);
			projectile->rb.ApplyImpulse((projDir * 3000.0f), NULL_VECTOR);
			sectPtr->AddEntPtrToSector(projectile);
	}
}

void BossStream::Stun(float i_stunTime)
{
	stunSecs = i_stunTime / 2.0f;
}

void BossStream::SetDiffVars(int i_diff, int i_lvlNum)
{
	switch (i_diff) {
	case EASY: {
		speed = ENEMYSPEEDEASY;
		numShots = 6;
		maxHealth = 9;
		health = maxHealth;
		break;
	}
	case MEDIUM: {
		speed = ENEMYSPEEDMED;
		numShots = 7;
		maxHealth = 10;
		health = maxHealth;
		break;
	}
	case HARD: {
		speed = ENEMYSPEEDHARD;
		numShots = 8;
		maxHealth =11;
		health = maxHealth;
		break;
	}
	}
	speed *= 4;
}

/////////////////////// Rush Boss class ///////////////////////
BossRush::BossRush(std::shared_ptr<Entity> i_charPtr, Sector * i_sectPtr, DIFFICULTY i_diff, Vector2f i_startPosition, RigidBody i_rb) :
	Enemy(i_charPtr, i_sectPtr, i_diff, i_startPosition, i_rb, ENEMY_BOSS)
{
	Stun(1.0f);
	rushTime = 3.0f;
	waitTime = 1.0f;
	rushing = true;
	timeTillSwitch = rushTime;
	lastShotFired = hiResTime::now();
	shipRateOfFire = 1.0f;
	currDir = CreateRandomDir();
	SetDiffVars(i_diff, i_sectPtr->myLevel->m_lvl_num);
}

void BossRush::Update(float i_stepSize)
{
	UpdateHealth(i_stepSize);
	if (stunSecs < 0) {
		float secsInUpdate = i_stepSize / 1000.0f;
		timeTillSwitch -= secsInUpdate;
		if (rushing) {
			Vector2f playerDir = charPtr->rb.transform.pos - rb.transform.pos;
			playerDir.normalize();
			float moveDist = (speed / 2.0f) * i_stepSize;
			rb.ApplyImpulse(playerDir * moveDist, NULL_VECTOR);
			if (timeTillSwitch < 0) {
				// also apply random impulse, lets see what happens lul
				rushing = false;
				timeTillSwitch = waitTime;
			}
		}
		else {
			rb.vel -= (rb.vel * secsInUpdate * (1.0f / waitTime));
			if (timeTillSwitch < 0) {
				// also apply random impulse, lets see what happens lul
				rushing = true;
				timeTillSwitch = rushTime;
			}
		}
	}
	else {
		float secsInUpdate = i_stepSize / 1000.0f;
		stunSecs -= secsInUpdate;
	}
}

void BossRush::Destroy()
{
	sectPtr->PlaySound(RESOURCES::BOSSEXPLODE);
	killMeNextLoop = true;
}

void BossRush::TakeDamage(float i_dmg)
{
	sectPtr->PlaySound(RESOURCES::OUCH3);
	health -= i_dmg;
}

void BossRush::shootProj()
{
}

void BossRush::Stun(float i_stunTime)
{
	stunSecs = i_stunTime / 2.0f;
}

void BossRush::SetDiffVars(int i_diff, int i_lvlNum)
{
	switch (i_diff) {
	case EASY: {
		speed = ENEMYSPEEDEASY;
		numShots = 6;
		maxHealth = 8;
		health = maxHealth;
		break;
	}
	case MEDIUM: {
		speed = ENEMYSPEEDMED;
		numShots = 7;
		maxHealth = 9;
		health = maxHealth;
		break;
	}
	case HARD: {
		speed = ENEMYSPEEDHARD;
		numShots = 8;
		maxHealth = 10;
		health = maxHealth;
		break;
	}
	}
	speed *= 20;
}

void BossRush::CollideWithPainWall(PainWall * i_painWallPtr)
{
	//sectPtr->PlaySound(RESOURCES::FIRE5);
	//TakeDamage(1);
}

void BossRush::CollideWithPlayer(PlayerChar * i_playerPtr)
{
		i_playerPtr->TakeDamage(1.0f);
		//rushing = false;
		//timeTillDirSwitch = sameDirTime;
}

/////////////////////// Split Boss class ///////////////////////
BossSplit::BossSplit(std::shared_ptr<Entity> i_charPtr, Sector * i_sectPtr, DIFFICULTY i_diff, int i_splitsLeft, float i_spdFct, bool i_crazy, Vector2f i_startPosition, RigidBody i_rb) :
	Enemy(i_charPtr, i_sectPtr, i_diff, i_startPosition, i_rb, ENEMY_BOSS), splitsLeft { i_splitsLeft }, diff { i_diff }, crazy {i_crazy}
{
	speed = i_spdFct;
	Stun(0.05f);
	sameDirTime = 0.6;
	timeTillDirSwitch = 0.0f;
	lastShotFired = hiResTime::now();
	shipRateOfFire = 1.0f;
	currDir = CreateRandomDir();
	SetDiffVars(i_diff, i_sectPtr->myLevel->m_lvl_num);
}

void BossSplit::Update(float i_stepSize)
{
	UpdateHealth(i_stepSize);
	if (stunSecs < 0) {
		if (crazy) {
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
			Vector2f playerDir = charPtr->rb.transform.pos - rb.transform.pos;
			playerDir.normalize();
			float moveDist = speed * i_stepSize;
			rb.ApplyImpulse(playerDir * moveDist, NULL_VECTOR);
		}
	}
	else {
		float secsInUpdate = i_stepSize / 1000.0f;
		stunSecs -= secsInUpdate;
	}
}

void BossSplit::Destroy()
{
	switch (splitsLeft) {
	case 4: {
		sectPtr->PlaySound(RESOURCES::PLOPBANANA);
		std::shared_ptr<Shape> shape1 = std::make_shared<Circle>(100);
		std::shared_ptr<Shape> shape2 = std::make_shared<Circle>(100);
		RigidBody projBody1 = RigidBody(shape1, LESSBOUNCYBALL);
		RigidBody projBody2 = RigidBody(shape2, LESSBOUNCYBALL);
		std::shared_ptr<Entity> split1 = std::make_shared<BossSplit>(charPtr, sectPtr, diff, splitsLeft - 1, 4, true, rb.transform.pos, projBody1);
		std::shared_ptr<Entity> split2 = std::make_shared<BossSplit>(charPtr, sectPtr, diff, splitsLeft - 1, 4, false, rb.transform.pos, projBody2);
		sectPtr->AddEntPtrToSector(split1);
		sectPtr->AddEntPtrToSector(split2);
		killMeNextLoop = true;
		break;
	}
	case 3: {
		sectPtr->PlaySound(RESOURCES::PLOPBANANA);
		std::shared_ptr<Shape> shape1 = std::make_shared<Circle>(75);
		std::shared_ptr<Shape> shape2 = std::make_shared<Circle>(75);
		RigidBody projBody1 = RigidBody(shape1, LESSBOUNCYBALL);
		RigidBody projBody2 = RigidBody(shape2, LESSBOUNCYBALL);
		std::shared_ptr<Entity> split1 = std::make_shared<BossSplit>(charPtr, sectPtr, diff, splitsLeft - 1, 3, true, rb.transform.pos, projBody1);
		std::shared_ptr<Entity> split2 = std::make_shared<BossSplit>(charPtr, sectPtr, diff, splitsLeft - 1, 3, false, rb.transform.pos, projBody2);
		sectPtr->AddEntPtrToSector(split1);
		sectPtr->AddEntPtrToSector(split2);
		killMeNextLoop = true; 
		break;
	}
	case 2: {
		sectPtr->PlaySound(RESOURCES::PLOPBANANA);
		std::shared_ptr<Shape> shape1 = std::make_shared<Circle>(50);
		std::shared_ptr<Shape> shape2 = std::make_shared<Circle>(50);
		RigidBody projBody1 = RigidBody(shape1, LESSBOUNCYBALL);
		RigidBody projBody2 = RigidBody(shape2, LESSBOUNCYBALL);
		std::shared_ptr<Entity> split1 = std::make_shared<BossSplit>(charPtr, sectPtr, diff, splitsLeft - 1, 2, true, rb.transform.pos, projBody1);
		std::shared_ptr<Entity> split2 = std::make_shared<BossSplit>(charPtr, sectPtr, diff, splitsLeft - 1, 2, false, rb.transform.pos, projBody2);
		sectPtr->AddEntPtrToSector(split1);
		sectPtr->AddEntPtrToSector(split2);
		killMeNextLoop = true; 
		break;
	}
	case 1: {
		sectPtr->PlaySound(RESOURCES::PLOPBANANA);
		std::shared_ptr<Shape> shape1 = std::make_shared<Circle>(30);
		std::shared_ptr<Shape> shape2 = std::make_shared<Circle>(30);
		RigidBody projBody1 = RigidBody(shape1, BOUNCYBALL);
		RigidBody projBody2 = RigidBody(shape2, BOUNCYBALL);
		std::shared_ptr<Entity> split1 = std::make_shared<BossSplit>(charPtr, sectPtr, diff, splitsLeft - 1, true, 1, rb.transform.pos, projBody1);
		std::shared_ptr<Entity> split2 = std::make_shared<BossSplit>(charPtr, sectPtr, diff, splitsLeft - 1, false, 1, rb.transform.pos, projBody2);
		sectPtr->AddEntPtrToSector(split1);
		sectPtr->AddEntPtrToSector(split2);
		killMeNextLoop = true;
		break;
	}
	case 0: {
		sectPtr->PlaySound(RESOURCES::PLOP);
		killMeNextLoop = true;
	}
	}

}

void BossSplit::TakeDamage(float i_dmg)
{
	health -= i_dmg;
}

void BossSplit::shootProj()
{
}

void BossSplit::Stun(float i_stunTime)
{
	stunSecs = i_stunTime / 2.0f;
}

void BossSplit::SetDiffVars(int i_diff, int i_lvlNum)
{
	if (splitsLeft == 4)
	switch (i_diff) {
	case EASY: {
		speed *= ENEMYSPEEDEASY;
		numShots = 6;
		maxHealth = 2;
		health = maxHealth;
		break;
	}
	case MEDIUM: {
		speed *= ENEMYSPEEDMED;
		numShots = 7;
		maxHealth = 3;
		health = maxHealth;
		break;
	}
	case HARD: {
		speed *= ENEMYSPEEDHARD;
		numShots = 8;
		maxHealth = 4;
		health = maxHealth;
		break;
	}
	}
	else if (splitsLeft == 3)
		switch (i_diff) {
		case EASY: {
			speed *= ENEMYSPEEDEASY;
			numShots = 6;
			maxHealth = 2;
			health = maxHealth;
			break;
		}
		case MEDIUM: {
			speed *= ENEMYSPEEDMED;
			numShots = 7;
			maxHealth = 3;
			health = maxHealth;
			break;
		}
		case HARD: {
			speed *= ENEMYSPEEDHARD;
			numShots = 8;
			maxHealth = 3;
			health = maxHealth;
			break;
		}
		}
	if (splitsLeft == 2)
		switch (i_diff) {
		case EASY: {
			speed *= ENEMYSPEEDEASY;
			numShots = 6;
			maxHealth = 1;
			health = maxHealth;
			break;
		}
		case MEDIUM: {
			speed *= ENEMYSPEEDMED;
			numShots = 7;
			maxHealth = 2;
			health = maxHealth;
			break;
		}
		case HARD: {
			speed *= ENEMYSPEEDHARD;
			numShots = 8;
			maxHealth = 2;
			health = maxHealth;
			break;
		}
		}
	if (splitsLeft == 1)
		switch (i_diff) {
		case EASY: {
			speed *= ENEMYSPEEDEASY;
			numShots = 6;
			maxHealth = 1;
			health = maxHealth;
			break;
		}
		case MEDIUM: {
			speed *= ENEMYSPEEDMED;
			numShots = 7;
			maxHealth = 1;
			health = maxHealth;
			break;
		}
		case HARD: {
			speed *= ENEMYSPEEDHARD;
			numShots = 8;
			maxHealth = 1;
			health = maxHealth;
			break;
		}
		}
	if (splitsLeft == 0)
		switch (i_diff) {
		case EASY: {
			speed *= ENEMYSPEEDEASY;
			numShots = 6;
			maxHealth = 1;
			health = maxHealth;
			break;
		}
		case MEDIUM: {
			speed *= ENEMYSPEEDMED;
			numShots = 7;
			maxHealth = 1;
			health = maxHealth;
			break;
		}
		case HARD: {
			speed *= ENEMYSPEEDHARD;
			numShots = 8;
			maxHealth = 1;
			health = maxHealth;
			break;
		}
		}
}

/////////////////////// Spawn Boss class ///////////////////////
BossSpawn::BossSpawn(std::shared_ptr<Entity> i_charPtr, Sector * i_sectPtr, DIFFICULTY i_diff, Vector2f i_startPosition, RigidBody i_rb) :
	Enemy(i_charPtr, i_sectPtr, i_diff, i_startPosition, i_rb, ENEMY_BOSS), diff { i_diff }
{
	invulnerable = true;
	Stun(0.5f);
	lastShotFired = hiResTime::now();
	shipRateOfFire = 1.0f;
	currDir = CreateRandomDir();
	SetDiffVars(i_diff, i_sectPtr->myLevel->m_lvl_num);
}

void BossSpawn::Update(float i_stepSize)
{
	if (sectPtr->sectEnemyNum < 2) {
		invulnerable = false;
		fillColor = EMERALD;
		outlineColor = PISTACHIO;
	}
	else {
		invulnerable = true;
		fillColor = SILVER;
		outlineColor = DIMGRAY;
	}
	UpdateHealth(i_stepSize);
	if (stunSecs < 0) {
			if (timeTillDirSwitch < 0) {
				if (sectPtr->sectEnemyNum < 2) {
					timeTillDirSwitch = sameDirTime;
					sectPtr->PlaySound(RESOURCES::BRAKE);
					sectPtr->GenerateEnemies(numShots, ENEMY_RAND, CENTER, 1, diff, 0);
					sectPtr->GenerateEnemies(numShots, ENEMY_SEEK, CENTER, 1, diff, 0);
				}
				else {
					timeTillDirSwitch = sameDirTime;
					sectPtr->PlaySound(RESOURCES::BRAKE);
					sectPtr->GenerateEnemies(std::max((int)numShots-2, 1), ENEMY_RAND, CENTER, 1, diff, 0);
					sectPtr->GenerateEnemies(std::max((int)numShots - 2, 1), ENEMY_SEEK, CENTER, 1, diff, 0);
				}
				}
			else {
				float secsInUpdate = i_stepSize / 1000.0f;
				timeTillDirSwitch -= secsInUpdate;
			}
		}
	else {
		float secsInUpdate = i_stepSize / 1000.0f;
		stunSecs -= secsInUpdate;
	}
}

void BossSpawn::Destroy()
{
	sectPtr->PlaySound(RESOURCES::BOSSEXPLODE);
	killMeNextLoop = true;
}

void BossSpawn::TakeDamage(float i_dmg)
{
	if (!invulnerable) {
		int xasd = 5;
		sectPtr->PlaySound(RESOURCES::OUCH3);
		health -= i_dmg;
	}
	int x = 5;
	int xasd = 5;
	int xa = 5;
	int xq = 5;
	if (invulnerable) {
		int x = 5;
		sectPtr->PlaySound(RESOURCES::SWORDCLASH);
	}
}

void BossSpawn::shootProj()
{
}

void BossSpawn::Stun(float i_stunTime)
{
	stunSecs = i_stunTime / 2.0f;
}

void BossSpawn::SetDiffVars(int i_diff, int i_lvlNum)
{
	switch (i_diff) {
	case EASY: {
		numShots = 4;
		maxHealth = 16;
		sameDirTime = 14.0;
		timeTillDirSwitch = 0.0f;
		health = maxHealth;
		break;
	}
	case MEDIUM: {
		numShots = 5;
		maxHealth = 18;
		sameDirTime = 10.0;
		timeTillDirSwitch = 0.0f;
		health = maxHealth;
		break;
	}
	case HARD: {
		numShots = 6;
		maxHealth = 10;
		sameDirTime = 8.0;
		timeTillDirSwitch = 0.0f;
		health = maxHealth;
		break;
	}
	}
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
	colorSwitchRate = 1.5;
	timeSinceColorSwitch = 0.0f;
	colorState = true;
	fillColor = sf::Color::Red;
	outlineColor = sf::Color::Black;
}

PainWall::~PainWall()
{
}


void PainWall::Update(float i_stepSize)
{
	if (timeSinceColorSwitch < 0) {
		timeSinceColorSwitch = colorSwitchRate;
		fillColor = sf::Color::Red;
		if (colorState) {
			fillColor = sf::Color::Yellow;
			colorState = false;
		}
		else {
			fillColor = sf::Color::Red;
			colorState = true;
		}

	}
	else {
		float secsInUpdate = i_stepSize / 1000.0f;
		timeSinceColorSwitch -= secsInUpdate;
	}
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
		fillColor = sf::Color::Cyan;
		outlineColor = sf::Color::Magenta;
		active = true;
		rb.angVel = 1.0f;
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
