#include "Game.h"
#include "PlayerChar.h"
#include "Blast.h"
#include "Door.h"
#include "EndObject.h"
#include "PowerUp.h"
#include "Projectile.h"
#include <math.h>

PlayerChar::PlayerChar(int i_strtHealth, Vector2f i_startPosition, RigidBody i_rb) :
	Entity(i_startPosition, i_rb, PLAYER), pController{ PlayerController(GLBVRS::GPTR->renderWindow) }
{
	numShots = 1;
	origColorFill = FORDSILVER;
	origColorOutLine = FORDSILVER;
	origColorInnerHex = METALLICSILVER;
	origColorInnerHexOutline = METALLICSILVER;
	origColorShaft = DARKSILVER;
	currColorInnerHex = origColorInnerHex;
	currColorInnerHexOutline = origColorInnerHexOutline;
	currColorShaft = origColorShaft;


	deathColorFill = FORDDARKCOPPER;
	deathColorOutLine = FORDDARKCOPPER;


	shipRateOfFire = 0.9f;
	shipRateOfFireWall = 1.3;
	wallDelay = shipRateOfFireWall;
	weaponDelay = shipRateOfFire;
	shipRateOfAOE = 6.0f;
	//Ship speed was 100 before I normalized input direction, it was basicallyu doubled whenever the player was moving diagonally, will compensate now
	shipSpeed = 150;
	size_t minutes = 255;
	lastShotFired = std::chrono::high_resolution_clock::now() - std::chrono::minutes(1);
	lastWallFired = std::chrono::high_resolution_clock::now() - std::chrono::minutes(1);
	lastAOEFired = std::chrono::high_resolution_clock::now() - std::chrono::minutes(1);
	lastDamageReceived = std::chrono::high_resolution_clock::now() - std::chrono::minutes(1);
	dmgRate = 1.0f;
	maxHealth = i_strtHealth;
	health = maxHealth;
	currSpecialAmmo = 3;
	hasExploded = false;
	alreadyDead = false;

	wallWidth = 40;
	wallHeight = 270;
	BlastRadius = 230;
	//used to be 600
	blastStrength = 30000.0f;
	blastStunTime = 4.0f;
	maxSpecialAmmo = 4;
	currSpecialAmmo = maxSpecialAmmo;
	weapSpeed = 3000.0f;
	InitLvl();
	rectPtr = dynamic_cast<Rectangle*>(rb.shape.get());
	hasVisuals = true;


	//if (TESTING) {
	//	ReceivePowerUp(RATE_OF_FIRE);
	//	ReceivePowerUp(RATE_OF_FIRE);
	//	ReceivePowerUp(RATE_OF_FIRE);
	//	ReceivePowerUp(SCATTER);
	//	ReceivePowerUp(SCATTER);
	//	ReceivePowerUp(SCATTER);
	//	ReceivePowerUp(BIG_SHIP);
	//	ReceivePowerUp(BIG_SHIP);
	//	ReceivePowerUp(BIG_SHIP);
	//}



	sf::Texture texture;
	if (!texture.loadFromFile("Textures/tile115.png"))
	{
		std::cerr << "failed to load Textures/tile113.png";
	}
	else {
		texturePtr = std::make_shared<sf::Texture>(texture);
		textStretchHeight = 1;
		textStretchWidth = 1;
	}
}

PlayerChar::~PlayerChar()
{
}

void PlayerChar::Update(float i_stepSize)
{
	UpdateHealth(i_stepSize);
	if ((GLBVRS::canPressButtonsAgain) && (health > 0.0f)) {
		UpdateMovement(i_stepSize);
		AcceptWeaponInput(i_stepSize);
	}
	else {
		currThrustDirVect = Vector2f(0.0f, 0.0f);
		GLBVRS::RSRCS->playEngineSound(false);
	}
}

