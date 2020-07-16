#include "Sector.h"

Sector::Sector(Level* i_lvlPtr, RESOURCES* i_resources, sf::Color i_colA, sf::Color i_colB) :
	myLevel { i_lvlPtr }, resources { i_resources }, colPalA { i_colA }, colPalB { i_colB }
{
	InitializeSector();
}

Sector::~Sector()
{
}

std::list<std::shared_ptr<Entity>>* Sector::GetSectorEntities()
{
	/*if (firstsPhase) {*/
		return &lvlEntitiesPhase1;
	/*}
	else {
		return &lvlEntitiesPhase2;
	}*/
}

void Sector::AddEntPtrToSector(std::shared_ptr<Entity> i_entPtr)
{
	lvlEntitiesPhase1.push_back(i_entPtr);
}

void Sector::GenerateEnemies(int i_numEnems, TypeID enemyType, SCREEN_AREA i_area, int i_phaseNum, DIFFICULTY i_diff, int i_sizeMod)
{ //zero is the intended default for the size mod, adding to the size mod will make the enemies larger and decrease difficulty
	auto screenAreas = GetScreenAreas(i_area);
	for (int i = 0; i < i_numEnems; ++i) {

		std::random_device sizeSeed;
		std::mt19937 genRoomSeed(sizeSeed());
		std::discrete_distribution<> sizeModDist({ 12, 13, 50, 13, 12 });
		int rand1 = sizeModDist(genRoomSeed);
		int ranDifwidth = (rand1 - 2 + i_sizeMod) * 5;
		int rand2 = sizeModDist(genRoomSeed);
		int randDifHeight = (rand2 - 2 + i_sizeMod) * 5;

		//Create random spawn position
		std::shared_ptr<Shape> shape = std::make_shared<Circle>(40);
		RigidBody projBody = RigidBody(shape, ROCK);
		int zoneInd = i % (screenAreas.size());
		std::random_device rd;
		std::mt19937 gen1(rd());
		int leftX = (int)std::get<0>(screenAreas[zoneInd])[0];
		int rightX = (int)std::get<1>(screenAreas[zoneInd])[0];
		int topY = (int)std::get<0>(screenAreas[zoneInd])[1];
		int bottomY = (int)std::get<1>(screenAreas[zoneInd])[1];
		std::uniform_int_distribution<> xCordDist(leftX, rightX);
		std::uniform_int_distribution<> yCordDist(topY, bottomY);
		int randXcordInBounds = xCordDist(gen1);
		int randYcordInBounds = yCordDist(gen1);
		Vector2f spawnPos(randXcordInBounds, randYcordInBounds);
		std::shared_ptr<Entity> ent;
		switch (enemyType) {
		case ENEMY_SEEK_PUSH: {
			std::shared_ptr<Shape> shape = std::make_shared<Rectangle>(60 + ranDifwidth, 60 + randDifHeight);
			//std::shared_ptr<Shape> shape = std::make_shared<Rectangle>(60 + randSizeDiff, 60 + randSizeDiff2);
			Material Rock = Material(0.6f, 0.1f, 0.6f, 0.3f);
			RigidBody projBody = RigidBody(shape, Rock);
			ent = std::make_shared<Enemy>(myLevel->charPtr, this, i_diff, spawnPos, projBody);
			break;
		}
		case ENEMY_RAND_PUSH:
			std::shared_ptr<Shape> shape = std::make_shared<Circle>(40 + ranDifwidth);
			RigidBody projBody = RigidBody(shape, ROCK);
			ent = std::make_shared<CrazyBoi>(myLevel->charPtr, this, i_diff, spawnPos, projBody);
			break;
		}
		if (i_phaseNum == 1) {
			sectEnemyNum += 1;
			lvlEntitiesPhase1.push_back(ent);
		}
		else {
			lvlEntitiesPhase2.push_back(ent);
		}
	}
}

