#include "Sector.h"

Sector::Sector(Level* i_lvlPtr, RESOURCES* i_resources) :
	myLevel { i_lvlPtr }, resources { i_resources }
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

void Sector::GenerateLevelCubes(int i_numCubes, int i_phaseNum)
{
	float offset = GLOBAL_CONSTANTS::SCREEN_WIDTH / 20.0f;
	std::vector<Vector2f> cornerZones;
	float cornerZoneWidth = GLOBAL_CONSTANTS::COURT_WIDTH / 8.0f;
	float cornerZoneHeight = GLOBAL_CONSTANTS::COURT_HEIGHT / 8.0f;
	//array of the positions representing the top left corners of the zones in which I will regularly spawn enemies
	cornerZones.push_back(Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN, GLOBAL_CONSTANTS::VERT_MARGIN));
	cornerZones.push_back(Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN + (GLOBAL_CONSTANTS::COURT_WIDTH * 7.0f / 8.0f), GLOBAL_CONSTANTS::VERT_MARGIN));
	cornerZones.push_back(Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN + (GLOBAL_CONSTANTS::COURT_WIDTH * 7.0f / 8.0f), GLOBAL_CONSTANTS::VERT_MARGIN + (GLOBAL_CONSTANTS::COURT_HEIGHT * 7.0f / 8.0f)));
	cornerZones.push_back(Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN, GLOBAL_CONSTANTS::VERT_MARGIN + (GLOBAL_CONSTANTS::COURT_HEIGHT * 7.0f / 8.0f)));
		for (int i = 0; i < i_numCubes; ++i) {

			std::random_device rdRoomSeed;  //Will be used to obtain a seed for the random number engine
			std::mt19937 genRoomSeed(rdRoomSeed()); //Standard mersenne_twister_engine seeded with rd()
			std::discrete_distribution<> extraRoomDist({ 12, 13, 50, 13, 12 });
			int rand1 = extraRoomDist(genRoomSeed);
			int randSizeDiff = (rand1 - 2) * 15;
			int rand2 = extraRoomDist(genRoomSeed);
			int randSizeDiff2 = (rand2 - 2) * 15;

			std::shared_ptr<Shape> shape = std::make_shared<Rectangle>(60 + randSizeDiff, 60 + randSizeDiff2);
			Material Rock = Material(0.6f, 0.1f, 0.6f, 0.3f);
			RigidBody projBody = RigidBody(shape, Rock);
			int zoneIndex = i % (cornerZones.size());
			Vector2f spawnPos(cornerZones[zoneIndex][0] + (cornerZoneWidth / i_numCubes), cornerZones[zoneIndex][1] + (cornerZoneHeight / i_numCubes));
			std::shared_ptr<Entity> ent = std::make_shared<Enemy>(myLevel->charPtr, this, spawnPos, projBody);
			if (i_phaseNum == 1) {
				sectEnemyNum += 1;
				lvlEntitiesPhase1.push_back(ent);
			}
			else {
				lvlEntitiesPhase2.push_back(ent);
			}

		}
}