void PlayerChar::UpdateMovement(float i_stepSize)
{
	currThrustDirVect = Vector2f(0.0f, 0.0f);
	float moveDist = shipSpeed * i_stepSize;
	currThrustDirVect = pController.GetMovementDir();
	rb.ApplyImpulse(currThrustDirVect * moveDist, NULL_VECTOR);

	if ((std::abs(currThrustDirVect[0]) > 0.005) || (std::abs(currThrustDirVect[1]) > 0.005)) {
		GLBVRS::RSRCS->playEngineSound(true);
	}
	else {
		GLBVRS::RSRCS->playEngineSound(false);
	}
	currThrustDirVect.normalize();
	if ((currThrustDirVect[0] != lastThrustDirVect[0]) || (currThrustDirVect[1] != lastThrustDirVect[1])) {
		lastThrustDirChange = hiResTime::now();
		timeSinceDirChange = 0.0f;
	}
	else {
		float secondsSinceThrustChange = (std::chrono::duration_cast<std::chrono::microseconds>(hiResTime::now() - lastThrustDirChange)).count() / 1000000.0f;
		timeSinceDirChange = secondsSinceThrustChange;
	}
	lastThrustDirVect = currThrustDirVect;
}

void PlayerChar::UpdateHealth(float i_stepSize)
{
	auto timeSinceDied = std::chrono::duration_cast<std::chrono::milliseconds>(hiResTime::now() - timeWhenDied);
	float splosionTimerMilli = 2400.0f;
	if ((health <= 0.0f) && (timeSinceDied.count() < splosionTimerMilli)) {
		fillColor = PENNYBROWN;
		outlineColor = FIREBRICK;
		microSec microSec(250000000);
		std::vector<Vector2f> shapePoints = rb.GetVertCords();
		float minX = INT_MAX;
		float minY = INT_MAX;
		float maxX = INT_MIN;
		float maxY = INT_MIN;
		for (int i = 0; i < shapePoints.size(); i++) {
			if (shapePoints[i][0] < minX) { minX = shapePoints[i][0]; }
			if (shapePoints[i][0] > maxX) { maxX = shapePoints[i][0]; }

			if (shapePoints[i][1] < minY) { minY = shapePoints[i][1]; }
			if (shapePoints[i][1] > maxY) { maxY = shapePoints[i][1]; }
		}

		Vector2f topLeft(minX, minY);
		Vector2f bottomRight(maxX, maxY);

		for (int i = 0; i < 2; i++) {
			Vector2f spawnPos = Math::GetRandomCoordInRect(topLeft, bottomRight);
			std::shared_ptr<Entity> anim = std::make_shared<Anim>(spawnPos, microSec);
			spawnVect.push_back(anim);
		}
	}
	else if ((health <= 0.0f) && (timeSinceDied.count() >= 2.0) && (!hasExploded)) {
		hasExploded = true;
		physicalObject = false;
		hasVisuals = false;

		GLBVRS::RSRCS->PlayExplosionSound(false);
		GLBVRS::RSRCS->PlaySound(RESOURCES::EXPLODE7);

		for (int i = 0; i < 8; i++) {
			std::shared_ptr<Shape> shape1 = Physics::CreateIrregularPolygon(4, 150, 300);
			RigidBody fragment1 = RigidBody(shape1, STATIC);
			std::shared_ptr<Wall> wall1 = std::make_shared<Wall>(
				rb.transform.pos, fragment1, PENNYBROWN, FIREBRICK, true);
			wall1->TakeDamage(999);
			std::shared_ptr<Entity> frag1 = move(wall1);
			spawnVect.push_back(frag1);
		}

		std::shared_ptr<Entity> projectile = std::make_shared<Blast>(rb.transform.pos, 0, 6000, blastStunTime, 300);
		projectile->fillColor = sf::Color(255, 236, 25, 188);
		projectile->outlineColor = sf::Color(139, 0, 0, 188);
		spawnVect.push_back(projectile);
	}
}