void Sector::AddWallsToLevel()
{
	Material Static = Material(0.0f, 0.4f, 0.4f, 0.2f);

	float wall_thick = 5.0f;

	//Right wall
	std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(GLBVRS::HR_MRG * wall_thick, GLBVRS::CRT_HGHT * 2.0f);
	RigidBody rightWallBody = RigidBody(vertRect1, Static);
	std::shared_ptr<Entity> rightWall = std::make_shared<Wall>(
		Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH) + ((GLBVRS::HR_MRG * wall_thick) / 2.0f),
				GLBVRS::VRT_MRG + (GLBVRS::CRT_HGHT / 2.0f)), this, rightWallBody, colPalA, colPalB);
	lvlEntitiesPhase1.push_back(rightWall);
	//Left wall
	std::shared_ptr<Shape> vertRect2 = std::make_shared<Rectangle>(GLBVRS::HR_MRG * wall_thick, GLBVRS::CRT_HGHT * 2.0f);
	RigidBody leftWallBody = RigidBody(vertRect2, Static);
	std::shared_ptr<Entity> leftWall = std::make_shared<Wall>(
		Vector2f((GLBVRS::HR_MRG / 2.0f) - (GLBVRS::HR_MRG * 2.0),
			GLBVRS::VRT_MRG + (GLBVRS::CRT_HGHT / 2.0f)), this, leftWallBody, colPalA, colPalB);
	AddEntPtrToSector(leftWall);
	//Top wall
	std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(GLBVRS::CRT_WDTH + (GLBVRS::HR_MRG * 8), GLBVRS::VRT_MRG * wall_thick);
	RigidBody wallBody1 = RigidBody(horRect1, Static);
	std::shared_ptr<Entity> upperWall = std::make_shared<Wall>(
		Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH / 2.0f), 
				- (GLBVRS::VRT_MRG * 1.5f)), this, wallBody1, colPalA, colPalB);
	lvlEntitiesPhase1.push_back(upperWall);

	//Bottom wall
	std::shared_ptr<Shape> horRect2 = std::make_shared<Rectangle>(GLBVRS::CRT_WDTH + (GLBVRS::HR_MRG * 8), GLBVRS::VRT_MRG * wall_thick);
	RigidBody lowerWallBody = RigidBody(horRect2, Static);
	std::shared_ptr<Entity> lowerWall = std::make_shared<Wall>(
		Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH / 2.0f),
			(GLBVRS::VRT_MRG * 2.5f) + GLBVRS::CRT_HGHT + GLBVRS::VRT_MRG)), this, lowerWallBody, colPalA, colPalB);
	lvlEntitiesPhase1.push_back(lowerWall);

}

