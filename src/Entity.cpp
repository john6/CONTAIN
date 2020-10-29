//I have to include a bunch of header files or else I get the circular header file reference issue,
// because my Entities and Sector and Level and Game all reference each other whoopsidasies
#include <CONTAIN/Entity.h>
#include <CONTAIN/Game.h>
#include <CONTAIN/Level.h>
#include <CONTAIN/Sector.h>

///////////////////////Entity///////////////////////
Entity::Entity() :
	rb { RigidBody(std::make_shared<Rectangle>(100.0f, 100.0f)) }, 
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
	rb { i_rb }, typeID{ i_typeID }
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
	else if (i_other.GetTypeID() == DOOR) {
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
	drawShape->setOrigin(sf::Vector2f(rb.shape->GetSFMLOriginOffset()[0], rb.shape->GetSFMLOriginOffset()[1]));
	drawShape->setPosition(lerpPos[0], lerpPos[1]);
	drawShape->setRotation((lerpOrient*180.0f) / PI);
	drawShape->setFillColor(fillColor);
	drawShape->setOutlineColor(outlineColor);
	drawShape->setOutlineThickness(borderThickness);
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

///////////////////////PlayerChar class ///////////////////////
PlayerChar::PlayerChar(int i_strtHealth, Vector2f i_startPosition, RigidBody i_rb) :
	Entity(i_startPosition, i_rb, PLAYER), pController{ PlayerController(GLBVRS::GPTR->renderWindow) }
{
	gamePtr = GLBVRS::GPTR;
	numShots = 1;
	fillColor = GUNMETAL;
	outlineColor = FORDSILVER;
	shipRateOfFire = 0.9f;
	shipRateOfFireWall = 1.3;
	wallDelay = shipRateOfFireWall;
	weaponDelay = shipRateOfFire;
	shipRateOfAOE = 6.0f;
	shipSpeed = 100 * std::pow(GLBVRS::SIZE_RAT, 2);
	size_t minutes = 255;
	lastShotFired = std::chrono::high_resolution_clock::now() - std::chrono::minutes(1);
	lastWallFired = std::chrono::high_resolution_clock::now() - std::chrono::minutes(1);
	lastAOEFired = std::chrono::high_resolution_clock::now() - std::chrono::minutes(1);
	lastDamageReceived = std::chrono::high_resolution_clock::now() - std::chrono::minutes(1);
	dmgRate = 1.0f;
	maxHealth = i_strtHealth;
	health = maxHealth;
	currSpecialAmmo = 3;

	wallWidth = 40 * GLBVRS::SIZE_RAT;
	wallHeight = 270 * GLBVRS::SIZE_RAT;
	BlastRadius = 185 * GLBVRS::SIZE_RAT;
	blastStrength = 600.0f * GLBVRS::SIZE_RAT;
	blastStunTime = 4.0f;
	maxSpecialAmmo = 4;
	currSpecialAmmo = maxSpecialAmmo;
	weapSpeed = 3000.0f * std::pow(GLBVRS::SIZE_RAT, 2);
	InitLvl();
	rectPtr = dynamic_cast<Rectangle*>(rb.shape.get());
	hasVisuals = true;
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
	health = std::min(health + i_healthUp, maxHealth);
}

void PlayerChar::AcceptWeaponInput(float i_stepSize)
{
	Vector2f leftMousePos = pController.LeftClick();
	if (leftMousePos[0] > 0 && leftMousePos[1] > 0) {
		ShootBasic(leftMousePos);
	}
	Vector2f rightMousePos = pController.RightClick();
	if (rightMousePos[0] > 0 && rightMousePos[1] > 0) {
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
			weapSpeed *= 1.85;
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
			auto playerPoly = dynamic_cast<Polygon*>(rb.shape.get());
			playerPoly->ResizeMutliple(0.80f);
			rb.SetMassData();
			rb.objVerts = playerPoly->GetPoints();
			//b.ChangeSizeOfShape(playerRect->GetWidth() - 15.0f, playerRect->GetHeight() - 15.0f);
		}
		break;
	}
	case (BIG_SHIP): { //number of shots
		if (shipLvl.at(BIG_SHIP) < GLBVRS::GetUpgradeMax(BIG_SHIP)) {
			shipLvl[SMALL_SHIP] = 3;
			shipLvl[BIG_SHIP]++;
			auto playerPoly = dynamic_cast<Polygon*>(rb.shape.get());
			playerPoly->ResizeMutliple(1.10f);
			rb.SetMassData();
			rb.objVerts = playerPoly->GetPoints();
			maxHealth += 7;
			AddHealth(6);
			shipSpeed *= 1.3;
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
	case (TEMP_HEALTH): { //number of shots
		if (shipLvl.at(WALL_BIG) < GLBVRS::GetUpgradeMax(WALL_BIG)) {
			AddHealth(3);
		}
		break;
	}
	case (WALL_BIG): { //number of shots
		if (shipLvl.at(WALL_BIG) < GLBVRS::GetUpgradeMax(WALL_BIG)) {
			shipLvl[WALL_BIG]++;
			//wallWidth += 10;
			//wallHeight += 60;
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
		rb.ResetPosition(i_doorPtr->GetOutPos());
		gamePtr->RequestTravelToSector(i_doorPtr->GetOutCoord());
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

void PlayerChar::UpdateVisuals(float i_stepSize)
{
	auto poly = dynamic_cast<Polygon *> (rb.shape.get());
	float baseDiam = poly->GetDistToCorner() * (3.0f / 12.0f);
	//Cannon Shaft
	std::shared_ptr<sf::Shape> drawShapeShaft = std::make_shared<sf::RectangleShape>(sf::Vector2f(poly->GetDistToCorner(), baseDiam));
	drawShapeShaft->setOrigin(sf::Vector2f(0.0f, baseDiam / 2.0f));
	drawShapeShaft->setFillColor(fillColor);
	drawShapeShaft->setOutlineColor(outlineColor);
	drawShapeShaft->setPosition(sf::Vector2f(rb.transform.pos[0], rb.transform.pos[1]));
	drawShapeShaft->setOutlineThickness(2.0f);
	Vector2f mosDir = pController.GetMousePos() - rb.transform.pos;
	float shootDir = Math::VectToAngle((mosDir).normalized()) * 57.2958;
	drawShapeShaft->setRotation(shootDir);
	visuals->emplace_back(drawShapeShaft);
	//Center Circle
	float radius = baseDiam * (3.0f / 5.0f);
	std::shared_ptr<sf::Shape> drawShapeBase = std::make_shared<sf::CircleShape>(radius);
	drawShapeBase->setOrigin(sf::Vector2f(radius, radius));
	drawShapeBase->setFillColor(outlineColor);
	drawShapeBase->setOutlineColor(outlineColor);
	drawShapeBase->setPosition(sf::Vector2f(rb.transform.pos[0], rb.transform.pos[1]));
	drawShapeBase->setOutlineThickness(2.0f);
	visuals->emplace_back(drawShapeBase);
}

void PlayerChar::ShootBasic(Vector2f i_mousePos)
{
	weaponDelay = (std::chrono::duration_cast<std::chrono::microseconds>(hiResTime::now() - lastShotFired)).count() / 1000000.0f;
	if (weaponDelay >= shipRateOfFire) {
		gamePtr->levels[gamePtr->currLvl]->GetSector(gamePtr->currSector)->PlaySound(RESOURCES::SHOT2);
		lastShotFired = hiResTime::now();
		Vector2f projDir = i_mousePos - rb.transform.pos; 
		auto poly = dynamic_cast<Polygon *> (rb.shape.get());
		float projCenterOffset = poly->GetDistToCorner() + GLBVRS::PROJECTILE_RADIUS;
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
				gamePtr->levels[gamePtr->GetCurrLvl()]->GetSector(gamePtr->currSector).get(),
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
	wallDelay = (std::chrono::duration_cast<std::chrono::microseconds>(hiResTime::now() - lastWallFired)).count() / 1000000.0f;
	if (wallDelay >= shipRateOfFireWall) {
		lastWallFired = hiResTime::now();
		auto poly = dynamic_cast<Polygon *> (rb.shape.get());
		float projCenterOffset = poly->GetDistToCorner() + GLBVRS::PROJECTILE_RADIUS;
		gamePtr->levels[gamePtr->currLvl]->GetSector(gamePtr->currSector)->PlaySound(RESOURCES::SHOOT14);
		Vector2f projectileDirFront = (i_mousePos - rb.transform.pos);
		projectileDirFront.normalize();
		Vector2f projectileDirBack = -1 * (i_mousePos - rb.transform.pos);
		projectileDirBack.normalize();
		Vector2f projectileDirLeft = (i_mousePos - rb.transform.pos);
		projectileDirLeft = Vector2f(projectileDirLeft[1], -1 * projectileDirLeft[0]);
		projectileDirLeft.normalize();
		Vector2f projectileDirRight = (i_mousePos - rb.transform.pos);
		projectileDirRight = Vector2f(-1 * projectileDirRight[1], projectileDirRight[0]);
		projectileDirRight.normalize();

		if (shipLvl[WALL_BIG] == 0) {
			std::shared_ptr<Entity> projectile = std::make_shared<Blocker>(
				rb.transform.pos + (projectileDirFront * projCenterOffset),
				RigidBody(std::make_shared<Rectangle>(wallWidth, wallHeight), HEAVYBALL));
			projectile->rb.ApplyImpulse((projectileDirFront * 7000.0f * GLBVRS::SIZE_RAT), NULL_VECTOR);
			projectile->rb.ResetOrientation(projectileDirFront);
			gamePtr->levels[gamePtr->GetCurrLvl()]->GetSector(gamePtr->currSector)->AddEntPtrToSector(projectile);
		}

		else if (shipLvl[WALL_BIG] == 1) {
			std::shared_ptr<Entity> projectile1 = std::make_shared<Blocker>(
				rb.transform.pos + (projectileDirFront * projCenterOffset),
				RigidBody(std::make_shared<Rectangle>(wallWidth, wallHeight + (wallWidth * 2.0f)), HEAVYBALL));
			projectile1->rb.ApplyImpulse((projectileDirFront * 7000.0f * GLBVRS::SIZE_RAT), NULL_VECTOR);
			projectile1->rb.ResetOrientation(projectileDirFront);
			gamePtr->levels[gamePtr->GetCurrLvl()]->GetSector(gamePtr->currSector)->AddEntPtrToSector(projectile1);

			std::shared_ptr<Entity> projectile2 = std::make_shared<Blocker>(
				rb.transform.pos + (projectileDirBack * projCenterOffset),
				RigidBody(std::make_shared<Rectangle>(wallWidth, wallHeight), HEAVYBALL));
			projectile2->rb.ApplyImpulse((projectileDirBack * 7000.0f * GLBVRS::SIZE_RAT), NULL_VECTOR);
			projectile2->rb.ResetOrientation(projectileDirBack);
			gamePtr->levels[gamePtr->GetCurrLvl()]->GetSector(gamePtr->currSector)->AddEntPtrToSector(projectile2);
		}
		else if (shipLvl[WALL_BIG] == 2) {
			std::shared_ptr<Entity> projectile1 = std::make_shared<Blocker>(
				rb.transform.pos + (projectileDirFront * projCenterOffset),
				RigidBody(std::make_shared<Rectangle>(wallWidth, wallHeight + (wallWidth * 2.0f)), HEAVYBALL));
			projectile1->rb.ApplyImpulse((projectileDirFront * 7000.0f * GLBVRS::SIZE_RAT), NULL_VECTOR);
			projectile1->rb.ResetOrientation(projectileDirFront);
			gamePtr->levels[gamePtr->GetCurrLvl()]->GetSector(gamePtr->currSector)->AddEntPtrToSector(projectile1);

			std::shared_ptr<Entity> projectile2 = std::make_shared<Blocker>(
				rb.transform.pos + (projectileDirBack * projCenterOffset),
				RigidBody(std::make_shared<Rectangle>(wallWidth, wallHeight + (wallWidth * 2.0f)), HEAVYBALL));
			projectile2->rb.ApplyImpulse((projectileDirBack * 7000.0f * GLBVRS::SIZE_RAT), NULL_VECTOR);
			projectile2->rb.ResetOrientation(projectileDirBack);
			gamePtr->levels[gamePtr->GetCurrLvl()]->GetSector(gamePtr->currSector)->AddEntPtrToSector(projectile2);

			std::shared_ptr<Entity> projectile3 = std::make_shared<Blocker>(
				rb.transform.pos + (projectileDirLeft * projCenterOffset),
				RigidBody(std::make_shared<Rectangle>(wallWidth, wallHeight - (wallWidth * 2.0f)), HEAVYBALL));
			projectile3->rb.ApplyImpulse((projectileDirLeft * 7000.0f * GLBVRS::SIZE_RAT), NULL_VECTOR);
			projectile3->rb.ResetOrientation(projectileDirLeft);
			gamePtr->levels[gamePtr->GetCurrLvl()]->GetSector(gamePtr->currSector)->AddEntPtrToSector(projectile3);

			std::shared_ptr<Entity> projectile4 = std::make_shared<Blocker>(
				rb.transform.pos + (projectileDirRight * projCenterOffset),
				RigidBody(std::make_shared<Rectangle>(wallWidth, wallHeight - (wallWidth * 2.0f)), HEAVYBALL));
			projectile4->rb.ApplyImpulse((projectileDirRight * 7000.0f * GLBVRS::SIZE_RAT), NULL_VECTOR);
			projectile4->rb.ResetOrientation(projectileDirRight);
			gamePtr->levels[gamePtr->GetCurrLvl()]->GetSector(gamePtr->currSector)->AddEntPtrToSector(projectile4);
		}
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
					gamePtr->levels[gamePtr->GetCurrLvl()]->GetSector(gamePtr->currSector).get(),
					rb.transform.pos, 0, blastStrength, blastStunTime, RigidBody(std::make_shared<Circle>(BlastRadius), STATIC));
		gamePtr->levels[gamePtr->GetCurrLvl()]->GetSector(gamePtr->currSector)->AddEntPtrToSector(projectile);
	}
}

///////////////////////Basic Projectile Class ///////////////////////
Projectile::Projectile(Sector* i_sectPtr, Vector2f i_startPosition, int i_projType, RigidBody i_rb) :
	Entity(i_startPosition, i_rb, PROJ_BASIC), sectPtr{ i_sectPtr },  projType{ i_projType }
{
	borderThickness = 6.0f;
	if (projType == 0) {
		fillColor = VIVIDGAMBOGE;
		outlineColor = DARKRED;
	}
	else if (projType == 1) {
		fillColor = BASIL;
		outlineColor = EMERALD;
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
	if (projType == 1) {
		Explode();
	}
	Destroy();
}

void Projectile::CollideWithProjectile(Projectile * i_projPtr)
{
	if (i_projPtr->projType != projType) {
		Explode();
		Destroy();
	}
}

void Projectile::CollideWithBlocker(Blocker * i_blockPtr)
{
	if (projType == 1) {
		Explode();
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
				if (projType == 0) {
					Explode();
				}
				Destroy();
			}
		}
		else {
			if (projType == 0) {
				Explode();
			}
			Destroy();
		}
	}
	Explode();
	Destroy();
}

void Projectile::CollideWithWall(Wall * i_wallPtr)
{
	Destroy();
}

void Projectile::CollideWithPainWall(PainWall * i_painWallPtr)
{
	Explode();
	Destroy();
}

void Projectile::Explode()
{
	//lvlPtr->GenerateDeathEffects(this, CANNED_EXPLOSION);
	microSec ms(120000000);
	//microSec ms(1200000000);
		std::shared_ptr<Entity> anim = std::make_shared<Anim>(sectPtr, rb.transform.pos, ms, CANNED_EXPLOSION, this);
		sectPtr->AddEntPtrToSector(anim);
}

void Projectile::CollideWithDoor(Door * i_doorPtr)
{
	Explode();
	Destroy();
}

/////////////////////// Blocker ///////////////////////
Blocker::Blocker(Vector2f i_startPosition, RigidBody i_rb) :
	Entity(i_startPosition, i_rb, PROJ_WALL)
{
	fillColor = INDIGO;
	outlineColor = METALLICSILVER;
}

Blocker::~Blocker()
{
}

/////////////////////// Blast ///////////////////////
Blast::Blast(Sector* i_sectPtr, Vector2f i_startPosition, int i_blastType, float i_strength, float i_stunTime, RigidBody i_rb) :
	Entity(i_startPosition, i_rb, BLAST_STUN),
	sectPtr{ i_sectPtr }, blastType{ i_blastType },
	strength { i_strength }, stunTime { i_stunTime }
{
	charP = GLBVRS::PPTR;
	fillColor = sf::Color(0, 255, 255, 128);
	outlineColor = CYAN;
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
Enemy::Enemy(Sector* i_sectPtr, DIFFICULTY i_diff, Vector2f i_startPosition, RigidBody i_rb, TypeID i_typeID) :
	Entity(i_startPosition, i_rb, i_typeID),sectPtr{ i_sectPtr }
{
	charPtr = GLBVRS::PPTR;
	metal = false;
	origColorFill = MOSS;
	origColorOutLine = PEAR;
	fillColor = origColorFill;
	outlineColor = origColorOutLine;
	SetDiffVars(i_diff);
	maxHealth = 1;
	health = maxHealth;
	deathColorFill = UMBERBROWN;
	deathColorOutLine = PENNYBROWN;
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
	sectPtr->GenerateDeathEffects(this, ENEMY_BURST_DEATH);
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

void Enemy::DropPowerUp()
{
	std::shared_ptr<Entity> smallShipPOW3 = std::make_shared<PowerUp>(sectPtr,
		Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH * (2.0f / 5.0f)), GLBVRS::CRT_HGHT / 2.0)), TEMP_HEALTH);
	smallShipPOW3->rb.transform.pos = rb.transform.pos;
	sectPtr->AddEntPtrToSector(smallShipPOW3);
}

void Enemy::UpdateHealth(float i_stepSize)
{
	if (health <= 0.0f) {
		//fillColor = sf::Color::Red;
		//outlineColor = sf::Color::Red;
		Destroy();
	}
}

void Enemy::TakeDamage(float i_dmg)
{
		health -= i_dmg;
		ChangeColorHealth();
}

void Enemy::Stun(float i_stunTime)
{
	stunSecs = i_stunTime;
}

void Enemy::SetDiffVars(int i_diff)
{
	switch (i_diff) {
	case EASY: {
		speed = GLBVRS::ENEMYSPEEDEASY;
		Stun(0.7f);
		break;
	}
	case MEDIUM: {
		speed = GLBVRS::ENEMYSPEEDMED;
		Stun(0.45f);
		break;
	}
	case HARD: {
		Stun(0.3f);
		speed = GLBVRS::ENEMYSPEEDHARD;
		break;
	}
	}
}

void Enemy::ChangeColorHealth()
{
	float lifeRatio = std::max((health / maxHealth), 0.0f);
	float fillColorR = std::max(std::min((int)((lifeRatio * origColorFill.r) + ((1.0f - lifeRatio) * deathColorFill.r)), 255), 0);
	float fillColorG = std::max(std::min((int)((lifeRatio * origColorFill.g) + ((1.0f - lifeRatio) * deathColorFill.g)), 255), 0);
	float fillColorB = std::max(std::min((int)((lifeRatio * origColorFill.b) + ((1.0f - lifeRatio) * deathColorFill.b)), 255), 0);
	float outLineColorR = std::max(std::min((int)((lifeRatio * origColorOutLine.r) + ((1.0f - lifeRatio) * deathColorOutLine.r)), 255), 0);
	float outLineColorG = std::max(std::min((int)((lifeRatio * origColorOutLine.g) + ((1.0f - lifeRatio) * deathColorOutLine.g)), 255), 0);
	float outLineColorB = std::max(std::min((int)((lifeRatio * origColorOutLine.b) + ((1.0f - lifeRatio) * deathColorOutLine.b)), 255), 0);
	fillColor = sf::Color(fillColorR, fillColorG, fillColorB);
	outlineColor = sf::Color(outLineColorR, outLineColorG, outLineColorB);
}

void Enemy::TurnToMetal()
{
	metal = true;
	fillColor = UMBERBROWN;
	outlineColor = PENNYBROWN;
	rb.mat = DENSE_METAL;
	speed *= 3.0f;
	rb.SetMassData();
}

/////////////////////// CrazyBoi class ///////////////////////
CrazyBoi::CrazyBoi(Sector* i_sectPtr, DIFFICULTY i_diff, Vector2f i_startPosition, RigidBody i_rb) :
	Enemy(i_sectPtr, i_diff, i_startPosition, i_rb, ENEMY_RAND)
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
		speed = GLBVRS::ENEMYSPEEDEASY;
		break;
	}
	case MEDIUM: {
		speed = GLBVRS::ENEMYSPEEDMED;
		break;
	}
	case HARD: {
		speed = GLBVRS::ENEMYSPEEDHARD;
		break;
	}
	}
	speed *= 2;
}