void PlayerChar::ResetHealth()
{
	health = maxHealth;
	fillColor = origColorFill;
	//fillColor = sf::Color(142, 152, 157);
	outlineColor = origColorOutLine;
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
	if (!pController.IsControllerConnected()) {
		Vector2f leftMousePos = pController.LeftClick();
		if (!leftMousePos.isZero()) {
			Vector2f projDir = leftMousePos - rb.transform.pos;
			projDir.normalize();
			ShootBasic(projDir);
		}
		Vector2f rightMousePos = pController.RightClick();
		if (!rightMousePos.isZero()) {
			Vector2f projDir = rightMousePos - rb.transform.pos;
			projDir.normalize();
			ShootWall(projDir);
		}
		bool scrollClick = pController.ScrollClick();
		if (scrollClick) {
			ShootAOE();
		}
	}
	else {
		if (pController.IsRightTriggerPressed()) {
			ShootBasic(pController.GetRightStickVect());
		}
		if (pController.IsLeftTriggerPressed()) {
			ShootWall(pController.GetRightStickVect());
		}
		bool scrollClick = pController.IsRightStickPressed();
		if (scrollClick) {
			ShootAOE();
		}
	}
}

void PlayerChar::TakeDamage(float i_dmg, CollisionData i_coll) 
{
	auto timeSinceDamaged = std::chrono::duration_cast<std::chrono::seconds>(hiResTime::now() - lastDamageReceived);
	if ((!alreadyDead) && (timeSinceDamaged.count() >= dmgRate)) {
		GLBVRS::RSRCS->PlaySound(RESOURCES::SOUNDS::EXPLOSION);
		lastDamageReceived = hiResTime::now();
		health -= i_dmg;
		GenerateDamageEffects(i_coll);
		if (health <= 0.0f) {
			alreadyDead = true;
			timeWhenDied = hiResTime::now();
			GLBVRS::RSRCS->PlayExplosionSound(true);
		}
	}
}

void PlayerChar::TakeDamage(float i_dmg)
{
	auto timeSinceDamaged = std::chrono::duration_cast<std::chrono::seconds>(hiResTime::now() - lastDamageReceived);
	if ((!alreadyDead) && (timeSinceDamaged.count() >= dmgRate)) {
		GLBVRS::RSRCS->PlaySound(RESOURCES::SOUNDS::EXPLOSION);
		lastDamageReceived = hiResTime::now();
		health -= i_dmg;
		if (health <= 0.0f) {
			alreadyDead = true;
			timeWhenDied = hiResTime::now();
			GLBVRS::RSRCS->PlayExplosionSound(true);
		}
	}
}

void PlayerChar::GenerateDamageEffects(CollisionData i_collisionCopy)
{
	microSec ms(200000000);
	Vector2f collisionDir = i_collisionCopy.entA->rb.transform.pos - i_collisionCopy.entB->rb.transform.pos;
	std::shared_ptr<Entity> anim = std::make_shared<Anim>(i_collisionCopy.norm, i_collisionCopy.contactPoints, ms, 0, 0);
	spawnVect.push_back(anim);
	ChangeColorHealth();
}