void Sector::AddPainfullWallsToLevel()
{
	Material Static = Material(0.0f, 0.4f, 0.4f, 0.2f);

	float halfCrtVrtWall = GLBVRS::CRT_HGHT * (1.0f / 2.0f) - DOOR_WIDTH;
	float halfCrtHorWall = GLBVRS::CRT_WDTH * (1.0f / 2.0f) - DOOR_WIDTH;


	std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(GLBVRS::HR_MRG, halfCrtVrtWall);
	RigidBody rightWallBody = RigidBody(vertRect1, Static);

	std::shared_ptr<Entity> rightWallUp = std::make_shared<PainWall>(
		Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH)+(GLBVRS::HR_MRG / 2.0f),
			GLBVRS::VRT_MRG - (DOOR_WIDTH / 2.0f ) + (GLBVRS::CRT_HGHT * ( 1.0f / 4.0f))), this, rightWallBody);
	lvlEntitiesPhase1.push_back(rightWallUp);
	std::shared_ptr<Entity> rightWallDown = std::make_shared<PainWall>(
		Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH)+(GLBVRS::HR_MRG / 2.0f),
			GLBVRS::VRT_MRG + (DOOR_WIDTH / 2.0f) + (GLBVRS::CRT_HGHT * (3.0f / 4.0f))), this, rightWallBody);
	lvlEntitiesPhase1.push_back(rightWallDown);

	std::shared_ptr<Shape> vertRect2 = std::make_shared<Rectangle>(GLBVRS::HR_MRG, halfCrtVrtWall);
	RigidBody leftWallBody = RigidBody(vertRect2, Static);

	std::shared_ptr<Entity> leftWallUp = std::make_shared<PainWall>(
		Vector2f((GLBVRS::HR_MRG / 2.0f),
			GLBVRS::VRT_MRG - (DOOR_WIDTH / 2.0f) + (GLBVRS::CRT_HGHT * (1.0f / 4.0f))), this, leftWallBody);
	AddEntPtrToSector(leftWallUp);
	std::shared_ptr<Entity> leftWallDown = std::make_shared<PainWall>(
		Vector2f((GLBVRS::HR_MRG / 2.0f),
			GLBVRS::VRT_MRG + (DOOR_WIDTH / 2.0f) + (GLBVRS::CRT_HGHT * (3.0f / 4.0f))), this, leftWallBody);
	AddEntPtrToSector(leftWallDown);

	std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(halfCrtHorWall, GLBVRS::VRT_MRG);
	RigidBody wallBody1 = RigidBody(horRect1, Static);

	std::shared_ptr<Entity> upperWallLeft = std::make_shared<PainWall>(
		Vector2f(GLBVRS::HR_MRG - (DOOR_WIDTH / 2.0f) + (GLBVRS::CRT_WDTH * (1.0f / 4.0f)),
		(GLBVRS::VRT_MRG / 2.0f)), this, wallBody1);
	lvlEntitiesPhase1.push_back(upperWallLeft);
	std::shared_ptr<Entity> upperWallRight = std::make_shared<PainWall>(
		Vector2f(GLBVRS::HR_MRG + (DOOR_WIDTH / 2.0f) + (GLBVRS::CRT_WDTH * (3.0f / 4.0f)),
		(GLBVRS::VRT_MRG / 2.0f)), this, wallBody1);
	lvlEntitiesPhase1.push_back(upperWallRight);


	std::shared_ptr<Shape> horRect2 = std::make_shared<Rectangle>(halfCrtHorWall, GLBVRS::VRT_MRG);
	RigidBody lowerWallBody = RigidBody(horRect2, Static);

	std::shared_ptr<Entity> lowerWallLeft = std::make_shared<PainWall>(
		Vector2f(Vector2f(GLBVRS::HR_MRG - (DOOR_WIDTH / 2.0f) + (GLBVRS::CRT_WDTH * (1.0f / 4.0f)),
		(GLBVRS::VRT_MRG / 2.0f) + GLBVRS::CRT_HGHT + GLBVRS::VRT_MRG)), this, lowerWallBody);
	lvlEntitiesPhase1.push_back(lowerWallLeft);
	std::shared_ptr<Entity> lowerWallRight = std::make_shared<PainWall>(
		Vector2f(Vector2f(GLBVRS::HR_MRG + (DOOR_WIDTH / 2.0f) + (GLBVRS::CRT_WDTH * (3.0f / 4.0f)),
		(GLBVRS::VRT_MRG / 2.0f) + GLBVRS::CRT_HGHT + GLBVRS::VRT_MRG)), this, lowerWallBody);
	lvlEntitiesPhase1.push_back(lowerWallRight);

}