/////////////////////// Burst Boss class ///////////////////////
BossBurst::BossBurst(Sector * i_sectPtr, DIFFICULTY i_diff, Vector2f i_startPosition, bool i_isMiniBoss, RigidBody i_rb) :
	Enemy(i_sectPtr, i_diff, i_startPosition, i_rb, ENEMY_BOSS), isMiniBoss{i_isMiniBoss}
{
	origColorFill = PARAKEET;
	origColorOutLine = EMERALD;
	fillColor = origColorFill;
	outlineColor = origColorOutLine;
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
	ChangeColorHealth();
}

void BossBurst::Destroy()
{
	sectPtr->PlaySound(RESOURCES::BOSSEXPLODE);
	sectPtr->GenerateDeathEffects(this, ENEMY_BURST_DEATH);
	killMeNextLoop = true;
	if (isMiniBoss) {
		DropPowerUp();
	}
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
			sectPtr,
			rb.transform.pos + (projDir * (100.0f + (i * 15.0f))), 1);
		projectile->rb.ApplyImpulse((projDir * projSpeed), NULL_VECTOR);
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
	if (isMiniBoss) {
		rb = RigidBody(std::make_shared<Circle>(60 * GLBVRS::SIZE_RAT), WOOD);
		switch (i_diff) {
		case EASY: {
			speed = GLBVRS::ENEMYSPEEDEASY;
			projSpeed = 2800.0f * std::pow(GLBVRS::SIZE_RAT, 2);
			numShots = 2;
			maxHealth = 2;
			health = maxHealth;
			break;
		}
		case MEDIUM: {
			speed = GLBVRS::ENEMYSPEEDMED;
			projSpeed = 3000.0f * std::pow(GLBVRS::SIZE_RAT, 2);
			numShots = 2;
			maxHealth = 3;
			health = maxHealth;
			break;
		}
		case HARD: {
			speed = GLBVRS::ENEMYSPEEDHARD;
			projSpeed = 3200.0f * std::pow(GLBVRS::SIZE_RAT, 2);
			numShots = 3;
			maxHealth = 4;
			health = maxHealth;
			break;
		}
		}
		speed *= 1.5;
	}
	else {
		switch (i_diff) {
		case EASY: {
			speed = GLBVRS::ENEMYSPEEDEASY;
			projSpeed = 2800.0f * std::pow(GLBVRS::SIZE_RAT, 2);
			numShots = 7;
			maxHealth = 8;
			health = maxHealth;
			break;
		}
		case MEDIUM: {
			speed = GLBVRS::ENEMYSPEEDMED;
			projSpeed = 3000.0f * std::pow(GLBVRS::SIZE_RAT, 2);
			numShots = 8;
			maxHealth = 9;
			health = maxHealth;
			break;
		}
		case HARD: {
			speed = GLBVRS::ENEMYSPEEDHARD;
			projSpeed = 3200.0f * std::pow(GLBVRS::SIZE_RAT, 2);
			numShots = 9;
			maxHealth = 10;
			health = maxHealth;
			break;
		}
		}
		speed *= 6;
	}
}