void Sector::GenerateLevelCircles(int i_numCircs, int i_phaseNum)
{

	float offset = GLOBAL_CONSTANTS::SCREEN_WIDTH / 20.0f;
	std::vector<Vector2f> cornerZones;
	float cornerZoneWidth = GLOBAL_CONSTANTS::COURT_WIDTH / 8.0f;
	float cornerZoneHeight = GLOBAL_CONSTANTS::COURT_HEIGHT / 8.0f;
	//array of the positions representing the top left corners of the zones in which I will regularly spawn enemies
	cornerZones.push_back(Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN, GLOBAL_CONSTANTS::VERT_MARGIN));
	cornerZones.push_back(Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN + (GLOBAL_CONSTANTS::COURT_WIDTH * 7.0f / 8.0f), GLOBAL_CONSTANTS::VERT_MARGIN));
	cornerZones.push_back(Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN + (GLOBAL_CONSTANTS::COURT_WIDTH * 7.0f / 8.0f), GLOBAL_CONSTANTS::VERT_MARGIN + (GLOBAL_CONSTANTS::COURT_HEIGHT * 7.0f / 8.0f)));
	cornerZones.push_back(Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN, GLOBAL_CONSTANTS::VERT_MARGIN + (GLOBAL_CONSTANTS::COURT_HEIGHT * 7.0f / 8.0f)));
	for (int i = 0; i < i_numCircs; ++i) {

		std::random_device rdRoomSeed;  //Will be used to obtain a seed for the random number engine
		std::mt19937 genRoomSeed(rdRoomSeed()); //Standard mersenne_twister_engine seeded with rd()
		std::discrete_distribution<> extraRoomDist({ 12, 13, 50, 13, 12 });
		int rand1 = extraRoomDist(genRoomSeed);
		int randSizeDiff = (rand1 - 2) * 5;
		int rand2 = extraRoomDist(genRoomSeed);
		int randSizeDiff2 = (rand2 - 2) * 5;

		std::shared_ptr<Shape> shape = std::make_shared<Circle>(40 + randSizeDiff);
		Material Rock = Material(0.6f, 0.1f, 0.6f, 0.3f);
		RigidBody projBody = RigidBody(shape, Rock);
		int zoneIndex = i % (cornerZones.size());
		Vector2f spawnPos(cornerZones[zoneIndex][0] + (cornerZoneWidth / i_numCircs), cornerZones[zoneIndex][1] + (cornerZoneHeight / i_numCircs));
		std::shared_ptr<Entity> ent = std::make_shared<CrazyBoi>(myLevel->charPtr, this, spawnPos, projBody);
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


	std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(GLOBAL_CONSTANTS::HOR_MARGIN, GLOBAL_CONSTANTS::COURT_HEIGHT);
	RigidBody rightWallBody = RigidBody(vertRect1, Static);
	std::shared_ptr<Entity> rightWall = std::make_shared<Wall>(
		Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN + (GLOBAL_CONSTANTS::COURT_WIDTH)+(GLOBAL_CONSTANTS::HOR_MARGIN / 2.0f), GLOBAL_CONSTANTS::VERT_MARGIN + (GLOBAL_CONSTANTS::COURT_HEIGHT / 2.0f)), this, rightWallBody);
	lvlEntitiesPhase1.push_back(rightWall);

	std::shared_ptr<Shape> vertRect2 = std::make_shared<Rectangle>(GLOBAL_CONSTANTS::HOR_MARGIN, GLOBAL_CONSTANTS::COURT_HEIGHT);
	RigidBody leftWallBody = RigidBody(vertRect2, Static);
	std::shared_ptr<Entity> leftWall = std::make_shared<Wall>(
		Vector2f((GLOBAL_CONSTANTS::HOR_MARGIN / 2.0f), GLOBAL_CONSTANTS::VERT_MARGIN + (GLOBAL_CONSTANTS::COURT_HEIGHT / 2.0f)), this, leftWallBody);
	AddEntPtrToSector(leftWall);

	std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(GLOBAL_CONSTANTS::COURT_WIDTH + (GLOBAL_CONSTANTS::HOR_MARGIN * 2), GLOBAL_CONSTANTS::VERT_MARGIN);
	RigidBody wallBody1 = RigidBody(horRect1, Static);
	std::shared_ptr<Entity> upperWall = std::make_shared<Wall>(
		Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN + (GLOBAL_CONSTANTS::COURT_WIDTH / 2.0f), (GLOBAL_CONSTANTS::VERT_MARGIN / 2.0f)), this, wallBody1);
	lvlEntitiesPhase1.push_back(upperWall);


	std::shared_ptr<Shape> horRect2 = std::make_shared<Rectangle>(GLOBAL_CONSTANTS::COURT_WIDTH + (GLOBAL_CONSTANTS::HOR_MARGIN * 2), GLOBAL_CONSTANTS::VERT_MARGIN);
	RigidBody lowerWallBody = RigidBody(horRect2, Static);
	std::shared_ptr<Entity> lowerWall = std::make_shared<Wall>(
		Vector2f(Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN + (GLOBAL_CONSTANTS::COURT_WIDTH / 2.0f), (GLOBAL_CONSTANTS::VERT_MARGIN / 2.0f) + GLOBAL_CONSTANTS::COURT_HEIGHT + GLOBAL_CONSTANTS::VERT_MARGIN)), this, lowerWallBody);
	lvlEntitiesPhase1.push_back(lowerWall);

}