void PlayerChar::ChangeColorHealth()
{
	float lifeRatio = std::max(((float)health / (float)maxHealth), 0.0f);
	float fillColorR = std::max(std::min((int)((lifeRatio * origColorFill.r) + ((1.0f - lifeRatio) * deathColorFill.r)), 255), 0);
	float fillColorG = std::max(std::min((int)((lifeRatio * origColorFill.g) + ((1.0f - lifeRatio) * deathColorFill.g)), 255), 0);
	float fillColorB = std::max(std::min((int)((lifeRatio * origColorFill.b) + ((1.0f - lifeRatio) * deathColorFill.b)), 255), 0);
	float outLineColorR = std::max(std::min((int)((lifeRatio * origColorOutLine.r) + ((1.0f - lifeRatio) * deathColorOutLine.r)), 255), 0);
	float outLineColorG = std::max(std::min((int)((lifeRatio * origColorOutLine.g) + ((1.0f - lifeRatio) * deathColorOutLine.g)), 255), 0);
	float outLineColorB = std::max(std::min((int)((lifeRatio * origColorOutLine.b) + ((1.0f - lifeRatio) * deathColorOutLine.b)), 255), 0);
	fillColor = sf::Color(fillColorR, fillColorG, fillColorB);
	outlineColor = sf::Color(outLineColorR, outLineColorG, outLineColorB);

	float innerHexColorR = std::max(std::min((int)((lifeRatio * origColorInnerHex.r) + ((1.0f - lifeRatio) * deathColorFill.r)), 255), 0);
	float innerHexColorG = std::max(std::min((int)((lifeRatio * origColorInnerHex.g) + ((1.0f - lifeRatio) * deathColorFill.g)), 255), 0);
	float innerHexColorB = std::max(std::min((int)((lifeRatio * origColorInnerHex.b) + ((1.0f - lifeRatio) * deathColorFill.b)), 255), 0);
	currColorInnerHex= sf::Color(innerHexColorR, innerHexColorG, innerHexColorB);

	float shaftColorR = std::max(std::min((int)((lifeRatio * origColorShaft.r) + ((1.0f - lifeRatio) * deathColorFill.r)), 255), 0);
	float shaftColorG = std::max(std::min((int)((lifeRatio * origColorShaft.g) + ((1.0f - lifeRatio) * deathColorFill.g)), 255), 0);
	float shaftColorB = std::max(std::min((int)((lifeRatio * origColorShaft.b) + ((1.0f - lifeRatio) * deathColorFill.b)), 255), 0);
	currColorShaft = sf::Color(shaftColorR, shaftColorG, shaftColorB);
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
			//shipLvl[BIG_SHIP] = 3;
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
			//shipLvl[SMALL_SHIP] = 3;
			shipLvl[BIG_SHIP]++;
			//rb.ResetOrientation(0.0f);
			//rb.vel = Vector2f(0.0f, 0.0f);
			//rb.force = Vector2f(0.0f, 0.0f);
			//rb.angVel = 0.0f;
			auto playerPoly = dynamic_cast<Polygon*>(rb.shape.get());
			playerPoly->ResizeMutliple(1.10f);
			rb.objVerts = playerPoly->GetPoints();
			rb.SetMassData();
			maxHealth += 7;
			AddHealth(6);
			shipSpeed *= 1.3;
		}
		break;
	}
	case (BLAST): { //number of shots
		if (shipLvl.at(BLAST) < GLBVRS::GetUpgradeMax(BLAST)) {
			shipLvl[BLAST]++;
			BlastRadius *= 1.2f;
			blastStrength *= 1.3f;
			blastStunTime *= 1.3f;
			//maxSpecialAmmo += 2;
			shipRateOfAOE *= 0.70;
		}
		break;
	}
	case (TEMP_HEALTH): { //number of shots
		AddHealth(3);
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
	case (NO_UPGRADES_LEFT): {
		break;
	}
	default : {
		break;
	}
	}
}

float PlayerChar::GetCurrHealth()
{
	return health;
}

void PlayerChar::CollideWithEnemy(CollisionData i_coll)
{
	auto enemyPtr = dynamic_cast<Enemy*>(i_coll.entB.get());
	auto timeSinceDamaged = std::chrono::duration_cast<std::chrono::seconds>(hiResTime::now() - lastDamageReceived);
	if ((enemyPtr) && (timeSinceDamaged.count() >= dmgRate)) {
		if (!enemyPtr->metal) {
			TakeDamage(1.0f, i_coll);
		}
	}
}

void PlayerChar::CollideWithPainWall(CollisionData i_coll)
{
	auto timeSinceDamaged = std::chrono::duration_cast<std::chrono::seconds>(hiResTime::now() - lastDamageReceived);
	if (timeSinceDamaged.count() >= dmgRate) {
		GLBVRS::RSRCS->PlaySound(RESOURCES::FIREBALL);
		TakeDamage(1.0f, i_coll);
	}
}