/////////////////////// Stream Boss class ///////////////////////
BossStream::BossStream(Sector * i_sectPtr, DIFFICULTY i_diff, Vector2f i_startPosition, bool i_isMiniBoss, RigidBody i_rb) :
	Enemy(i_sectPtr, i_diff, i_startPosition, i_rb, ENEMY_BOSS), isMiniBoss{ i_isMiniBoss }
{
	origColorFill = SAGE;
	origColorOutLine = EMERALD;
	fillColor = origColorFill;
	outlineColor = origColorOutLine;
	Stun(0.3f);
	sameDirTime = 0.2;
	timeTillDirSwitch = 0.0f;
	lastShotFired = hiResTime::now();
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
	ChangeColorHealth();
}

void BossStream::Destroy()
{
	sectPtr->PlaySound(RESOURCES::BOSSEXPLODE);
	sectPtr->GenerateDeathEffects(this, ENEMY_BURST_DEATH);
	killMeNextLoop = true;
	if (isMiniBoss) {
		DropPowerUp();
	}
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
				sectPtr,
				rb.transform.pos + (projDir * (100.0f)), 1);
			projectile->rb.ApplyImpulse((projDir * projSpeed), NULL_VECTOR);
			sectPtr->AddEntPtrToSector(projectile);
	}
}