void Sector::AddPainfullWallsToLevel()
{
	Material Static = Material(0.0f, 0.4f, 0.4f, 0.2f);

	float halfCrtVrtWall = GLOBAL_CONSTANTS::COURT_HEIGHT * (1.0f / 2.0f) - DOOR_WIDTH;
	float halfCrtHorWall = GLOBAL_CONSTANTS::COURT_WIDTH * (1.0f / 2.0f) - DOOR_WIDTH;


	std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(GLOBAL_CONSTANTS::HOR_MARGIN, halfCrtVrtWall);
	RigidBody rightWallBody = RigidBody(vertRect1, Static);

	std::shared_ptr<Entity> rightWallUp = std::make_shared<PainWall>(
		Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN + (GLOBAL_CONSTANTS::COURT_WIDTH)+(GLOBAL_CONSTANTS::HOR_MARGIN / 2.0f), GLOBAL_CONSTANTS::VERT_MARGIN - (DOOR_WIDTH / 2.0f ) + (GLOBAL_CONSTANTS::COURT_HEIGHT * ( 1.0f / 4.0f))), this, rightWallBody);
	lvlEntitiesPhase1.push_back(rightWallUp);
	std::shared_ptr<Entity> rightWallDown = std::make_shared<PainWall>(
		Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN + (GLOBAL_CONSTANTS::COURT_WIDTH)+(GLOBAL_CONSTANTS::HOR_MARGIN / 2.0f), GLOBAL_CONSTANTS::VERT_MARGIN + (DOOR_WIDTH / 2.0f) + (GLOBAL_CONSTANTS::COURT_HEIGHT * (3.0f / 4.0f))), this, rightWallBody);
	lvlEntitiesPhase1.push_back(rightWallDown);

	std::shared_ptr<Shape> vertRect2 = std::make_shared<Rectangle>(GLOBAL_CONSTANTS::HOR_MARGIN, halfCrtVrtWall);
	RigidBody leftWallBody = RigidBody(vertRect2, Static);

	std::shared_ptr<Entity> leftWallUp = std::make_shared<PainWall>(
		Vector2f((GLOBAL_CONSTANTS::HOR_MARGIN / 2.0f), GLOBAL_CONSTANTS::VERT_MARGIN - (DOOR_WIDTH / 2.0f) + (GLOBAL_CONSTANTS::COURT_HEIGHT * (1.0f / 4.0f))), this, leftWallBody);
	AddEntPtrToSector(leftWallUp);
	std::shared_ptr<Entity> leftWallDown = std::make_shared<PainWall>(
		Vector2f((GLOBAL_CONSTANTS::HOR_MARGIN / 2.0f), GLOBAL_CONSTANTS::VERT_MARGIN + (DOOR_WIDTH / 2.0f) + (GLOBAL_CONSTANTS::COURT_HEIGHT * (3.0f / 4.0f))), this, leftWallBody);
	AddEntPtrToSector(leftWallDown);

	std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(halfCrtHorWall, GLOBAL_CONSTANTS::VERT_MARGIN);
	RigidBody wallBody1 = RigidBody(horRect1, Static);

	std::shared_ptr<Entity> upperWallLeft = std::make_shared<PainWall>(
		Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN - (DOOR_WIDTH / 2.0f) + (GLOBAL_CONSTANTS::COURT_WIDTH * (1.0f / 4.0f)), (GLOBAL_CONSTANTS::VERT_MARGIN / 2.0f)), this, wallBody1);
	lvlEntitiesPhase1.push_back(upperWallLeft);
	std::shared_ptr<Entity> upperWallRight = std::make_shared<PainWall>(
		Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN + (DOOR_WIDTH / 2.0f) + (GLOBAL_CONSTANTS::COURT_WIDTH * (3.0f / 4.0f)), (GLOBAL_CONSTANTS::VERT_MARGIN / 2.0f)), this, wallBody1);
	lvlEntitiesPhase1.push_back(upperWallRight);


	std::shared_ptr<Shape> horRect2 = std::make_shared<Rectangle>(halfCrtHorWall, GLOBAL_CONSTANTS::VERT_MARGIN);
	RigidBody lowerWallBody = RigidBody(horRect2, Static);

	std::shared_ptr<Entity> lowerWallLeft = std::make_shared<PainWall>(
		Vector2f(Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN - (DOOR_WIDTH / 2.0f) + (GLOBAL_CONSTANTS::COURT_WIDTH * (1.0f / 4.0f)), (GLOBAL_CONSTANTS::VERT_MARGIN / 2.0f) + GLOBAL_CONSTANTS::COURT_HEIGHT + GLOBAL_CONSTANTS::VERT_MARGIN)), this, lowerWallBody);
	lvlEntitiesPhase1.push_back(lowerWallLeft);
	std::shared_ptr<Entity> lowerWallRight = std::make_shared<PainWall>(
		Vector2f(Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN + (DOOR_WIDTH / 2.0f) + (GLOBAL_CONSTANTS::COURT_WIDTH * (3.0f / 4.0f)), (GLOBAL_CONSTANTS::VERT_MARGIN / 2.0f) + GLOBAL_CONSTANTS::COURT_HEIGHT + GLOBAL_CONSTANTS::VERT_MARGIN)), this, lowerWallBody);
	lvlEntitiesPhase1.push_back(lowerWallRight);

}