void Sector::AddRandomPainWall(int i_index)
{
	Material Static = Material(0.0f, 0.4f, 0.4f, 0.2f);

	float halfCrtVrtWall = GLBVRS::CRT_HGHT * (1.0f / 2.0f) - DOOR_WIDTH;
	float halfCrtHorWall = GLBVRS::CRT_WDTH * (1.0f / 2.0f) - DOOR_WIDTH;

	switch (i_index) {
	case (0): {
		std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(GLBVRS::HR_MRG, halfCrtVrtWall);
		RigidBody rightWallBody = RigidBody(vertRect1, Static);
		std::shared_ptr<Entity> rightWallUp = std::make_shared<PainWall>(
			Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH)+(GLBVRS::HR_MRG / 2.0f), GLBVRS::VRT_MRG - (DOOR_WIDTH / 2.0f) + (GLBVRS::CRT_HGHT * (1.0f / 4.0f))), this, rightWallBody);
		lvlEntitiesPhase1.push_back(rightWallUp);
		break;
	}
	case (1): {
		std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(GLBVRS::HR_MRG, halfCrtVrtWall);
		RigidBody rightWallBody = RigidBody(vertRect1, Static);
		std::shared_ptr<Entity> rightWallDown = std::make_shared<PainWall>(
			Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH)+(GLBVRS::HR_MRG / 2.0f), GLBVRS::VRT_MRG + (DOOR_WIDTH / 2.0f) + (GLBVRS::CRT_HGHT * (3.0f / 4.0f))), this, rightWallBody);
		lvlEntitiesPhase1.push_back(rightWallDown);
		break;
	}
	case (2): {
		std::shared_ptr<Shape> vertRect2 = std::make_shared<Rectangle>(GLBVRS::HR_MRG, halfCrtVrtWall);
		RigidBody leftWallBody = RigidBody(vertRect2, Static);
		std::shared_ptr<Entity> leftWallUp = std::make_shared<PainWall>(
			Vector2f((GLBVRS::HR_MRG / 2.0f), GLBVRS::VRT_MRG - (DOOR_WIDTH / 2.0f) + (GLBVRS::CRT_HGHT * (1.0f / 4.0f))), this, leftWallBody);
		AddEntPtrToSector(leftWallUp);
		break;
	}
	case (3): {
		std::shared_ptr<Shape> vertRect2 = std::make_shared<Rectangle>(GLBVRS::HR_MRG, halfCrtVrtWall);
		RigidBody leftWallBody = RigidBody(vertRect2, Static);
		std::shared_ptr<Entity> leftWallDown = std::make_shared<PainWall>(
			Vector2f((GLBVRS::HR_MRG / 2.0f), GLBVRS::VRT_MRG + (DOOR_WIDTH / 2.0f) + (GLBVRS::CRT_HGHT * (3.0f / 4.0f))), this, leftWallBody);
		AddEntPtrToSector(leftWallDown);
		break;
	}
	case (4): {
		std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(halfCrtHorWall, GLBVRS::VRT_MRG);
		RigidBody wallBody1 = RigidBody(horRect1, Static);
		std::shared_ptr<Entity> upperWallLeft = std::make_shared<PainWall>(
			Vector2f(GLBVRS::HR_MRG - (DOOR_WIDTH / 2.0f) + (GLBVRS::CRT_WDTH * (1.0f / 4.0f)), (GLBVRS::VRT_MRG / 2.0f)), this, wallBody1);
		lvlEntitiesPhase1.push_back(upperWallLeft);
		break;
	}
	case (5): {
		std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(halfCrtHorWall, GLBVRS::VRT_MRG);
		RigidBody wallBody1 = RigidBody(horRect1, Static);
		std::shared_ptr<Entity> upperWallRight = std::make_shared<PainWall>(
			Vector2f(GLBVRS::HR_MRG + (DOOR_WIDTH / 2.0f) + (GLBVRS::CRT_WDTH * (3.0f / 4.0f)), (GLBVRS::VRT_MRG / 2.0f)), this, wallBody1);
		lvlEntitiesPhase1.push_back(upperWallRight);
		break;
	}
	case (6): {
		std::shared_ptr<Shape> horRect2 = std::make_shared<Rectangle>(halfCrtHorWall, GLBVRS::VRT_MRG);
		RigidBody lowerWallBody = RigidBody(horRect2, Static);
		std::shared_ptr<Entity> lowerWallLeft = std::make_shared<PainWall>(
			Vector2f(Vector2f(GLBVRS::HR_MRG - (DOOR_WIDTH / 2.0f) + (GLBVRS::CRT_WDTH * (1.0f / 4.0f)), (GLBVRS::VRT_MRG / 2.0f) + GLBVRS::CRT_HGHT + GLBVRS::VRT_MRG)), this, lowerWallBody);
		lvlEntitiesPhase1.push_back(lowerWallLeft);
		break;
	}
	case (7): {
		std::shared_ptr<Shape> horRect2 = std::make_shared<Rectangle>(halfCrtHorWall, GLBVRS::VRT_MRG);
		RigidBody lowerWallBody = RigidBody(horRect2, Static);
		std::shared_ptr<Entity> lowerWallRight = std::make_shared<PainWall>(
			Vector2f(Vector2f(GLBVRS::HR_MRG + (DOOR_WIDTH / 2.0f) + (GLBVRS::CRT_WDTH * (3.0f / 4.0f)), (GLBVRS::VRT_MRG / 2.0f) + GLBVRS::CRT_HGHT + GLBVRS::VRT_MRG)), this, lowerWallBody);
		lvlEntitiesPhase1.push_back(lowerWallRight);
		break;
	}
	}
}

