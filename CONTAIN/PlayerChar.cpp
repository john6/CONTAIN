#include "Game.h"
#include "PlayerChar.h"
#include "Blast.h"
#include "Door.h"
#include "EndObject.h"
#include "PowerUp.h"
#include "Projectile.h"

PlayerChar::PlayerChar(int i_strtHealth, Vector2f i_startPosition, RigidBody i_rb) :
	Entity(i_startPosition, i_rb, PLAYER), pController{ PlayerController(GLBVRS::GPTR->renderWindow) }
{
	numShots = 1;
	fillColor = GUNMETAL;
	outlineColor = FORDSILVER;
	shipRateOfFire = 0.9f;
	shipRateOfFireWall = 1.3;
	wallDelay = shipRateOfFireWall;
	weaponDelay = shipRateOfFire;
	shipRateOfAOE = 6.0f;
	shipSpeed = 100;
	size_t minutes = 255;
	lastShotFired = std::chrono::high_resolution_clock::now() - std::chrono::minutes(1);
	lastWallFired = std::chrono::high_resolution_clock::now() - std::chrono::minutes(1);
	lastAOEFired = std::chrono::high_resolution_clock::now() - std::chrono::minutes(1);
	lastDamageReceived = std::chrono::high_resolution_clock::now() - std::chrono::minutes(1);
	dmgRate = 1.0f;
	maxHealth = i_strtHealth;
	health = maxHealth;
	currSpecialAmmo = 3;

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
		GLBVRS::RSRCS->PlaySound(3);
		lastDamageReceived = hiResTime::now();
		health -= i_dmg;
	}
}

void PlayerChar::ReceivePowerUp(UPGRADE_TYPE i_powType)
{
	GLBVRS::RSRCS->PlaySound(RESOURCES::MENUACCEPT6);
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
	GLBVRS::RSRCS->PlaySound(RESOURCES::FIREBALL);
	TakeDamage(1.0f);
}

void PlayerChar::CollideWithDoor(Door * i_doorPtr)
{
	if (i_doorPtr->IsOpen()) {
		GLBVRS::RSRCS->PlaySound(RESOURCES::MAGIC7);
		rb.ignoreForcesThisStep = true;
		rb.ResetPosition(i_doorPtr->GetOutPos());
		GLBVRS::GPTR->RequestTravelToSector(i_doorPtr->GetOutCoord());
	}
}

void PlayerChar::CollideWithEndObject(EndObject * i_endPtr)
{
	if (i_endPtr->active) {
		GLBVRS::GPTR->RequestGoToNextLvl();
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
		GLBVRS::RSRCS->PlaySound(RESOURCES::SHOT2);
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
			std::shared_ptr<Entity> projectile = std::make_shared<Projectile>(
					rb.transform.pos + (currDirVect * (projCenterOffset)));
			projectile->rb.ApplyImpulse((currDirVect * weapSpeed), NULL_VECTOR);
			spawnVect.push_back(projectile);
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
		GLBVRS::RSRCS->PlaySound(RESOURCES::SHOOT14);
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
			projectile->rb.ApplyImpulse((projectileDirFront * 7000.0f), NULL_VECTOR);
			projectile->rb.ResetOrientation(projectileDirFront);
			spawnVect.push_back(projectile);
		}

		else if (shipLvl[WALL_BIG] == 1) {
			std::shared_ptr<Entity> projectile1 = std::make_shared<Blocker>(
				rb.transform.pos + (projectileDirFront * projCenterOffset),
				RigidBody(std::make_shared<Rectangle>(wallWidth, wallHeight + (wallWidth * 2.0f)), HEAVYBALL));
			projectile1->rb.ApplyImpulse((projectileDirFront * 7000.0f), NULL_VECTOR);
			projectile1->rb.ResetOrientation(projectileDirFront);
			spawnVect.push_back(projectile1);

			std::shared_ptr<Entity> projectile2 = std::make_shared<Blocker>(
				rb.transform.pos + (projectileDirBack * projCenterOffset),
				RigidBody(std::make_shared<Rectangle>(wallWidth, wallHeight), HEAVYBALL));
			projectile2->rb.ApplyImpulse((projectileDirBack * 7000.0f), NULL_VECTOR);
			projectile2->rb.ResetOrientation(projectileDirBack);
			spawnVect.push_back(projectile2);
		}
		else if (shipLvl[WALL_BIG] == 2) {
			std::shared_ptr<Entity> projectile1 = std::make_shared<Blocker>(
				rb.transform.pos + (projectileDirFront * projCenterOffset),
				RigidBody(std::make_shared<Rectangle>(wallWidth, wallHeight + (wallWidth * 2.0f)), HEAVYBALL));
			projectile1->rb.ApplyImpulse((projectileDirFront * 7000.0f), NULL_VECTOR);
			projectile1->rb.ResetOrientation(projectileDirFront);
			spawnVect.push_back(projectile1);

			std::shared_ptr<Entity> projectile2 = std::make_shared<Blocker>(
				rb.transform.pos + (projectileDirBack * projCenterOffset),
				RigidBody(std::make_shared<Rectangle>(wallWidth, wallHeight + (wallWidth * 2.0f)), HEAVYBALL));
			projectile2->rb.ApplyImpulse((projectileDirBack * 7000.0f), NULL_VECTOR);
			projectile2->rb.ResetOrientation(projectileDirBack);
			spawnVect.push_back(projectile2);

			std::shared_ptr<Entity> projectile3 = std::make_shared<Blocker>(
				rb.transform.pos + (projectileDirLeft * projCenterOffset),
				RigidBody(std::make_shared<Rectangle>(wallWidth, wallHeight - (wallWidth * 2.0f)), HEAVYBALL));
			projectile3->rb.ApplyImpulse((projectileDirLeft * 7000.0f), NULL_VECTOR);
			projectile3->rb.ResetOrientation(projectileDirLeft);
			spawnVect.push_back(projectile3);

			std::shared_ptr<Entity> projectile4 = std::make_shared<Blocker>(
				rb.transform.pos + (projectileDirRight * projCenterOffset),
				RigidBody(std::make_shared<Rectangle>(wallWidth, wallHeight - (wallWidth * 2.0f)), HEAVYBALL));
			projectile4->rb.ApplyImpulse((projectileDirRight * 7000.0f), NULL_VECTOR);
			projectile4->rb.ResetOrientation(projectileDirRight);
			spawnVect.push_back(projectile4);
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
{
	//TODO make some time cast function so I dont have to redo it everywhere
	weaponDelay = (std::chrono::duration_cast<std::chrono::microseconds>(hiResTime::now() - lastAOEFired)).count() / 1000000.0f;
	if (weaponDelay >= shipRateOfAOE) {
		GLBVRS::RSRCS->PlaySound(RESOURCES::MAGIC10);
		--currSpecialAmmo;
		lastAOEFired = hiResTime::now();
		std::shared_ptr<Entity> projectile = std::make_shared<Blast>(
					rb.transform.pos, 0, blastStrength, blastStunTime, RigidBody(std::make_shared<Circle>(BlastRadius), STATIC));
		spawnVect.push_back(projectile);
	}
}