void Sector::AddRandomPainWall(int i_index)
{
	Material Static = Material(0.0f, 0.4f, 0.4f, 0.2f);

	float halfCrtVrtWall = GLOBAL_CONSTANTS::COURT_HEIGHT * (1.0f / 2.0f) - DOOR_WIDTH;
	float halfCrtHorWall = GLOBAL_CONSTANTS::COURT_WIDTH * (1.0f / 2.0f) - DOOR_WIDTH;

	switch (i_index) {
	case (0): {
		std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(GLOBAL_CONSTANTS::HOR_MARGIN, halfCrtVrtWall);
		RigidBody rightWallBody = RigidBody(vertRect1, Static);
		std::shared_ptr<Entity> rightWallUp = std::make_shared<PainWall>(
			Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN + (GLOBAL_CONSTANTS::COURT_WIDTH)+(GLOBAL_CONSTANTS::HOR_MARGIN / 2.0f), GLOBAL_CONSTANTS::VERT_MARGIN - (DOOR_WIDTH / 2.0f) + (GLOBAL_CONSTANTS::COURT_HEIGHT * (1.0f / 4.0f))), this, rightWallBody);
		lvlEntitiesPhase1.push_back(rightWallUp);
		break;
	}
	case (1): {
		std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(GLOBAL_CONSTANTS::HOR_MARGIN, halfCrtVrtWall);
		RigidBody rightWallBody = RigidBody(vertRect1, Static);
		std::shared_ptr<Entity> rightWallDown = std::make_shared<PainWall>(
			Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN + (GLOBAL_CONSTANTS::COURT_WIDTH)+(GLOBAL_CONSTANTS::HOR_MARGIN / 2.0f), GLOBAL_CONSTANTS::VERT_MARGIN + (DOOR_WIDTH / 2.0f) + (GLOBAL_CONSTANTS::COURT_HEIGHT * (3.0f / 4.0f))), this, rightWallBody);
		lvlEntitiesPhase1.push_back(rightWallDown);
		break;
	}
	case (2): {
		std::shared_ptr<Shape> vertRect2 = std::make_shared<Rectangle>(GLOBAL_CONSTANTS::HOR_MARGIN, halfCrtVrtWall);
		RigidBody leftWallBody = RigidBody(vertRect2, Static);
		std::shared_ptr<Entity> leftWallUp = std::make_shared<PainWall>(
			Vector2f((GLOBAL_CONSTANTS::HOR_MARGIN / 2.0f), GLOBAL_CONSTANTS::VERT_MARGIN - (DOOR_WIDTH / 2.0f) + (GLOBAL_CONSTANTS::COURT_HEIGHT * (1.0f / 4.0f))), this, leftWallBody);
		AddEntPtrToSector(leftWallUp);
		break;
	}
	case (3): {
		std::shared_ptr<Shape> vertRect2 = std::make_shared<Rectangle>(GLOBAL_CONSTANTS::HOR_MARGIN, halfCrtVrtWall);
		RigidBody leftWallBody = RigidBody(vertRect2, Static);
		std::shared_ptr<Entity> leftWallDown = std::make_shared<PainWall>(
			Vector2f((GLOBAL_CONSTANTS::HOR_MARGIN / 2.0f), GLOBAL_CONSTANTS::VERT_MARGIN + (DOOR_WIDTH / 2.0f) + (GLOBAL_CONSTANTS::COURT_HEIGHT * (3.0f / 4.0f))), this, leftWallBody);
		AddEntPtrToSector(leftWallDown);
		break;
	}
	case (4): {
		std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(halfCrtHorWall, GLOBAL_CONSTANTS::VERT_MARGIN);
		RigidBody wallBody1 = RigidBody(horRect1, Static);
		std::shared_ptr<Entity> upperWallLeft = std::make_shared<PainWall>(
			Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN - (DOOR_WIDTH / 2.0f) + (GLOBAL_CONSTANTS::COURT_WIDTH * (1.0f / 4.0f)), (GLOBAL_CONSTANTS::VERT_MARGIN / 2.0f)), this, wallBody1);
		lvlEntitiesPhase1.push_back(upperWallLeft);
		break;
	}
	case (5): {
		std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(halfCrtHorWall, GLOBAL_CONSTANTS::VERT_MARGIN);
		RigidBody wallBody1 = RigidBody(horRect1, Static);
		std::shared_ptr<Entity> upperWallRight = std::make_shared<PainWall>(
			Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN + (DOOR_WIDTH / 2.0f) + (GLOBAL_CONSTANTS::COURT_WIDTH * (3.0f / 4.0f)), (GLOBAL_CONSTANTS::VERT_MARGIN / 2.0f)), this, wallBody1);
		lvlEntitiesPhase1.push_back(upperWallRight);
		break;
	}
	case (6): {
		std::shared_ptr<Shape> horRect2 = std::make_shared<Rectangle>(halfCrtHorWall, GLOBAL_CONSTANTS::VERT_MARGIN);
		RigidBody lowerWallBody = RigidBody(horRect2, Static);
		std::shared_ptr<Entity> lowerWallLeft = std::make_shared<PainWall>(
			Vector2f(Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN - (DOOR_WIDTH / 2.0f) + (GLOBAL_CONSTANTS::COURT_WIDTH * (1.0f / 4.0f)), (GLOBAL_CONSTANTS::VERT_MARGIN / 2.0f) + GLOBAL_CONSTANTS::COURT_HEIGHT + GLOBAL_CONSTANTS::VERT_MARGIN)), this, lowerWallBody);
		lvlEntitiesPhase1.push_back(lowerWallLeft);
		break;
	}
	case (7): {
		std::shared_ptr<Shape> horRect2 = std::make_shared<Rectangle>(halfCrtHorWall, GLOBAL_CONSTANTS::VERT_MARGIN);
		RigidBody lowerWallBody = RigidBody(horRect2, Static);
		std::shared_ptr<Entity> lowerWallRight = std::make_shared<PainWall>(
			Vector2f(Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN + (DOOR_WIDTH / 2.0f) + (GLOBAL_CONSTANTS::COURT_WIDTH * (3.0f / 4.0f)), (GLOBAL_CONSTANTS::VERT_MARGIN / 2.0f) + GLOBAL_CONSTANTS::COURT_HEIGHT + GLOBAL_CONSTANTS::VERT_MARGIN)), this, lowerWallBody);
		lvlEntitiesPhase1.push_back(lowerWallRight);
		break;
	}
	}
}