void Sector::PopulateEntranceRoom()
{
	std::shared_ptr<Entity> lowerWall = std::make_shared<EndObject>(this,
				Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH / 2.0f), GLBVRS::CRT_HGHT / 2.0)));
	lvlEntitiesPhase1.push_back(lowerWall);

	std::shared_ptr<Entity> smallShipPOW = std::make_shared<PowerUp>(this,
		Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH / 4.0f), GLBVRS::CRT_HGHT / 2.0)), WEAP_SPEED);
	lvlEntitiesPhase1.push_back(smallShipPOW);

	std::shared_ptr<Entity> smallShipPOW2 = std::make_shared<PowerUp>(this,
		Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH * (3.0f / 4.0f)), GLBVRS::CRT_HGHT / 2.0)), WEAP_SPEED);
	lvlEntitiesPhase1.push_back(smallShipPOW2);

	std::shared_ptr<Entity> smallShipPOW3 = std::make_shared<PowerUp>(this,
		Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH / 4.0f), GLBVRS::CRT_HGHT / 4.0)), WEAP_SPEED);
	lvlEntitiesPhase1.push_back(smallShipPOW3);

	std::shared_ptr<Entity> smallShipPOW4 = std::make_shared<PowerUp>(this,
		Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH / 4.0f), GLBVRS::CRT_HGHT / 4.0)), WEAP_SPEED);
	lvlEntitiesPhase1.push_back(smallShipPOW4);

	std::shared_ptr<Entity> smallShipPOW41 = std::make_shared<PowerUp>(this,
		Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH * (3.0f / 4.0f)), GLBVRS::CRT_HGHT / 4.0)), BIG_SHIP);
	lvlEntitiesPhase1.push_back(smallShipPOW41);

	std::shared_ptr<Entity> smallShipPOW42 = std::make_shared<PowerUp>(this,
		Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH * (3.0f / 4.0f)), GLBVRS::CRT_HGHT / 4.0)), BIG_SHIP);
	lvlEntitiesPhase1.push_back(smallShipPOW42);

	std::shared_ptr<Entity> smallShipPOW43 = std::make_shared<PowerUp>(this,
		Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH * (3.0f / 4.0f)), GLBVRS::CRT_HGHT / 4.0)), BIG_SHIP);
	lvlEntitiesPhase1.push_back(smallShipPOW43);

	std::shared_ptr<Entity> smallShipPOW44 = std::make_shared<PowerUp>(this,
		Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH * (3.0f / 4.0f)), GLBVRS::CRT_HGHT / 4.0)), BIG_SHIP);
	lvlEntitiesPhase1.push_back(smallShipPOW44);
}

void Sector::PopulateBossRoom(DIFFICULTY i_diff)
{
	isBossRoom = true;

	std::shared_ptr<Shape> shape = std::make_shared<Circle>(70);
	RigidBody projBody = RigidBody(shape, ROCK);
	std::shared_ptr<Entity> ent = std::make_shared<ShootyBoi>(myLevel->charPtr, this, i_diff, Vector2f(400.0f, 400.0f), projBody);
	lvlEntitiesPhase1.push_back(ent);
	++sectEnemyNum;
}