void PlayerChar::CollideWithDoor(CollisionData i_coll)
{
	auto doorPtr = dynamic_cast<Door*>(i_coll.entB.get());
	if ((doorPtr) && (doorPtr->IsOpen())) {
		GLBVRS::RSRCS->PlaySound(RESOURCES::MAGIC7);
		rb.ignoreForcesThisStep = true;
		rb.ResetPosition(doorPtr->GetOutPos());
		GLBVRS::GPTR->RequestTravelToSector(doorPtr->GetOutCoord());
	}
}

void PlayerChar::CollideWithEndObject(CollisionData i_coll)
{
	auto endPtr = dynamic_cast<EndObject*>(i_coll.entB.get());
	if ((endPtr) && (endPtr->active)) {
		GLBVRS::GPTR->RequestGoToNextLvl();
	}
}

void PlayerChar::CollideWithPowUp(CollisionData i_coll)
{
	auto powPtr = dynamic_cast<PowerUp*>(i_coll.entB.get());
	if (powPtr) {
		ReceivePowerUp(powPtr->powType);
		powPtr->Destroy();
	}
}

void PlayerChar::CollideWithProjectile(CollisionData i_coll)
{
	auto projPtr = dynamic_cast<Projectile*>(i_coll.entB.get());
	if ((projPtr) && (projPtr->projType == 1)) {
		TakeDamage(1.0f, i_coll);
	}
}