void BossStream::Stun(float i_stunTime)
{
	stunSecs = i_stunTime / 2.0f;
}

void BossStream::SetDiffVars(int i_diff, int i_lvlNum)
{
	if (isMiniBoss) {
		rb = RigidBody(std::make_shared<Circle>(60 * GLBVRS::SIZE_RAT), WOOD);
		switch (i_diff) {
		case EASY: {
			speed = GLBVRS::ENEMYSPEEDEASY;
			projSpeed = 2800.0f * std::pow(GLBVRS::SIZE_RAT, 2);
			numShots = 6;
			maxHealth = 2;
			shipRateOfFire = 0.40f;
			health = maxHealth;
			break;
		}
		case MEDIUM: {
			speed = GLBVRS::ENEMYSPEEDMED;
			projSpeed = 3000.0f * std::pow(GLBVRS::SIZE_RAT, 2);
			numShots = 7;
			maxHealth = 3;
			shipRateOfFire = 0.25f;
			health = maxHealth;
			break;
		}
		case HARD: {
			speed = GLBVRS::ENEMYSPEEDHARD;
			projSpeed = 3200.0f * std::pow(GLBVRS::SIZE_RAT, 2);
			numShots = 8;
			maxHealth = 4;
			shipRateOfFire = 0.15f;
			health = maxHealth;
			break;
		}
		}
		speed *= 2;
	}
	else {
		switch (i_diff) {
		case EASY: {
			speed = GLBVRS::ENEMYSPEEDEASY;
			projSpeed = 2900.0f * std::pow(GLBVRS::SIZE_RAT, 2);
			numShots = 6;
			maxHealth = 10;
			shipRateOfFire = 0.10f;
			health = maxHealth;
			break;
		}
		case MEDIUM: {
			speed = GLBVRS::ENEMYSPEEDMED;
			projSpeed = 3100.0f * std::pow(GLBVRS::SIZE_RAT, 2);
			numShots = 7;
			maxHealth = 11;
			shipRateOfFire = 0.05f;
			health = maxHealth;
			break;
		}
		case HARD: {
			speed = GLBVRS::ENEMYSPEEDHARD;
			projSpeed = 3300.0f * std::pow(GLBVRS::SIZE_RAT, 2);
			numShots = 8;
			maxHealth = 12;
			shipRateOfFire = 0.04f;
			health = maxHealth;
			break;
		}
		}
		speed *= 7;
	}

}