void Sector::RemoveDestroyedEntities() {
	std::list<std::shared_ptr<Entity>>::iterator iter = lvlEntitiesPhase1.begin();
	while (iter != lvlEntitiesPhase1.end()) {
		if (iter._Ptr->_Myval->MarkedForDeath()) {
			int entType = iter._Ptr->_Myval->GetTypeID();
			if ((entType == ENEMY_SEEK) || (entType == ENEMY_RAND) || (entType == ENEMY_SEEK) || 
				(entType == ENEMY_SEEK_PUSH) || (entType == ENEMY_RAND_PUSH) || (entType == ENEMY_BOSS)) {
				--sectEnemyNum;
				if (sectEnemyNum <= 0) {
					resources->PlaySound(RESOURCES::POWERUP4);
				}
			}
			lvlEntitiesPhase1.erase(iter++);
		}
		else { ++iter; }
	}
	if 	((isBossRoom) && (sectEnemyNum <= 0) && (firstPhase)) {
		SwitchLevelToPhaseTwo();
	}
}

void Sector::AddTerrain(int i_terrainType)
{
	float smallWidth = GLBVRS::HR_MRG / 2.5f;
	float smallHeight = GLBVRS::CRT_HGHT / 4.0f;
	float bigWidth = GLBVRS::HR_MRG / 2.5f;
	float bigHeight = GLBVRS::CRT_HGHT / 1.5f;
	switch (i_terrainType) {
		//small walls
	case 0: {
		std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(smallWidth, smallHeight);
		RigidBody rightWallBody = RigidBody(vertRect1, STATIC);
		std::shared_ptr<Entity> rightWall = std::make_shared<Wall>(
			Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH * ( 1.0f / 5.0f)), GLBVRS::VRT_MRG + (GLBVRS::CRT_HGHT / 2.0f)), this, rightWallBody);
		lvlEntitiesPhase1.push_back(rightWall);
		break;
	}
	case 1: {
		std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(smallWidth, GLBVRS::CRT_HGHT / 4.0f);
		RigidBody rightWallBody = RigidBody(vertRect1, STATIC);
		std::shared_ptr<Entity> rightWall = std::make_shared<Wall>(
			Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH *  (4.0f / 5.0f)), GLBVRS::VRT_MRG + (GLBVRS::CRT_HGHT / 2.0f)), this, rightWallBody);
		lvlEntitiesPhase1.push_back(rightWall);
		break;
	}
	case 2: {
		std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(GLBVRS::CRT_WDTH / 4.0f, smallWidth);
		RigidBody wallBody1 = RigidBody(horRect1, STATIC);
		std::shared_ptr<Entity> upperWall = std::make_shared<Wall>(
			Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH / 2.0f), GLBVRS::VRT_MRG + (GLBVRS::CRT_HGHT * (1.0f / 5.0f))), this, wallBody1);
		lvlEntitiesPhase1.push_back(upperWall);
		break;
	}
	case 3: {
		std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(GLBVRS::CRT_WDTH / 4.0f, smallWidth);
		RigidBody wallBody1 = RigidBody(horRect1, STATIC);
		std::shared_ptr<Entity> upperWall = std::make_shared<Wall>(
			Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH / 2.0f), GLBVRS::VRT_MRG + (GLBVRS::CRT_HGHT * (4.0f / 5.0f))), this, wallBody1);
		lvlEntitiesPhase1.push_back(upperWall);
		break;
	}
	//big walls
	case 4: {
		std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(bigWidth, bigHeight);
		RigidBody rightWallBody = RigidBody(vertRect1, STATIC);
		std::shared_ptr<Entity> rightWall = std::make_shared<Wall>(
			Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH * (1.0f / 5.0f)), GLBVRS::VRT_MRG + (GLBVRS::CRT_HGHT / 2.0f)), this, rightWallBody);
		lvlEntitiesPhase1.push_back(rightWall);
		break;
	}
	case 5: {
		std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(bigWidth, bigHeight);
		RigidBody rightWallBody = RigidBody(vertRect1, STATIC);
		std::shared_ptr<Entity> rightWall = std::make_shared<Wall>(
			Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH *  (4.0f / 5.0f)), GLBVRS::VRT_MRG + (GLBVRS::CRT_HGHT / 2.0f)), this, rightWallBody);
		lvlEntitiesPhase1.push_back(rightWall);
		break;
	}
	case 6: {
		std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(bigHeight, bigWidth);
		RigidBody wallBody1 = RigidBody(horRect1, STATIC);
		std::shared_ptr<Entity> upperWall = std::make_shared<Wall>(
			Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH / 2.0f), GLBVRS::VRT_MRG + (GLBVRS::CRT_HGHT * (1.0f / 5.0f))), this, wallBody1);
		lvlEntitiesPhase1.push_back(upperWall);
		break;
	}
	case 7: {
		std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(bigHeight, bigWidth);
		RigidBody wallBody1 = RigidBody(horRect1, STATIC);
		std::shared_ptr<Entity> upperWall = std::make_shared<Wall>(
			Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH / 2.0f), GLBVRS::VRT_MRG + (GLBVRS::CRT_HGHT * (4.0f / 5.0f))), this, wallBody1);
		lvlEntitiesPhase1.push_back(upperWall);
		break;
	}
	}
}