void PlayerChar::UpdateVisuals(float i_lerpFraction)
{
	Vector2f lerpPos = rb.GetLerpPosition(i_lerpFraction);
	float lerpOrient = rb.GetLerpOrient(i_lerpFraction);

	auto poly = dynamic_cast<Polygon *> (rb.shape.get());
	float baseDiam = poly->GetDistToCorner() * (3.0f / 12.0f);


	//Thrusters

	//jetshape
	Polygon* polyPtr = dynamic_cast<Polygon*>(rb.shape.get());
	std::vector<Vector2f> verts = polyPtr->pointArr;

	//lerped vertices
	Eigen::Rotation2D<float> rotation(rb.transform.orient);
	Matrix2f rotationMatrix = rotation.toRotationMatrix();
	int size = verts.size();
	for (int i = 0; i < size; ++i) {
		verts[i] = rotationMatrix * verts[i];
	}

	int vertNum = verts.size();
	float thrusterRadius = 6.0f;
	for (int i = 0; i < vertNum; i++) {
		//fireshape
		//must check to see that the direction of thrust is not opposite to angle from the center to the vert
		float angleInDegrees = ((Math::VectToAngle(-currThrustDirVect)*180.0f) / PI);
		float vertAngleinDegrees = ((Math::VectToAngle(verts[i])*180.0f) / PI);
		float angularDiff = abs(angleInDegrees - vertAngleinDegrees);
		angularDiff = std::min(angularDiff, 360 - angularDiff);
		if ((currThrustDirVect.norm() > 0) && (angularDiff < 100)) {
			float thrustFireLength = std::min((timeSinceDirChange * 15), 30.0f);
			std::vector<Vector2f> flamePoints = { Vector2f(0.0f, 0.0f), Vector2f(2.5f, 5.0f), Vector2f(15.0f + thrustFireLength, 0.0f), Vector2f(2.5f, -5.0f) };
			//std::shared_ptr<Polygon> firePolyShape = std::make_shared<Polygon>(irregPoly1);
			std::shared_ptr<sf::ConvexShape> polyPtr = std::make_shared<sf::ConvexShape>(4);
			for (int i = 0; i < flamePoints.size(); ++i) {
				sf::Vector2f vect = sf::Vector2f(flamePoints[i][0], flamePoints[i][1]);
				polyPtr->setPoint(i, vect);
			}
			std::shared_ptr<sf::Shape> flameShapePtr = move(polyPtr);
			flameShapePtr->setFillColor(RED);
			flameShapePtr->setOutlineColor(YELLOWCYBER);
			flameShapePtr->setPosition(sf::Vector2f(lerpPos[0] + verts[i][0], lerpPos[1] + verts[i][1]));
			
			float flameOutLineIntensity = 4.0f + -((int)((timeSinceDirChange * 2.0f)+i) % 2);

			flameShapePtr->setOutlineThickness(flameOutLineIntensity);
			//rotation
			flameShapePtr->setRotation(angleInDegrees);
			visuals->emplace_back(flameShapePtr);
		}


		//cirlces 
		std::shared_ptr<sf::CircleShape> circlePtr = std::make_shared<sf::CircleShape>();
		circlePtr->setRadius(thrusterRadius);
		std::shared_ptr<sf::Shape> shapePtr = move(circlePtr);
		shapePtr->setOrigin(sf::Vector2f(thrusterRadius, thrusterRadius));
		shapePtr->setFillColor(outlineColor);
		shapePtr->setOutlineColor(outlineColor);
		shapePtr->setPosition(sf::Vector2f(lerpPos[0] + verts[i][0], lerpPos[1] + verts[i][1]));
		shapePtr->setOutlineThickness(2.0f);
		visuals->emplace_back(shapePtr);
	}

	//Cannon Shaft
	float prevAngleRads;
	Vector2f prevDirVect;
	Vector2f mosDir;
	if (!pController.IsControllerConnected()) {
		mosDir = pController.GetMousePos() - lerpPos;
	}
	else {
		Vector2f stickDir = pController.GetRightStickVect();
		if ((stickDir[0] != 0.0f) && (stickDir[1] != 0.0f)) {
			mosDir = stickDir;
			lastCannonVect = stickDir;
		}
		else {
			mosDir = lastCannonVect;
		}
	}
	float mouseAngleRad = Math::VectToAngle(mosDir);
	float currAngleRads = mouseAngleRad - (0.0872665 * (numShots - 1));
	Vector2f currDirVect = Math::AngleToVect(currAngleRads);
	//drawablePtrVect cannons = drawablePtrVect();
	for (int i = 0; i < numShots; i++) {
		float shaftDiam = baseDiam - (numShots * (baseDiam * 0.04));
		std::shared_ptr<sf::Shape> drawShapeShaft = std::make_shared<sf::RectangleShape>(sf::Vector2f(poly->GetDistToCorner(), shaftDiam));
		drawShapeShaft->setOrigin(sf::Vector2f(0.0f, baseDiam / 2.0f));
		drawShapeShaft->setFillColor(currColorShaft);
		drawShapeShaft->setOutlineColor(outlineColor);
		drawShapeShaft->setPosition(sf::Vector2f(lerpPos[0], lerpPos[1]));
		drawShapeShaft->setOutlineThickness(2.0f);
		drawShapeShaft->setTexture(texturePtr.get());
		drawShapeShaft->setRotation(Math::VectToAngle(currDirVect) * 57.2958);
		visuals->emplace_back(drawShapeShaft);

		prevAngleRads = currAngleRads;
		prevDirVect = currDirVect;
		currAngleRads = prevAngleRads + 0.174533 * (pow(-1, i) * (numShots-i));
		currDirVect = Math::AngleToVect(currAngleRads);
	}

	//extra cannon cover
	if (numShots > 1) {
		float cornerDist = poly->GetDistToCorner();
		auto sharedPtrPolyShape = Physics::CreateRegularPolygon(6, cornerDist * 0.65);
		auto sfmlPolyShapeBase = sharedPtrPolyShape->GetSFMLRepr();
		//sfmlPolyShapeBase->setOrigin(sf::Vector2f(radius, radius));
		//Burst Charge indicator
		sfmlPolyShapeBase->setTexture(texturePtr.get());
		sfmlPolyShapeBase->setFillColor(currColorInnerHex);
		sfmlPolyShapeBase->setOutlineColor(currColorInnerHex);
		sfmlPolyShapeBase->setPosition(sf::Vector2f(lerpPos[0], lerpPos[1]));
		sfmlPolyShapeBase->setOutlineThickness(2.0f);
		
		float angleInDegrees = ((rb.GetLerpOrient(i_lerpFraction)*180.0f) / PI);
		sfmlPolyShapeBase->setRotation(angleInDegrees);
		visuals->emplace_back(sfmlPolyShapeBase);
	}


	//Center Circle
	float radius = baseDiam * (3.0f / 5.0f);
	std::shared_ptr<sf::Shape> drawShapeBase = std::make_shared<sf::CircleShape>(radius);
	drawShapeBase->setOrigin(sf::Vector2f(radius, radius));
	//Burst Charge indicator
	weaponDelay = (std::chrono::duration_cast<std::chrono::microseconds>(hiResTime::now() - lastAOEFired)).count() / 1000000.0f;
	sf::Color centerColorFill;
	sf::Color centerColorOutLine;
	if (weaponDelay >= shipRateOfAOE) {
		centerColorFill = CYAN;
		centerColorOutLine = CYAN;
	}
	else {
		centerColorFill = FORDSILVER;
		centerColorOutLine = FORDSILVER;
	}
	drawShapeBase->setFillColor(centerColorFill);
	drawShapeBase->setOutlineColor(centerColorOutLine);
	drawShapeBase->setPosition(sf::Vector2f(lerpPos[0], lerpPos[1]));
	drawShapeBase->setOutlineThickness(2.0f);
	visuals->emplace_back(drawShapeBase);
}