void Sector::PopulateEntranceRoom()
{
	std::shared_ptr<Entity> lowerWall = std::make_shared<EndObject>(this,
				Vector2f(Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN + (GLOBAL_CONSTANTS::COURT_WIDTH / 2.0f), GLOBAL_CONSTANTS::COURT_HEIGHT / 2.0)));
	lvlEntitiesPhase1.push_back(lowerWall);
}

void Sector::PopulateBossRoom()
{
	isBossRoom = true;

	std::shared_ptr<Entity> lowerWall2 = std::make_shared<PowerUp>(this,
		Vector2f(Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN + (GLOBAL_CONSTANTS::COURT_WIDTH / 4.0f), GLOBAL_CONSTANTS::COURT_HEIGHT / 2.0)), 0);
	lvlEntitiesPhase1.push_back(lowerWall2);

	std::shared_ptr<Entity> lowerWall3 = std::make_shared<PowerUp>(this,
		Vector2f(Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN + (GLOBAL_CONSTANTS::COURT_WIDTH * (3.0f / 4.0f)), GLOBAL_CONSTANTS::COURT_HEIGHT / 2.0)), 1);
	lvlEntitiesPhase1.push_back(lowerWall3);
}

void Sector::RemoveDestroyedEntities() {
	std::list<std::shared_ptr<Entity>>::iterator iter = lvlEntitiesPhase1.begin();
	while (iter != lvlEntitiesPhase1.end()) {
		if (iter._Ptr->_Myval->MarkedForDeath()) {
			int entType = iter._Ptr->_Myval->GetTypeID();
			if ((entType == ENEMY_SEEK) || (entType == ENEMY_RAND) || (entType == ENEMY_SEEK) || 
				(entType == ENEMY_SEEK_PUSH) || (entType == ENEMY_RAND_PUSH) || (entType == ENEMY_BOSS)) {
				--sectEnemyNum;
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
	Material Static = Material(0.0f, 0.4f, 0.4f, 0.2f);

	switch (i_terrainType) {
		//small walls
	case 0: {
		std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(GLOBAL_CONSTANTS::HOR_MARGIN / 8.0f, GLOBAL_CONSTANTS::COURT_HEIGHT / 4.0f);
		RigidBody rightWallBody = RigidBody(vertRect1, Static);
		std::shared_ptr<Entity> rightWall = std::make_shared<Wall>(
			Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN + (GLOBAL_CONSTANTS::COURT_WIDTH * ( 1.0f / 5.0f)), GLOBAL_CONSTANTS::VERT_MARGIN + (GLOBAL_CONSTANTS::COURT_HEIGHT / 2.0f)), this, rightWallBody);
		lvlEntitiesPhase1.push_back(rightWall);
		break;
	}
	case 1: {
		std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(GLOBAL_CONSTANTS::HOR_MARGIN / 8.0f, GLOBAL_CONSTANTS::COURT_HEIGHT / 4.0f);
		RigidBody rightWallBody = RigidBody(vertRect1, Static);
		std::shared_ptr<Entity> rightWall = std::make_shared<Wall>(
			Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN + (GLOBAL_CONSTANTS::COURT_WIDTH *  (4.0f / 5.0f)), GLOBAL_CONSTANTS::VERT_MARGIN + (GLOBAL_CONSTANTS::COURT_HEIGHT / 2.0f)), this, rightWallBody);
		lvlEntitiesPhase1.push_back(rightWall);
		break;
	}
	case 2: {
		std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(GLOBAL_CONSTANTS::COURT_WIDTH / 4.0f, GLOBAL_CONSTANTS::VERT_MARGIN / 8.0f);
		RigidBody wallBody1 = RigidBody(horRect1, Static);
		std::shared_ptr<Entity> upperWall = std::make_shared<Wall>(
			Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN + (GLOBAL_CONSTANTS::COURT_WIDTH / 2.0f), GLOBAL_CONSTANTS::VERT_MARGIN + (GLOBAL_CONSTANTS::COURT_HEIGHT * (1.0f / 5.0f))), this, wallBody1);
		lvlEntitiesPhase1.push_back(upperWall);
		break;
	}
	case 3: {
		std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(GLOBAL_CONSTANTS::COURT_WIDTH / 4.0f, GLOBAL_CONSTANTS::VERT_MARGIN / 8.0f);
		RigidBody wallBody1 = RigidBody(horRect1, Static);
		std::shared_ptr<Entity> upperWall = std::make_shared<Wall>(
			Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN + (GLOBAL_CONSTANTS::COURT_WIDTH / 2.0f), GLOBAL_CONSTANTS::VERT_MARGIN + (GLOBAL_CONSTANTS::COURT_HEIGHT * (4.0f / 5.0f))), this, wallBody1);
		lvlEntitiesPhase1.push_back(upperWall);
		break;
	}
	//big walls
	case 4: {
		std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(GLOBAL_CONSTANTS::HOR_MARGIN / 4.0f, GLOBAL_CONSTANTS::COURT_HEIGHT / 2.0f);
		RigidBody rightWallBody = RigidBody(vertRect1, Static);
		std::shared_ptr<Entity> rightWall = std::make_shared<Wall>(
			Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN + (GLOBAL_CONSTANTS::COURT_WIDTH * (1.0f / 5.0f)), GLOBAL_CONSTANTS::VERT_MARGIN + (GLOBAL_CONSTANTS::COURT_HEIGHT / 2.0f)), this, rightWallBody);
		lvlEntitiesPhase1.push_back(rightWall);
		break;
	}
	case 5: {
		std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(GLOBAL_CONSTANTS::HOR_MARGIN / 4.0f, GLOBAL_CONSTANTS::COURT_HEIGHT / 2.0f);
		RigidBody rightWallBody = RigidBody(vertRect1, Static);
		std::shared_ptr<Entity> rightWall = std::make_shared<Wall>(
			Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN + (GLOBAL_CONSTANTS::COURT_WIDTH *  (4.0f / 5.0f)), GLOBAL_CONSTANTS::VERT_MARGIN + (GLOBAL_CONSTANTS::COURT_HEIGHT / 2.0f)), this, rightWallBody);
		lvlEntitiesPhase1.push_back(rightWall);
		break;
	}
	case 6: {
		std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(GLOBAL_CONSTANTS::COURT_WIDTH / 2.0f, GLOBAL_CONSTANTS::VERT_MARGIN / 4.0f);
		RigidBody wallBody1 = RigidBody(horRect1, Static);
		std::shared_ptr<Entity> upperWall = std::make_shared<Wall>(
			Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN + (GLOBAL_CONSTANTS::COURT_WIDTH / 2.0f), GLOBAL_CONSTANTS::VERT_MARGIN + (GLOBAL_CONSTANTS::COURT_HEIGHT * (1.0f / 5.0f))), this, wallBody1);
		lvlEntitiesPhase1.push_back(upperWall);
		break;
	}
	case 7: {
		std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(GLOBAL_CONSTANTS::COURT_WIDTH / 2.0f, GLOBAL_CONSTANTS::VERT_MARGIN / 4.0f);
		RigidBody wallBody1 = RigidBody(horRect1, Static);
		std::shared_ptr<Entity> upperWall = std::make_shared<Wall>(
			Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN + (GLOBAL_CONSTANTS::COURT_WIDTH / 2.0f), GLOBAL_CONSTANTS::VERT_MARGIN + (GLOBAL_CONSTANTS::COURT_HEIGHT * (4.0f / 5.0f))), this, wallBody1);
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