void Sector::InitializeSector()
{
	AddWallsToLevel();
	sectEnemyNum = 0;
	isBossRoom = false;
	firstPhase = true;
}

void Sector::PlaySound(int i_soundNum)
{
	resources->PlaySound(i_soundNum);
}

void Sector::SwitchToPhaseTwo()
{
	firstPhase = false;
	for (std::shared_ptr<Entity> entPtr : lvlEntitiesPhase2) {
		auto enemy = dynamic_cast<Enemy*>(entPtr.get());
		enemy->TurnToMetal();
		lvlEntitiesPhase1.push_back(entPtr);
	}
}

void Sector::SwitchLevelToPhaseTwo()
{
	myLevel->SwitchSectorsToPhaseTwo();
}

std::vector<std::tuple<Vector2f, Vector2f>> Sector::GetScreenAreas(SCREEN_AREA i_area)
{
	//creates a bunch of tuples holding the top right corner and bottom left corner of valid rectangles in which to place enemies
	std::vector<std::tuple<Vector2f, Vector2f>> screenAreas;
	
	float crnrWdth = (GLBVRS::CRT_WDTH * (1.0f / 8.0f));
	float crnrHght = (GLBVRS::CRT_HGHT * (1.0f / 8.0f));
	Vector2f tpLftCrn = Vector2f(GLBVRS::HR_MRG, GLBVRS::VRT_MRG);
	Vector2f tpRghtCrn = Vector2f(GLBVRS::HR_MRG + GLBVRS::CRT_WDTH, GLBVRS::VRT_MRG);
	Vector2f bttmRghtCrn = Vector2f(GLBVRS::HR_MRG + GLBVRS::CRT_WDTH, GLBVRS::VRT_MRG + GLBVRS::CRT_HGHT);
	Vector2f bttmLftCrn = Vector2f(GLBVRS::HR_MRG, GLBVRS::VRT_MRG + GLBVRS::CRT_HGHT);

	float twoFifthsWidth = (GLBVRS::HR_MRG + GLBVRS::CRT_WDTH * (2.0f / 5.0f));
	float threeFifthsWidth = (GLBVRS::HR_MRG + GLBVRS::CRT_WDTH * (3.0f / 5.0f));
	float twoFifthsHeight = (GLBVRS::VRT_MRG + GLBVRS::CRT_HGHT * (2.0f / 5.0f));
	float threeFifthsHeight = (GLBVRS::VRT_MRG + GLBVRS::CRT_HGHT * (3.0f / 5.0f));


	float doorOffsetX = (GLBVRS::CRT_WDTH * (1.0f / 10.0f));
	float doorOffsetY = (GLBVRS::CRT_HGHT * (1.0f / 10.0f));



	switch (i_area) {
	case CORNERS: {
		std::tuple<Vector2f, Vector2f> upLeft = { tpLftCrn, Vector2f(tpLftCrn[0] + crnrWdth, tpLftCrn[1] + crnrHght) };
		std::tuple<Vector2f, Vector2f> upRight = { Vector2f(tpRghtCrn[0] - crnrWdth, tpRghtCrn[1]), Vector2f(tpRghtCrn[0], tpRghtCrn[1] + crnrHght) };
		std::tuple<Vector2f, Vector2f> lowRight = { Vector2f(bttmRghtCrn[0] - crnrWdth, bttmRghtCrn[1] - crnrHght), bttmRghtCrn };
		std::tuple<Vector2f, Vector2f> lowLeft = { Vector2f(bttmLftCrn[0], bttmLftCrn[1] - crnrHght), Vector2f(bttmLftCrn[0] + crnrWdth, bttmLftCrn[1]) };
		screenAreas.push_back(upLeft);
		screenAreas.push_back(upRight);
		screenAreas.push_back(lowRight);
		screenAreas.push_back(lowLeft);
		break;
	}
	case MARGINS: {
		std::tuple<Vector2f, Vector2f> tp = { tpLftCrn, Vector2f(tpRghtCrn[0], tpRghtCrn[1] + crnrHght) };
		std::tuple<Vector2f, Vector2f> rght= { Vector2f(tpRghtCrn[0] - crnrWdth, tpRghtCrn[1] + crnrHght), Vector2f(bttmRghtCrn[0], bttmRghtCrn[1] - crnrHght) };
		std::tuple<Vector2f, Vector2f> bttm= { Vector2f(bttmLftCrn[0], bttmLftCrn[1] - crnrHght), bttmRghtCrn };
		std::tuple<Vector2f, Vector2f> lft = { Vector2f(tpLftCrn[0], tpLftCrn[1] + crnrHght), Vector2f(bttmLftCrn[0] + crnrWdth, bttmLftCrn[1] - crnrHght) };
		screenAreas.push_back(tp);
		screenAreas.push_back(rght);
		screenAreas.push_back(bttm);
		screenAreas.push_back(lft);
		break;
	}
	case DOORS: { //Cant call before sector has door unsafe TODO
		for (std::shared_ptr<Entity> entPtr : lvlEntitiesPhase1) {
			if (auto door = dynamic_cast<Door*>(entPtr.get())) {
				std::vector<Vector2f> doorCords = door->rb.GetVertCords();
				Vector2f upLeftCord = doorCords[3];
				Vector2f btmRightCord = doorCords[1];
				switch (door->side) {
				case SCREEN_UP: {
					std::tuple<Vector2f, Vector2f> cord = { Vector2f(upLeftCord[0], upLeftCord[1] - doorOffsetY), Vector2f(btmRightCord[0], btmRightCord[1] - doorOffsetY) };
					screenAreas.push_back(cord);
					break;
				}
				case SCREEN_RIGHT: {
					std::tuple<Vector2f, Vector2f> cord = { Vector2f(upLeftCord[0] - doorOffsetX, upLeftCord[1]), Vector2f(btmRightCord[0] - doorOffsetX, btmRightCord[1]) };
					screenAreas.push_back(cord);

					break;
				}
				case SCREEN_DOWN: {
					std::tuple<Vector2f, Vector2f> cord = { Vector2f(upLeftCord[0], upLeftCord[1] + doorOffsetY), Vector2f(btmRightCord[0], btmRightCord[1] + doorOffsetY) };

					screenAreas.push_back(cord);
					break;
				}
				case SCREEN_LEFT: {
					std::tuple<Vector2f, Vector2f> cord = { Vector2f(upLeftCord[0] + doorOffsetX, upLeftCord[1]), Vector2f(btmRightCord[0] + doorOffsetX, btmRightCord[1]) };
					screenAreas.push_back(cord);
					break;
				}
				}
			}
		}
		break;
	}
	case CENTER: {
		std::tuple<Vector2f, Vector2f> center = { Vector2f(twoFifthsWidth,twoFifthsHeight), Vector2f(threeFifthsWidth, threeFifthsHeight) };
		screenAreas.push_back(center);
		break;
	}
	}
	return screenAreas;
}