/////////////////////// Rush Boss class ///////////////////////
BossRush::BossRush(Sector * i_sectPtr, DIFFICULTY i_diff, Vector2f i_startPosition, bool i_isMiniBoss, RigidBody i_rb) :
	Enemy(i_sectPtr, i_diff, i_startPosition, i_rb, ENEMY_BOSS), isMiniBoss{ i_isMiniBoss }
{
	origColorFill = MIDNIGHTMOSS;
	origColorOutLine = METALLICSILVER;
	fillColor = origColorFill;
	outlineColor = origColorOutLine;
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
	//sectPtr->GenerateDeathEffects(this, ENEMY_BURST_DEATH);
	killMeNextLoop = true;
	if (isMiniBoss) {
		DropPowerUp();
	}
}

void BossRush::TakeDamage(float i_dmg)
{
	sectPtr->PlaySound(RESOURCES::OUCH3);
	health -= i_dmg;
	ChangeColorHealth();
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
	if (isMiniBoss) {
		rb = RigidBody(std::make_shared<Circle>(60 * GLBVRS::SIZE_RAT), HEAVYBOUNCE);
		switch (i_diff) {
		case EASY: {
			speed = GLBVRS::ENEMYSPEEDEASY;
			numShots = 6;
			maxHealth = 2;
			health = maxHealth;
			break;
		}
		case MEDIUM: {
			speed = GLBVRS::ENEMYSPEEDMED;
			numShots = 7;
			maxHealth = 3;
			health = maxHealth;
			break;
		}
		case HARD: {
			speed = GLBVRS::ENEMYSPEEDHARD;
			numShots = 8;
			maxHealth = 4;
			health = maxHealth;
			break;
		}
		}
		speed *= 13;
	}
	else {
		switch (i_diff) {
		case EASY: {
			speed = GLBVRS::ENEMYSPEEDEASY;
			numShots = 6;
			maxHealth = 9;
			health = maxHealth;
			break;
		}
		case MEDIUM: {
			speed = GLBVRS::ENEMYSPEEDMED;
			numShots = 7;
			maxHealth = 10;
			health = maxHealth;
			break;
		}
		case HARD: {
			speed = GLBVRS::ENEMYSPEEDHARD;
			numShots = 8;
			maxHealth = 11;
			health = maxHealth;
			break;
		}
		}
		speed *= 28;
	}
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
BossSplit::BossSplit(Sector * i_sectPtr, DIFFICULTY i_diff, int i_splitsLeft, float i_spdFct, bool i_crazy, Vector2f i_startPosition, RigidBody i_rb) :
	Enemy(i_sectPtr, i_diff, i_startPosition, i_rb, ENEMY_BOSS), splitsLeft { i_splitsLeft }, diff { i_diff }, crazy {i_crazy}
{
	origColorFill = CHARTREUSE;
	origColorOutLine = MOSS;
	fillColor = origColorFill;
	outlineColor = origColorOutLine;
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
		std::shared_ptr<Entity> split1 = std::make_shared<BossSplit>(sectPtr, diff, splitsLeft - 1, 4, true, rb.transform.pos, projBody1);
		std::shared_ptr<Entity> split2 = std::make_shared<BossSplit>(sectPtr, diff, splitsLeft - 1, 4, false, rb.transform.pos, projBody2);
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
		std::shared_ptr<Entity> split1 = std::make_shared<BossSplit>(sectPtr, diff, splitsLeft - 1, 2.5f, true, rb.transform.pos, projBody1);
		std::shared_ptr<Entity> split2 = std::make_shared<BossSplit>(sectPtr, diff, splitsLeft - 1, 2.5f, false, rb.transform.pos, projBody2);
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
		std::shared_ptr<Entity> split1 = std::make_shared<BossSplit>(sectPtr, diff, splitsLeft - 1, 1.4f, true, rb.transform.pos, projBody1);
		std::shared_ptr<Entity> split2 = std::make_shared<BossSplit>(sectPtr, diff, splitsLeft - 1, 1.4f, false, rb.transform.pos, projBody2);
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
		std::shared_ptr<Entity> split1 = std::make_shared<BossSplit>(sectPtr, diff, splitsLeft - 1, true, 0.75f, rb.transform.pos, projBody1);
		std::shared_ptr<Entity> split2 = std::make_shared<BossSplit>(sectPtr, diff, splitsLeft - 1, false, 0.60f, rb.transform.pos, projBody2);
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
	sectPtr->GenerateDeathEffects(this, ENEMY_BURST_DEATH);
}

void BossSplit::TakeDamage(float i_dmg)
{
	health -= i_dmg;
	ChangeColorHealth();
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
	if (splitsLeft == 4) {
		switch (i_diff) {
		case EASY: {
			speed *= GLBVRS::ENEMYSPEEDEASY;
			numShots = 6;
			maxHealth = 3;
			health = maxHealth;
			break;
		}
		case MEDIUM: {
			speed *= GLBVRS::ENEMYSPEEDMED;
			numShots = 7;
			maxHealth = 4;
			health = maxHealth;
			break;
		}
		case HARD: {
			speed *= GLBVRS::ENEMYSPEEDHARD;
			numShots = 8;
			maxHealth = 5;
			health = maxHealth;
			break;

		}
		}
	}
	else if (splitsLeft == 3) {
		switch (i_diff) {
		case EASY: {
			speed *= GLBVRS::ENEMYSPEEDEASY;
			numShots = 6;
			maxHealth = 3;
			health = maxHealth;
			break;
		}
		case MEDIUM: {
			speed *= GLBVRS::ENEMYSPEEDMED;
			numShots = 7;
			maxHealth = 4;
			health = maxHealth;
			break;
		}
		case HARD: {
			speed *= GLBVRS::ENEMYSPEEDHARD;
			numShots = 8;
			maxHealth = 5;
			health = maxHealth;
			break;
		}
		}
	}
	if (splitsLeft == 2) {
		switch (i_diff) {
		case EASY: {
			speed *= GLBVRS::ENEMYSPEEDEASY;
			numShots = 6;
			maxHealth = 2;
			health = maxHealth;
			break;
		}
		case MEDIUM: {
			speed *= GLBVRS::ENEMYSPEEDMED;
			numShots = 7;
			maxHealth = 3;
			health = maxHealth;
			break;
		}
		case HARD: {
			speed *= GLBVRS::ENEMYSPEEDHARD;
			numShots = 8;
			maxHealth = 4;
			health = maxHealth;
			break;
		}

		}
	}
	if (splitsLeft == 1) {
		switch (i_diff) {
		case EASY: {
			speed *= GLBVRS::ENEMYSPEEDEASY;
			numShots = 6;
			maxHealth = 1;
			health = maxHealth;
			break;
		}
		case MEDIUM: {
			speed *= GLBVRS::ENEMYSPEEDMED;
			numShots = 7;
			maxHealth = 1;
			health = maxHealth;
			break;
		}
		case HARD: {
			speed *= GLBVRS::ENEMYSPEEDHARD;
			numShots = 8;
			maxHealth = 1;
			health = maxHealth;
			break;
		}
		}
	}
	if (splitsLeft == 0) {
		switch (i_diff) {
		case EASY: {
			speed *= GLBVRS::ENEMYSPEEDEASY;
			numShots = 6;
			maxHealth = 1;
			health = maxHealth;
			break;
		}
		case MEDIUM: {
			speed *= GLBVRS::ENEMYSPEEDMED;
			numShots = 7;
			maxHealth = 1;
			health = maxHealth;
			break;
		}
		case HARD: {
			speed *= GLBVRS::ENEMYSPEEDHARD;
			numShots = 8;
			maxHealth = 1;
			health = maxHealth;
			break;
		}
		}
	}
}

/////////////////////// Spawn Boss class ///////////////////////
BossSpawn::BossSpawn(Sector * i_sectPtr, DIFFICULTY i_diff, Vector2f i_startPosition, bool i_isMiniBoss, RigidBody i_rb) :
	Enemy(i_sectPtr, i_diff, i_startPosition, i_rb, ENEMY_BOSS), diff { i_diff }, isMiniBoss{ i_isMiniBoss }
{
	origColorFill = EMERALD;
	origColorOutLine = SEAFOAM;
	fillColor = origColorFill;
	outlineColor = origColorOutLine;
	if (isMiniBoss) {
		rb = RigidBody(std::make_shared<Rectangle>(100.0f * GLBVRS::SIZE_RAT, 100.0f * GLBVRS::SIZE_RAT), STATIC);
		rb.transform.pos = i_startPosition;
	}
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
		//fillColor = EMERALD;
		//outlineColor = PISTACHIO;
		ChangeColorHealth();
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
	sectPtr->GenerateDeathEffects(this, ENEMY_BURST_DEATH);
	killMeNextLoop = true;
	if (isMiniBoss) {
		DropPowerUp();
	}
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
	if (isMiniBoss) {
		switch (i_diff) {
		case EASY: {
			numShots = 3;
			maxHealth = 4;
			sameDirTime = 16.0;
			timeTillDirSwitch = 0.0f;
			health = maxHealth;
			break;
		}
		case MEDIUM: {
			numShots = 4;
			maxHealth = 5;
			sameDirTime = 12.0;
			timeTillDirSwitch = 0.0f;
			health = maxHealth;
			break;
		}
		case HARD: {
			numShots = 5;
			maxHealth = 6;
			sameDirTime = 10.0;
			timeTillDirSwitch = 0.0f;
			health = maxHealth;
			break;
		}
		}
	}
	else {
		switch (i_diff) {
		case EASY: {
			numShots = 4;
			maxHealth = 28;
			sameDirTime = 14.0;
			timeTillDirSwitch = 0.0f;
			health = maxHealth;
			break;
		}
		case MEDIUM: {
			numShots = 5;
			maxHealth = 22;
			sameDirTime = 10.0;
			timeTillDirSwitch = 0.0f;
			health = maxHealth;
			break;
		}
		case HARD: {
			numShots = 6;
			maxHealth = 26;
			sameDirTime = 8.0;
			timeTillDirSwitch = 0.0f;
			health = maxHealth;
			break;
		}
		}
	}
}

/////////////////////// Door Lockable class///////////////////////
Door::Door(Sector* i_sectPtr, MapCoord i_outCoord, Vector2f i_startPos, Vector2f i_outPos, RigidBody i_rb, SCREEN_SIDE i_side) :
	Entity(i_startPos, i_rb, DOOR), sectPtr{ i_sectPtr }, outCoord{ i_outCoord }, outPos{ i_outPos }, side { i_side }
{
	open = false;
	intangible = true;

	colorOpen = CYAN;
	colorClosed = DARKSILVER;
	colorOpenOutline = CYAN;
	colorClosedOutline = SILVER;
	fillColor = colorClosed;
	outlineColor = colorClosedOutline;
	disableTimeStamp = hiResTime::now();
	disabledAmount = 0.0f;
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
	float timeSinceDisable = (std::chrono::duration_cast<std::chrono::microseconds>(hiResTime::now() - disableTimeStamp)).count() / 1000000.0f;
	if (sectPtr->sectEnemyNum > 0) {
		open = false;
		fillColor = colorClosed;
		outlineColor = colorClosedOutline;
	}
	else if (timeSinceDisable <= disabledAmount) {
		open = false;
		fillColor = colorClosed;
		outlineColor = colorClosedOutline;
	}
	else {
		open = true;
		fillColor = colorOpen;
		outlineColor = colorOpenOutline;
	}
}

void Door::Disable(float i_disableTime)
{
	disableTimeStamp = hiResTime::now();
	disabledAmount = i_disableTime;
}


///////////////////////Wall Class ///////////////////////
Wall::Wall(Vector2f i_startPosition, Sector* i_sectPtr, RigidBody i_rb,
			sf::Color i_fillCol, sf::Color i_outCol, bool i_break) :
	Entity(i_startPosition, i_rb, WALL_BASIC), sectPtr{ i_sectPtr }, breakable {i_break}
{
	health = 20;
	fixedPosition = true;
	fillColor = i_fillCol;
	outlineColor = i_outCol;
}

void Wall::CollideWithProjectile(Projectile * i_projPtr)
{
	if (breakable) {
		TakeDamage(1.0f);
	}
}

void Wall::TakeDamage(float i_dmg)
{
	health -= i_dmg;
	if (health <= 0.0f) {
		rb.mat = DENSE_METAL;
		rb.SetMassData();
	}
}


Wall::~Wall()
{
}


/////////////////////// Pain Wall Class ///////////////////////
PainWall::PainWall(Vector2f i_startPosition, Sector* i_sectPtr, RigidBody i_rb) :
	Entity(i_startPosition, i_rb, WALL_FIRE), sectPtr{ i_sectPtr }
{
	intangible = true;
	fixedPosition = true;
	colorSwitchRate = 3;
	lastColorSwitch = hiResTime::now();
	colorState = true;
	fillColor = VIVIDYELLOW;
	outlineColor = sf::Color::Black;
	colorA = MYSTICRED;
	colorB = VIVIDYELLOW;
}

PainWall::~PainWall()
{
}


void PainWall::Update(float i_stepSize)
{
	float colorSwitchCurr = (std::chrono::duration_cast<std::chrono::microseconds>(hiResTime::now() - lastColorSwitch)).count() / 1000000.0f;
	if (colorSwitchCurr >= colorSwitchRate) {
		lastColorSwitch = hiResTime::now();
	}
	float colorpercent = (colorSwitchCurr / colorSwitchRate) * 2.0f * PI;
	float clrPhse = cos(colorpercent);
	float clrPhseInv = 1.0f - clrPhse;
	float r = std::min(255.0f, std::max(0.0f, (colorA.r * clrPhse) + (colorB.r * clrPhseInv)));
	float g = std::min(255.0f, std::max(0.0f, (colorA.g * clrPhse) + (colorB.g * clrPhseInv)));
	float b = std::min(255.0f, std::max(0.0f, (colorA.b * clrPhse) + (colorB.b * clrPhseInv)));
	fillColor = sf::Color(r,g,b);
	float secsInUpdate = i_stepSize / 1000.0f;
}

/////////////////////// End Object ///////////////////////
EndObject::EndObject(Sector* i_sectPtr, Vector2f i_startPosition, RigidBody i_rb) :
	Entity(i_startPosition, i_rb, END_LEVEL), sectPtr{ i_sectPtr }
{
	fillColor = DARKSILVER;
	outlineColor = SILVER;
	active = false;
}

EndObject::~EndObject()
{
}

void EndObject::Update(float i_stepSize)
{
	if (sectPtr->firstPhase) {
		fillColor = DARKSILVER;
		outlineColor = SILVER;
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
		outlineColor = METALLICSILVER;
		break;
	}
	case (SCATTER): {
		fillColor = sf::Color(51, 153, 102);
		outlineColor = METALLICSILVER;
		break;
	}
	case (TEMP_HEALTH): {
		fillColor = RED;
		outlineColor = METALLICSILVER;
		break;
	}
	//TODO
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

/////////////////////////////animations ///////////////////////////////////////////////////////////

Anim::Anim(Sector* i_sectPtr, Vector2f i_startPosition, microSec i_lifetime,
	ANIMTYPE i_aType, Entity* i_entPtr) :
	Entity(i_startPosition, RigidBody(std::make_shared<Circle>(1.0f)), ANIMATION),
	birthTime{ hiResTime::now() }, deathTime{ birthTime + i_lifetime }, lifeTime { i_lifetime },
	aType { i_aType }
{
	physicalObject = false;
	intangible = true;
	hasVisuals = true;
	drawables = std::make_shared<std::vector<std::shared_ptr<sf::Drawable>>>();
	rb.vel *= 0.1;

	rb = i_entPtr->rb;
	fillColor = i_entPtr->fillColor;
	outlineColor = i_entPtr->outlineColor;

	if (aType == CANNED_EXPLOSION) {
		rb.angVel = 0;
		rb.vel = Vector2f(0.0f, 0.0f);
		origPosition = sf::Vector2f(rb.transform.pos[0], rb.transform.pos[1]);
		auto regPoly1 = Physics::CreateRegularPolygon(5, 30.0f);
		auto regPoly2 = Physics::CreateRegularPolygon(5, 15.0f);
		polys.push_back(regPoly1);
		polys.push_back(regPoly2);
	}/*
	else if (i_entPtr != NULL) {
		fillColor = i_entPtr->fillColor;
		outlineColor = i_entPtr->outlineColor;
		rb = i_entPtr->rb;
		}*/
	if (rb.shape->GetType() == Shape::ShapeType::CIRCLE) {
		auto circle = dynamic_cast<Circle*>(rb.shape.get());
		origRadius = circle->radius;
	}
	if (rb.shape->GetType() == Shape::ShapeType::RECTANGLE) {
		auto rectangle = dynamic_cast<Rectangle*>(rb.shape.get());
		origWidth = rectangle->GetWidth();
		origHeight = rectangle->GetHeight();
	}
}

Anim::~Anim()
{
}

void Anim::Update(float i_stepSize)
{
	//Im gonna make the polygon shrink to !almost! nothing before it dissapears
	float microSecsAlive = (hiResTime::now() - birthTime).count();
	float lifeTimeFloat = lifeTime.count();
	float lifePercent = microSecsAlive / lifeTimeFloat;
	float lowBound = std::max(lifePercent, 0.1f);
	float upBound = std::min(1.0f, lowBound);
	float animPercent = upBound;
	if (animPercent > 0.99f) {
		Destroy();
	}
	else if (aType == CANNED_EXPLOSION) {
		for (int i = 0; i < polys.size(); i++) {
			// std::cout << "I am resizing and Im at " + std::to_string(animPercent) + "percent\n";
			polys[i]->ResetSize(animPercent);

		}
	}
	else if (aType == ENEMY_BURST_DEATH) {
				if ((rb.shape->GetType() == Shape::ShapeType::CIRCLE)) {
					auto circle = dynamic_cast<Circle*>(rb.shape.get());
					float radius = circle->radius;
					float sizeIncrease = animPercent * (origRadius * 0.5f);
					circle->ChangeSizeOfShape(origRadius + sizeIncrease, 0.0f);
				}
				else if (rb.shape->GetType() == Shape::ShapeType::RECTANGLE) {
					auto rect = dynamic_cast<Rectangle*>(rb.shape.get());
					float sizeIncreaseWidth = animPercent * (origWidth * 0.5f);
					float sizeIncreaseHeight = animPercent * (origHeight * 0.5f);
					rect->ChangeSizeOfShape(origWidth + sizeIncreaseWidth, origHeight + sizeIncreaseHeight);
				}
			}
}

void Anim::Destroy()
{
	killMeNextLoop = true;
}

drawablePtrVect Anim::CreateDrawables(float i_lerp_fraction)
{
	float microSecsAlive = (hiResTime::now() - birthTime).count();
	float lifeTimeFloat = lifeTime.count();
	float lifePercent = microSecsAlive / lifeTimeFloat;
	float lowBound = std::max(lifePercent, 0.1f);
	float upBound = std::min(1.0f, lowBound);
	float animPercent = upBound;
	drawables->clear();
	if (aType != CANNED_EXPLOSION) {
		std::shared_ptr<sf::Shape> drawShape = CreateDrawableRB(i_lerp_fraction);
		drawables->emplace_back(drawShape);
		return drawables;
	}
	for (int i = 0; i < polys.size(); i++) {
		std::shared_ptr<sf::Shape> polyDrawable = polys[i]->GetSFMLRepr();
		if ((i % 2) == 0) {
			polyDrawable->setFillColor(fillColor);
			polyDrawable->setOutlineColor(outlineColor);
		}
		else {
			polyDrawable->setFillColor(outlineColor);
			polyDrawable->setOutlineColor(fillColor);
		}
		polyDrawable->setOutlineThickness(6.0f);
		polyDrawable->setPosition(origPosition);
		polyDrawable->setPosition(origPosition);
		float rotationDir = i == 0 ? 1 : (i * -1);
		float rotationOffset = i * 36;
		polyDrawable->setRotation((animPercent * 270 * rotationDir) + rotationOffset);

		drawables->push_back(polyDrawable);
	}
	return drawables;
}


/////////////////////// Scenery ///////////////////////
Scenery::Scenery(Sector * i_sectPtr, Vector2f i_startPosition, std::shared_ptr <sf::Shape> i_drawable,
	RigidBody i_rb) : Entity(i_startPosition, i_rb, SCENERY), mySect {i_sectPtr}
{
	physicalObject = false;
	intangible = true;
	hasVisuals = true;
	drawables = std::make_shared<std::vector<std::shared_ptr<sf::Drawable>>>();
	drawables->push_back(i_drawable);
}

Scenery::~Scenery()
{
}

drawablePtrVect Scenery::CreateDrawables(float i_lerp_fraction)
{
	return drawables;
}