void PlayerChar::ShootBasic(Vector2f i_projDir)
{
	weaponDelay = (std::chrono::duration_cast<std::chrono::microseconds>(hiResTime::now() - lastShotFired)).count() / 1000000.0f;
	if (weaponDelay >= shipRateOfFire) {
		GLBVRS::RSRCS->PlaySound(RESOURCES::SHOT2);
		lastShotFired = hiResTime::now();
		Vector2f projDir = i_projDir;
		auto poly = dynamic_cast<Polygon *> (rb.shape.get());
		float projCenterOffset = poly->GetDistToCorner() + GLBVRS::PROJECTILE_RADIUS + 10.0f;
		projDir.normalize();
		float prevAngleRads;
		Vector2f prevDirVect;
		float mouseAngleRad = Math::VectToAngle(projDir);
		float currAngleRads = mouseAngleRad - (0.0872665 * (numShots - 1));
		//Original angle is shifted back so cluster is still centered on cursor
		Vector2f currDirVect = Math::AngleToVect(currAngleRads);
		//currDirVect.normalize();
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
		if ((shipLvl.at(SCATTER) >= 3) && (numShots < 3)) {
			std::cout << "wtf\n";
		}
	}
}

void PlayerChar::ShootWall(Vector2f i_projDir)
{
	wallDelay = (std::chrono::duration_cast<std::chrono::microseconds>(hiResTime::now() - lastWallFired)).count() / 1000000.0f;
	if (wallDelay >= shipRateOfFireWall) {
		lastWallFired = hiResTime::now();
		auto poly = dynamic_cast<Polygon *> (rb.shape.get());
		float projCenterOffset = poly->GetDistToCorner() + GLBVRS::PROJECTILE_RADIUS;
		GLBVRS::RSRCS->PlaySound(RESOURCES::SHOOT14);
		Vector2f projectileDirFront = i_projDir;
		projectileDirFront.normalize();
		Vector2f projectileDirBack = -1 * i_projDir;
		projectileDirBack.normalize();
		Vector2f projectileDirLeft = i_projDir;
		projectileDirLeft = Vector2f(projectileDirLeft[1], -1 * projectileDirLeft[0]);
		projectileDirLeft.normalize();
		Vector2f projectileDirRight = i_projDir;
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
		std::shared_ptr<Entity> projectile = std::make_shared<Blast>(rb.transform.pos, 0, blastStrength, blastStunTime, BlastRadius);
		spawnVect.push_back(projectile);
	}
}