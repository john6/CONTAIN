#include "Sector.h"

Sector::Sector(Level* i_lvlPtr, RESOURCES* i_resources, sf::Color i_colA, sf::Color i_colB, bool i_testSector) :
	myLevel { i_lvlPtr }, resources { i_resources }, colPalA { i_colA }, colPalB { i_colB }
{
	colPalA = Physics::GenerateRandomColor(5, 120);
	colPalB = Physics::GenerateRandomColor(5, 50);

	if (i_testSector) {
		emptyTerrainAreas = { TER_UP, TER_RIGHT, TER_DOWN, TER_LEFT };
		sectEnemyNum = 0;
		numBlockers = 0;
		isBossRoom = false;
		firstPhase = true;
		filledIn = false;
	}
	else {
		InitializeSector();
	}
}

Sector::~Sector()
{
}

std::list<std::shared_ptr<Entity>>* Sector::GetSectorEntities()
{ //TODO wtf is this shit
	/*if (firstsPhase) {*/
		return &lvlEntitiesPhase1;
	/*}
	else {
		return &lvlEntitiesPhase2;
	}*/
}

void Sector::AddEntPtrToSector(std::shared_ptr<Entity> i_entPtr)
{
	if (auto enemy = dynamic_cast<Enemy*>(i_entPtr.get())) {
		sectEnemyNum++;
	}
	else if (auto wall = dynamic_cast<Blocker*>(i_entPtr.get())) {
		numBlockers++;
		if (numBlockers > MAX_NUM_BLOCKERS) {
			//auto wallPtr = std::find_if(lvlEntitiesPhase1.begin(), lvlEntitiesPhase1.end(), IsBlocker);
			for (std::shared_ptr<Entity> iterPtr : lvlEntitiesPhase1) {
				if ((!iterPtr->MarkedForDeath()) && (iterPtr->GetTypeID() == PROJ_WALL) && (numBlockers > MAX_NUM_BLOCKERS)) {
					iterPtr->Destroy();
					numBlockers--;
				}
			}
		}
	}
	else if (auto doorPtr = std::dynamic_pointer_cast<Door>(i_entPtr)) {
		sectDoors.push_back(doorPtr);
	}
	else if (auto endPtr = std::dynamic_pointer_cast<EndObject>(i_entPtr)) {
		sectEndObj = endPtr;
	}
	lvlEntitiesPhase1.push_back(i_entPtr);
}

void Sector::AddEntsFromSpawnQueues()
{//TODO Sector needs player pointer so its doesnt have to cast all the time I think
	auto pPtr = GLBVRS::PPTR;
	for (auto spawnable : pPtr->spawnVect) {
		AddEntPtrToSector(spawnable);
	}
	for (auto entPtr : lvlEntitiesPhase1) {
		for (auto spawnable : entPtr->spawnVect) {
			AddEntPtrToSector(spawnable);
		}
	}
	
	pPtr->spawnVect.clear();

	for (auto entPtr : lvlEntitiesPhase1) {
		entPtr->spawnVect.clear();
	}
}

bool Sector::IsBlocker(std::shared_ptr<Entity> i_entPtr)
{
	if (i_entPtr->GetTypeID() == PROJ_WALL) { return true; }
	return false;
}

void Sector::GenerateEnemies(int i_numEnems, TypeID enemyType, SCREEN_AREA i_area, int i_phaseNum, DIFFICULTY i_diff, int i_sizeMod)
{
	auto screenAreas = GetScreenAreas(i_area);
	for (int i = 0; i < i_numEnems; ++i) {

		std::random_device sizeSeed;
		std::mt19937 genRoomSeed(sizeSeed());
		std::discrete_distribution<> widthModDist({ 10, 15, 50, 15, 10 });
		std::discrete_distribution<> radiusModDist({ 0, 15, 55, 20, 10 });
		std::discrete_distribution<> heightModDist({ 13, 18, 40, 18, 13 });
		int rand1 = widthModDist(genRoomSeed);
		int ranDifwidth = (rand1 - 2 + i_sizeMod) * 10;
		int rand2 = heightModDist(genRoomSeed);
		int randDifHeight = (rand2 - 2 + i_sizeMod) * 10;
		int randRad = radiusModDist(genRoomSeed);
		int randDifRadius = (randRad - 2 + i_sizeMod) * 10;

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
		case ENEMY_SEEK: {
			std::shared_ptr<Shape> shape = std::make_shared<Rectangle>((65 + ranDifwidth), (65 + randDifHeight));
			//std::shared_ptr<Shape> shape = std::make_shared<Rectangle>(60 + randSizeDiff, 60 + randSizeDiff2);
			Material Rock = Material(0.6f, 0.1f, 0.6f, 0.3f);
			RigidBody projBody = RigidBody(shape, Rock);
			ent = std::make_shared<Enemy>(i_diff, spawnPos, projBody);
			break;
		}
		case ENEMY_RAND:{
			std::shared_ptr<Shape> shape = std::make_shared<Circle>((40 + randDifRadius));
			RigidBody projBody = RigidBody(shape, ROCK);
			ent = std::make_shared<CrazyBoi>(i_diff, spawnPos, projBody);
			break;
		}
		case ENEMY_SEEK_PUSH: {
			std::shared_ptr<Shape> shape = std::make_shared<Rectangle>((65 + ranDifwidth), (65 + randDifHeight));
			//std::shared_ptr<Shape> shape = std::make_shared<Rectangle>(60 + randSizeDiff, 60 + randSizeDiff2);
			Material Rock = Material(0.6f, 0.1f, 0.6f, 0.3f);
			RigidBody projBody = RigidBody(shape, Rock);
			ent = std::make_shared<Enemy>(i_diff, spawnPos, projBody);
			break;
		}
		case ENEMY_RAND_PUSH:
			std::shared_ptr<Shape> shape = std::make_shared<Circle>((40 + randDifRadius));
			RigidBody projBody = RigidBody(shape, ROCK);
			ent = std::make_shared<CrazyBoi>(i_diff, spawnPos, projBody);
			break;
		}
		if (i_phaseNum == 1) {
			AddEntPtrToSector(ent);
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
				GLBVRS::VRT_MRG + (GLBVRS::CRT_HGHT / 2.0f)), rightWallBody, colPalA, colPalB);
	lvlEntitiesPhase1.push_back(rightWall);
	//Left wall
	std::shared_ptr<Shape> vertRect2 = std::make_shared<Rectangle>(GLBVRS::HR_MRG * wall_thick, GLBVRS::CRT_HGHT * 2.0f);
	RigidBody leftWallBody = RigidBody(vertRect2, Static);
	std::shared_ptr<Entity> leftWall = std::make_shared<Wall>(
		Vector2f((GLBVRS::HR_MRG / 2.0f) - (GLBVRS::HR_MRG * 2.0),
			GLBVRS::VRT_MRG + (GLBVRS::CRT_HGHT / 2.0f)), leftWallBody, colPalA, colPalB);
	AddEntPtrToSector(leftWall);
	//Top wall
	std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(GLBVRS::CRT_WDTH + (GLBVRS::HR_MRG * 8), GLBVRS::VRT_MRG * wall_thick);
	RigidBody wallBody1 = RigidBody(horRect1, Static);
	std::shared_ptr<Entity> upperWall = std::make_shared<Wall>(
		Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH / 2.0f), 
				- (GLBVRS::VRT_MRG * 1.5f)), wallBody1, colPalA, colPalB);
	lvlEntitiesPhase1.push_back(upperWall);

	//Bottom wall
	std::shared_ptr<Shape> horRect2 = std::make_shared<Rectangle>(GLBVRS::CRT_WDTH + (GLBVRS::HR_MRG * 8), GLBVRS::VRT_MRG * wall_thick);
	RigidBody lowerWallBody = RigidBody(horRect2, Static);
	std::shared_ptr<Entity> lowerWall = std::make_shared<Wall>(
		Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH / 2.0f),
			(GLBVRS::VRT_MRG * 2.5f) + GLBVRS::CRT_HGHT + GLBVRS::VRT_MRG)), lowerWallBody, colPalA, colPalB);
	lvlEntitiesPhase1.push_back(lowerWall);

}

void Sector::AddPainfullWallsToLevel()
{
	Material Static = Material(0.0f, 0.4f, 0.4f, 0.2f);

	float halfCrtVrtWall = GLBVRS::CRT_HGHT * (1.0f / 2.0f) - GLBVRS::DOOR_WIDTH;
	float halfCrtHorWall = GLBVRS::CRT_WDTH * (1.0f / 2.0f) - GLBVRS::DOOR_WIDTH;


	std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(GLBVRS::HR_MRG, halfCrtVrtWall);
	RigidBody rightWallBody = RigidBody(vertRect1, Static);

	std::shared_ptr<Entity> rightWallUp = std::make_shared<PainWall>(
		Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH)+(GLBVRS::HR_MRG / 2.0f),
			GLBVRS::VRT_MRG - (GLBVRS::DOOR_WIDTH / 2.0f ) + (GLBVRS::CRT_HGHT * ( 1.0f / 4.0f))), rightWallBody);
	lvlEntitiesPhase1.push_back(rightWallUp);
	std::shared_ptr<Entity> rightWallDown = std::make_shared<PainWall>(
		Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH)+(GLBVRS::HR_MRG / 2.0f),
			GLBVRS::VRT_MRG + (GLBVRS::DOOR_WIDTH / 2.0f) + (GLBVRS::CRT_HGHT * (3.0f / 4.0f))), rightWallBody);
	lvlEntitiesPhase1.push_back(rightWallDown);

	std::shared_ptr<Shape> vertRect2 = std::make_shared<Rectangle>(GLBVRS::HR_MRG, halfCrtVrtWall);
	RigidBody leftWallBody = RigidBody(vertRect2, Static);

	std::shared_ptr<Entity> leftWallUp = std::make_shared<PainWall>(
		Vector2f((GLBVRS::HR_MRG / 2.0f),
			GLBVRS::VRT_MRG - (GLBVRS::DOOR_WIDTH / 2.0f) + (GLBVRS::CRT_HGHT * (1.0f / 4.0f))), leftWallBody);
	AddEntPtrToSector(leftWallUp);
	std::shared_ptr<Entity> leftWallDown = std::make_shared<PainWall>(
		Vector2f((GLBVRS::HR_MRG / 2.0f),
			GLBVRS::VRT_MRG + (GLBVRS::DOOR_WIDTH / 2.0f) + (GLBVRS::CRT_HGHT * (3.0f / 4.0f))), leftWallBody);
	AddEntPtrToSector(leftWallDown);

	std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(halfCrtHorWall, GLBVRS::VRT_MRG);
	RigidBody wallBody1 = RigidBody(horRect1, Static);

	std::shared_ptr<Entity> upperWallLeft = std::make_shared<PainWall>(
		Vector2f(GLBVRS::HR_MRG - (GLBVRS::DOOR_WIDTH / 2.0f) + (GLBVRS::CRT_WDTH * (1.0f / 4.0f)),
		(GLBVRS::VRT_MRG / 2.0f)), wallBody1);
	lvlEntitiesPhase1.push_back(upperWallLeft);
	std::shared_ptr<Entity> upperWallRight = std::make_shared<PainWall>(
		Vector2f(GLBVRS::HR_MRG + (GLBVRS::DOOR_WIDTH / 2.0f) + (GLBVRS::CRT_WDTH * (3.0f / 4.0f)),
		(GLBVRS::VRT_MRG / 2.0f)), wallBody1);
	lvlEntitiesPhase1.push_back(upperWallRight);


	std::shared_ptr<Shape> horRect2 = std::make_shared<Rectangle>(halfCrtHorWall, GLBVRS::VRT_MRG);
	RigidBody lowerWallBody = RigidBody(horRect2, Static);

	std::shared_ptr<Entity> lowerWallLeft = std::make_shared<PainWall>(
		Vector2f(Vector2f(GLBVRS::HR_MRG - (GLBVRS::DOOR_WIDTH / 2.0f) + (GLBVRS::CRT_WDTH * (1.0f / 4.0f)),
		(GLBVRS::VRT_MRG / 2.0f) + GLBVRS::CRT_HGHT + GLBVRS::VRT_MRG)), lowerWallBody);
	lvlEntitiesPhase1.push_back(lowerWallLeft);
	std::shared_ptr<Entity> lowerWallRight = std::make_shared<PainWall>(
		Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::DOOR_WIDTH / 2.0f) + (GLBVRS::CRT_WDTH * (3.0f / 4.0f)),
		(GLBVRS::VRT_MRG / 2.0f) + GLBVRS::CRT_HGHT + GLBVRS::VRT_MRG)), lowerWallBody);
	lvlEntitiesPhase1.push_back(lowerWallRight);

}

void Sector::AddRandomPainWall(int i_index)
{
	Material Static = Material(0.0f, 0.4f, 0.4f, 0.2f);

	float halfCrtVrtWall = GLBVRS::CRT_HGHT * (1.0f / 2.0f) - GLBVRS::DOOR_WIDTH;
	float halfCrtHorWall = GLBVRS::CRT_WDTH * (1.0f / 2.0f) - GLBVRS::DOOR_WIDTH;

	switch (i_index) {
	case (0): {
		std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(GLBVRS::HR_MRG, halfCrtVrtWall);
		RigidBody rightWallBody = RigidBody(vertRect1, Static);
		std::shared_ptr<Entity> rightWallUp = std::make_shared<PainWall>(
			Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH)+(GLBVRS::HR_MRG / 2.0f), GLBVRS::VRT_MRG - (GLBVRS::DOOR_WIDTH / 2.0f) + (GLBVRS::CRT_HGHT * (1.0f / 4.0f))), rightWallBody);
		lvlEntitiesPhase1.push_back(rightWallUp);
		break;
	}
	case (1): {
		std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(GLBVRS::HR_MRG, halfCrtVrtWall);
		RigidBody rightWallBody = RigidBody(vertRect1, Static);
		std::shared_ptr<Entity> rightWallDown = std::make_shared<PainWall>(
			Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH)+(GLBVRS::HR_MRG / 2.0f), GLBVRS::VRT_MRG + (GLBVRS::DOOR_WIDTH / 2.0f) + (GLBVRS::CRT_HGHT * (3.0f / 4.0f))), rightWallBody);
		lvlEntitiesPhase1.push_back(rightWallDown);
		break;
	}
	case (2): {
		std::shared_ptr<Shape> vertRect2 = std::make_shared<Rectangle>(GLBVRS::HR_MRG, halfCrtVrtWall);
		RigidBody leftWallBody = RigidBody(vertRect2, Static);
		std::shared_ptr<Entity> leftWallUp = std::make_shared<PainWall>(
			Vector2f((GLBVRS::HR_MRG / 2.0f), GLBVRS::VRT_MRG - (GLBVRS::DOOR_WIDTH / 2.0f) + (GLBVRS::CRT_HGHT * (1.0f / 4.0f))), leftWallBody);
		AddEntPtrToSector(leftWallUp);
		break;
	}
	case (3): {
		std::shared_ptr<Shape> vertRect2 = std::make_shared<Rectangle>(GLBVRS::HR_MRG, halfCrtVrtWall);
		RigidBody leftWallBody = RigidBody(vertRect2, Static);
		std::shared_ptr<Entity> leftWallDown = std::make_shared<PainWall>(
			Vector2f((GLBVRS::HR_MRG / 2.0f), GLBVRS::VRT_MRG + (GLBVRS::DOOR_WIDTH / 2.0f) + (GLBVRS::CRT_HGHT * (3.0f / 4.0f))), leftWallBody);
		AddEntPtrToSector(leftWallDown);
		break;
	}
	case (4): {
		std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(halfCrtHorWall, GLBVRS::VRT_MRG);
		RigidBody wallBody1 = RigidBody(horRect1, Static);
		std::shared_ptr<Entity> upperWallLeft = std::make_shared<PainWall>(
			Vector2f(GLBVRS::HR_MRG - (GLBVRS::DOOR_WIDTH / 2.0f) + (GLBVRS::CRT_WDTH * (1.0f / 4.0f)), (GLBVRS::VRT_MRG / 2.0f)), wallBody1);
		lvlEntitiesPhase1.push_back(upperWallLeft);
		break;
	}
	case (5): {
		std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(halfCrtHorWall, GLBVRS::VRT_MRG);
		RigidBody wallBody1 = RigidBody(horRect1, Static);
		std::shared_ptr<Entity> upperWallRight = std::make_shared<PainWall>(
			Vector2f(GLBVRS::HR_MRG + (GLBVRS::DOOR_WIDTH / 2.0f) + (GLBVRS::CRT_WDTH * (3.0f / 4.0f)), (GLBVRS::VRT_MRG / 2.0f)), wallBody1);
		lvlEntitiesPhase1.push_back(upperWallRight);
		break;
	}
	case (6): {
		std::shared_ptr<Shape> horRect2 = std::make_shared<Rectangle>(halfCrtHorWall, GLBVRS::VRT_MRG);
		RigidBody lowerWallBody = RigidBody(horRect2, Static);
		std::shared_ptr<Entity> lowerWallLeft = std::make_shared<PainWall>(
			Vector2f(Vector2f(GLBVRS::HR_MRG - (GLBVRS::DOOR_WIDTH / 2.0f) + (GLBVRS::CRT_WDTH * (1.0f / 4.0f)), (GLBVRS::VRT_MRG / 2.0f) + GLBVRS::CRT_HGHT + GLBVRS::VRT_MRG)), lowerWallBody);
		lvlEntitiesPhase1.push_back(lowerWallLeft);
		break;
	}
	case (7): {
		std::shared_ptr<Shape> horRect2 = std::make_shared<Rectangle>(halfCrtHorWall, GLBVRS::VRT_MRG);
		RigidBody lowerWallBody = RigidBody(horRect2, Static);
		std::shared_ptr<Entity> lowerWallRight = std::make_shared<PainWall>(
			Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::DOOR_WIDTH / 2.0f) + (GLBVRS::CRT_WDTH * (3.0f / 4.0f)), (GLBVRS::VRT_MRG / 2.0f) + GLBVRS::CRT_HGHT + GLBVRS::VRT_MRG)), lowerWallBody);
		lvlEntitiesPhase1.push_back(lowerWallRight);
		break;
	}
	}
}

void Sector::PopulateEntranceRoom()
{
	std::shared_ptr<Entity> lowerWall = std::make_shared<EndObject>(
				Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH / 2.0f), GLBVRS::CRT_HGHT / 2.0)));
	AddEntPtrToSector(lowerWall);

	//std::shared_ptr<Entity> smallShipPOW2 = std::make_shared<PowerUp>(this,
	//	Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH * (1.0f / 5.0f)), GLBVRS::CRT_HGHT / 2.0)), BIG_SHIP);
	//lvlEntitiesPhase1.push_back(smallShipPOW2);

	//std::shared_ptr<Entity> smallShipPOW3 = std::make_shared<PowerUp>(this,
	//	Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH * (2.0f / 5.0f)), GLBVRS::CRT_HGHT / 2.0)), SMALL_SHIP);
	//lvlEntitiesPhase1.push_back(smallShipPOW3);

	//std::shared_ptr<Entity> smallShipPOW4 = std::make_shared<PowerUp>(this,
	//	Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH * (3.0f / 5.0f)), GLBVRS::CRT_HGHT / 2.0)), BIG_SHIP);
	//lvlEntitiesPhase1.push_back(smallShipPOW4);

	std::shared_ptr<Entity> smallShipPOW5 = std::make_shared<PowerUp>(
		Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH * (4.0f / 5.0f)), GLBVRS::CRT_HGHT / 2.0)), SCATTER);
	lvlEntitiesPhase1.push_back(smallShipPOW5);

	std::shared_ptr<Entity> asd1 = std::make_shared<PowerUp>(
		Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH * (1.0f / 5.0f)), GLBVRS::CRT_HGHT / 4.0)), RATE_OF_FIRE);
	lvlEntitiesPhase1.push_back(asd1);


	//AddRandomPainWall(0);
	std::shared_ptr<Entity> asd2 = std::make_shared<PowerUp>(
		Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH * (2.0f / 5.0f)), GLBVRS::CRT_HGHT / 4.0)), SCATTER);
	lvlEntitiesPhase1.push_back(asd2);

	std::shared_ptr<Entity> asd3 = std::make_shared<PowerUp>(
		Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH * (3.0f / 5.0f)), GLBVRS::CRT_HGHT / 4.0)), SCATTER);
	lvlEntitiesPhase1.push_back(asd3);

	std::shared_ptr<Entity> asd4 = std::make_shared<PowerUp>(
		Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH * (4.0f / 5.0f)), GLBVRS::CRT_HGHT / 4.0)), RATE_OF_FIRE);
	lvlEntitiesPhase1.push_back(asd4);

	//std::vector<Vector2f> vector;
	//counterclockwise
	//vector.push_back(Vector2f(0.0, 0.0));
	//vector.push_back(Vector2f(-500.0, 0.0));
	//vector.push_back(Vector2f(-500.0, 50.0));
	//vector.push_back(Vector2f(0.0, 50.0));
	//std::shared_ptr<Shape> shape1 = std::make_shared<Polygon>(vector);
	//RigidBody rb1 = RigidBody(shape1, METAL);
	//std::shared_ptr<Entity> asd5 = std::make_shared<Wall>(
	//	Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH / 2.0f), GLBVRS::CRT_HGHT / 2.0)),
	//	this, rb1);
	//lvlEntitiesPhase1.push_back(asd5);

	//RigidBody rb1 = RigidBody(Physics::CreateIrregularPolygon(6, 400), BOUNCYBALL);
	//std::shared_ptr<Entity> asd5 = std::make_shared<Wall>(
	//	Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH / 4.0f), GLBVRS::CRT_HGHT / 4.0)),
	//	this, rb1);
	//lvlEntitiesPhase1.push_back(asd5);

	//RigidBody rb2 = RigidBody(Physics::CreateIrregularPolygon(4, 200), BOUNCYBALL);
	//std::shared_ptr<Entity> asd6 = std::make_shared<Wall>(
	//	Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH / 4.0f), GLBVRS::CRT_HGHT / 4.0)),
	//	this, rb2);
	//lvlEntitiesPhase1.push_back(asd6);

	//RigidBody rb3 = RigidBody(Physics::CreateIrregularPolygon(7, 400), BOUNCYBALL);
	//std::shared_ptr<Entity> asd7 = std::make_shared<Wall>(
	//	Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH / 4.0f), GLBVRS::CRT_HGHT / 4.0)),
	//	this, rb3);
	//lvlEntitiesPhase1.push_back(asd7);

	//RigidBody rb4 = RigidBody(Physics::CreateIrregularPolygon(6, 300), BOUNCYBALL);
	//std::shared_ptr<Entity> asd8 = std::make_shared<Wall>(
	//	Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH / 4.0f), GLBVRS::CRT_HGHT / 4.0)),
	//	this, rb4);
	//lvlEntitiesPhase1.push_back(asd8);

	//RigidBody rb5 = RigidBody(Physics::CreateIrregularPolygon(6, 400), BOUNCYBALL);
	//std::shared_ptr<Entity> asd9 = std::make_shared<Wall>(
	//	Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH / 4.0f), GLBVRS::CRT_HGHT / 4.0)),
	//	this, rb5);
	//lvlEntitiesPhase1.push_back(asd9);


	//RigidBody rb2 = RigidBody(Physics::CreateRegularPolygon(5, 100), METAL);
	//std::shared_ptr<Entity> asd6 = std::make_shared<Wall>(
	//	Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH * 3.0f / 4.0f), GLBVRS::CRT_HGHT / 4.0)),
	//	this, rb2);
	//lvlEntitiesPhase1.push_back(asd6);

	//RigidBody rb3 = RigidBody(Physics::CreateRegularPolygon(6, 100), METAL);
	//std::shared_ptr<Entity> asd7 = std::make_shared<Wall>(
	//	Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH * 1.0f / 4.0f), GLBVRS::CRT_HGHT *3.0f / 4.0)),
	//	this, rb3);
	//lvlEntitiesPhase1.push_back(asd7);

	//RigidBody rb4 = RigidBody(Physics::CreateRegularPolygon(7, 100), METAL);
	//std::shared_ptr<Entity> asd8 = std::make_shared<Wall>(
	//	Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH * 3.0f / 4.0f), GLBVRS::CRT_HGHT *3.0f / 4.0)),
	//	this, rb4);
	//lvlEntitiesPhase1.push_back(asd8);

}

void Sector::PopulateBossRoom(int i_lvlNum, DIFFICULTY i_diff, bool i_isMiniBoss)
{
	if (!i_isMiniBoss) {
		isBossRoom = true;
	}
	//Create random spawn position
	auto screenAreas = GetScreenAreas(CENTER);
	std::random_device rd;
	std::mt19937 gen1(rd());
	int leftX = (int)std::get<0>(screenAreas[0])[0];
	int rightX = (int)std::get<1>(screenAreas[0])[0];
	int topY = (int)std::get<0>(screenAreas[0])[1];
	int bottomY = (int)std::get<1>(screenAreas[0])[1];
	std::uniform_int_distribution<> xCordDist(leftX, rightX);
	std::uniform_int_distribution<> yCordDist(topY, bottomY);
	int randXcordInBounds = xCordDist(gen1);
	int randYcordInBounds = yCordDist(gen1);
	Vector2f spawnPos(randXcordInBounds, randYcordInBounds);
	std::shared_ptr<Entity> ent;
	//i_lvlNum = 2;
	switch (i_lvlNum) {
	case 0: {
		if (!i_isMiniBoss) {
			ent = std::make_shared<BossBurst>(i_diff, spawnPos);
		}
		else {
			ent = std::make_shared<BossBurst>(i_diff, spawnPos, true);
		}
		break;
	}
	case 1: {
		if (!i_isMiniBoss) {
			ent = std::make_shared<BossRush>(i_diff, spawnPos);
			AddPainfullWallsToLevel();
		}
		else {
			ent = std::make_shared<BossRush>(i_diff, spawnPos, true);
			//AddRandomPainWall()
		}
		break;
	}
	case 2: {
		if (!i_isMiniBoss) {
			ent = std::make_shared<BossStream>(i_diff, spawnPos);
		}
		else {
			ent = std::make_shared<BossStream>(i_diff, spawnPos, true);
		}
		break;
	}
	case 3: {
		if (!i_isMiniBoss) {
			//std::shared_ptr<Shape> shape1 = std::make_shared<Circle>(100);
			//RigidBody projBody1 = RigidBody(shape1, ROCK);
			ent = std::make_shared<BossSplit>(i_diff, 4, 6, false, spawnPos);
		}
		else {
			//std::shared_ptr<Shape> shape1 = std::make_shared<Circle>(30);
			//RigidBody projBody1 = RigidBody(shape1, ROCK);

			std::shared_ptr<Shape> shape1 = std::make_shared<Circle>(50);
			RigidBody projBody1 = RigidBody(shape1, LESSBOUNCYBALL);

			ent = std::make_shared<BossSplit>(i_diff, 1, 6, false, spawnPos, projBody1);
		}
		break;
	}
	case 4: {
		if (!i_isMiniBoss) {
			ent = std::make_shared<BossSpawn>(i_diff, spawnPos);
			AddTerrain(0, 1);
			AddTerrain(1, 0);
			AddTerrain(2, 1);
			AddTerrain(3, 0);
		}
		else {
			//remove all terrain from level TODO
			ent = std::make_shared<BossSpawn>(i_diff, spawnPos, true);
			AddTerrain(1, 0);
			AddTerrain(3, 0);
		}
		break;
	}
	}
	lvlEntitiesPhase1.push_back(ent);
	++sectEnemyNum;
}

void Sector::PopulateBossRoom(std::string i_tutorial)
{
	isBossRoom = true;
	Vector2f spawnPos(GLBVRS::CRT_WDTH, GLBVRS::CRT_HGHT);
	std::shared_ptr<Shape> shape = std::make_shared<Rectangle>(100, 100);
	Material Rock = Material(0.6f, 0.1f, 0.6f, 0.3f);
	RigidBody projBody = RigidBody(shape, Rock);
	std::shared_ptr<Entity> ent = std::make_shared<Enemy>((DIFFICULTY)0, spawnPos, projBody);
	//auto enemy = dynamic_cast<Enemy*>(ent.get());
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
				if ((sectEnemyNum <= 0) && (firstPhase)) {
					UnlockRoom();
				}
			}
			lvlEntitiesPhase1.erase(iter++);
			myLevel->enemiesKilled++;
		}
		else { ++iter; }
	}
	if 	((isBossRoom) && (sectEnemyNum <= 0) && (firstPhase)) {
		SwitchLevelToPhaseTwo();
	}
}

void Sector::AddTerrain(int i_terrainType, bool terrainBig)
{
	int terrType = i_terrainType;
	if (terrType == -1) {//random
		if (emptyTerrainAreas.size() == 0) { return; }
		std::random_device rd1;
		std::mt19937 gen1(rd1());
		std::uniform_int_distribution<> distrib(1, 3);
		terrType = emptyTerrainAreas[distrib(gen1) % emptyTerrainAreas.size()];
		auto iter1 = std::find(emptyTerrainAreas.begin(), emptyTerrainAreas.end(), terrType);
		emptyTerrainAreas.erase(iter1);
	}
	else {
		if (std::find(emptyTerrainAreas.begin(), emptyTerrainAreas.end(), terrType) == emptyTerrainAreas.end()) {
			return;
		}
	}
	if (terrainBig) {
		terrType += 4;
	}
	float smallWidth = GLBVRS::HR_MRG / 2.5f;
	float smallHeight = GLBVRS::CRT_HGHT / 4.0f;
	float bigWidth = GLBVRS::HR_MRG / 2.5f;
	float bigHeight = GLBVRS::CRT_HGHT / 1.5f;
	switch (terrType) {
		//small walls
	case 0: {
		std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(smallWidth, smallHeight);
		RigidBody rightWallBody = RigidBody(vertRect1, STATIC);
		std::shared_ptr<Entity> rightWall = std::make_shared<Wall>(
			Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH * ( 1.0f / 5.0f)), GLBVRS::VRT_MRG + (GLBVRS::CRT_HGHT / 2.0f)), rightWallBody, colPalA, colPalB, true);
		lvlEntitiesPhase1.push_back(rightWall);
		break;
	}
	case 1: {
		std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(smallWidth, GLBVRS::CRT_HGHT / 4.0f);
		RigidBody rightWallBody = RigidBody(vertRect1, STATIC);
		std::shared_ptr<Entity> rightWall = std::make_shared<Wall>(
			Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH *  (4.0f / 5.0f)), GLBVRS::VRT_MRG + (GLBVRS::CRT_HGHT / 2.0f)), rightWallBody, colPalA, colPalB, true);
		lvlEntitiesPhase1.push_back(rightWall);
		break;
	}
	case 2: {
		std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(GLBVRS::CRT_WDTH / 4.0f, smallWidth);
		RigidBody wallBody1 = RigidBody(horRect1, STATIC);
		std::shared_ptr<Entity> upperWall = std::make_shared<Wall>(
			Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH / 2.0f), GLBVRS::VRT_MRG + (GLBVRS::CRT_HGHT * (1.0f / 5.0f))), wallBody1, colPalA, colPalB, true);
		lvlEntitiesPhase1.push_back(upperWall);
		break;
	}
	case 3: {
		std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(GLBVRS::CRT_WDTH / 4.0f, smallWidth);
		RigidBody wallBody1 = RigidBody(horRect1, STATIC);
		std::shared_ptr<Entity> upperWall = std::make_shared<Wall>(
			Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH / 2.0f), GLBVRS::VRT_MRG + (GLBVRS::CRT_HGHT * (4.0f / 5.0f))), wallBody1, colPalA, colPalB, true);
		lvlEntitiesPhase1.push_back(upperWall);
		break;
	}
	//big walls
	case 4: {
		std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(bigWidth, bigHeight);
		RigidBody rightWallBody = RigidBody(vertRect1, STATIC);
		std::shared_ptr<Entity> rightWall = std::make_shared<Wall>(
			Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH * (1.0f / 5.0f)), GLBVRS::VRT_MRG + (GLBVRS::CRT_HGHT / 2.0f)), rightWallBody, colPalA, colPalB, true);
		lvlEntitiesPhase1.push_back(rightWall);
		break;
	}
	case 5: {
		std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(bigWidth, bigHeight);
		RigidBody rightWallBody = RigidBody(vertRect1, STATIC);
		std::shared_ptr<Entity> rightWall = std::make_shared<Wall>(
			Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH *  (4.0f / 5.0f)), GLBVRS::VRT_MRG + (GLBVRS::CRT_HGHT / 2.0f)), rightWallBody, colPalA, colPalB, true);
		lvlEntitiesPhase1.push_back(rightWall);
		break;
	}
	case 6: {
		std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(bigHeight, bigWidth);
		RigidBody wallBody1 = RigidBody(horRect1, STATIC);
		std::shared_ptr<Entity> upperWall = std::make_shared<Wall>(
			Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH / 2.0f), GLBVRS::VRT_MRG + (GLBVRS::CRT_HGHT * (1.0f / 5.0f))), wallBody1, colPalA, colPalB, true);
		lvlEntitiesPhase1.push_back(upperWall);
		break;
	}
	case 7: {
		std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(bigHeight, bigWidth);
		RigidBody wallBody1 = RigidBody(horRect1, STATIC);
		std::shared_ptr<Entity> upperWall = std::make_shared<Wall>(
			Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH / 2.0f), GLBVRS::VRT_MRG + (GLBVRS::CRT_HGHT * (4.0f / 5.0f))), wallBody1, colPalA, colPalB, true);
		lvlEntitiesPhase1.push_back(upperWall);
		break;
	}
	}
}

void Sector::InitializeSector()
{
	emptyTerrainAreas = { TER_UP, TER_RIGHT, TER_DOWN, TER_LEFT }; //, TER_CENT };
	sectEnemyNum = 0;
	numBlockers = 0;
	isBossRoom = false;
	firstPhase = true;
	filledIn = false;
	GenerateBackGround();
	AddWallsToLevel();
}

void Sector::PlaySound(int i_soundNum)
{
	resources->PlaySound(i_soundNum);
}

void Sector::SwitchToPhaseTwo()
{ //TODO the phase one enemies and phase two names are kind of meaningless. What really happens is that one phase 1 enemies ever appear, phase two is just a bucket of extra enemies to dump in
	firstPhase = false;
	std::list<std::shared_ptr<Entity>>::iterator iter = lvlEntitiesPhase1.begin();
	while (iter != lvlEntitiesPhase1.end()) {
		int entType = iter._Ptr->_Myval->GetTypeID();
		if ((entType == ENEMY_SEEK) || (entType == ENEMY_RAND) || (entType == ENEMY_SEEK) ||
			(entType == ENEMY_SEEK_PUSH) || (entType == ENEMY_RAND_PUSH) || (entType == ENEMY_BOSS)) {
			--sectEnemyNum;
			lvlEntitiesPhase1.erase(iter++);
		}
		else if (entType == WALL_FIRE) {
			lvlEntitiesPhase1.erase(iter++);
		}
		else if (entType == WALL_BASIC) {
			//lvlEntitiesPhase1.erase(iter++);
			auto wall = dynamic_cast<Wall*>((*iter).get());
			if (wall->breakable) {
				(*iter)->rb.mat = DENSE_METAL;
				(*iter)->rb.SetMassData();
			}
			++iter;
		}
		else { ++iter; }
	}
	for (std::shared_ptr<Entity> entPtr : lvlEntitiesPhase2) {
		auto enemy = dynamic_cast<Enemy*>(entPtr.get());
		enemy->TurnToMetal();
		AddEntPtrToSector(entPtr);
	}

	if (sectEndObj) {
		sectEndObj->Unlock();
	}
	resources->PlaySound(RESOURCES::WARNING7);
}

void Sector::SwitchLevelToPhaseTwo()
{
	myLevel->SwitchSectorsToPhaseTwo();
	for (auto doorPtr : sectDoors) {
		doorPtr->Unlock();
	}
}

void Sector::Awaken()
{
	//disabled the door entered through
	Door* closestDoor = NULL;
	for (std::shared_ptr<Entity> entPtr : lvlEntitiesPhase1) {
		if (entPtr->GetTypeID() == DOOR) {
			if (closestDoor == NULL) {
				closestDoor = dynamic_cast<Door*>(entPtr.get());
			}
			else if ((closestDoor->rb.transform.pos - GLBVRS::PPTR->rb.transform.pos).squaredNorm()
				> (entPtr->rb.transform.pos - GLBVRS::PPTR->rb.transform.pos).squaredNorm()) {
				closestDoor = dynamic_cast<Door*>(entPtr.get());
			}
		}
	}
	if (closestDoor != NULL) {
		closestDoor->Disable(2.0f);
	}
	//disable all doors if its phase on and enemies
	if ((sectEnemyNum > 0) && (firstPhase)) {
		LockRoom();
	}
}

void Sector::GenerateBackGround()
{
	float horCenter =GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH * (1.0f / 2.0f));
	float vertCenter = GLBVRS::VRT_MRG + (GLBVRS::CRT_HGHT / 2.0);
	std::shared_ptr<sf::Shape> drawblPtr = std::make_shared<sf::RectangleShape>(sf::Vector2f(GLBVRS::SCREEN_WIDTH, GLBVRS::SCREEN_HEIGHT));
	drawblPtr->setFillColor(OFFBLACK4);
	std::shared_ptr<Entity> backgroundColor = std::make_shared<Scenery>(
		Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH * (1.0f / 2.0f)), GLBVRS::CRT_HGHT / 2.0)),
		drawblPtr);
	lvlEntitiesPhase1.push_back(backgroundColor);

	int rightAreaLeftBound = GLBVRS::HR_MRG + GLBVRS::CRT_WDTH * (8.5f / 10.0f);
	int rightAreaRightBound = GLBVRS::HR_MRG + GLBVRS::CRT_WDTH * (10.0f / 10.0f);
	int leftAreaLeftBound = GLBVRS::HR_MRG;
	int leftAreaRightBound = GLBVRS::HR_MRG + GLBVRS::CRT_WDTH * (1.5f / 10.0f);
	int topAreaTopBound = GLBVRS::VRT_MRG;
	int topAreaBottomBound = GLBVRS::VRT_MRG + +GLBVRS::CRT_HGHT * (1.5f / 10.0f);
	int  lowAreaLowBound = GLBVRS::VRT_MRG + +GLBVRS::CRT_HGHT * (10.0f / 10.0f);
	int lowAreaTopBound = GLBVRS::VRT_MRG + +GLBVRS::CRT_HGHT * (8.5f / 10.0f);

	// OFFBLACK0, OFFBLACK1, 
	std::vector<sf::Color> randColors = {OFFBLACK2, OFFBLACK3, OFFBLACK4, OFFBLACK5, OFFBLACK6, OFFBLACK7, OFFBLACK8, OFFBLACK9};
	std::random_device rd1;
	std::mt19937 gen1(rd1());
	std::uniform_int_distribution<> distrib(0, 3);
	std::uniform_int_distribution<> distribColor(0, randColors.size()-1);

	for (int i = 0; i < 4; i++) {
		int randLine = distrib(gen1);
		sf::Color randColor = randColors[distribColor(gen1)];
		switch (randLine) {
		case 0: {//left side
			std::uniform_int_distribution<> distribTemp(leftAreaLeftBound, leftAreaRightBound);
			float xPos = distribTemp(gen1);
			std::shared_ptr<sf::Shape> lineDrawbPtr = std::make_shared<sf::RectangleShape>(sf::Vector2f(2.0f, GLBVRS::SCREEN_HEIGHT));
			lineDrawbPtr->setFillColor(randColor);
			lineDrawbPtr->setOutlineColor(randColor);
			lineDrawbPtr->setPosition(sf::Vector2f(xPos, 0));
			std::shared_ptr<Entity> line = std::make_shared<Scenery>(Vector2f(xPos, 0), lineDrawbPtr);
			lvlEntitiesPhase1.push_back(line);
			break;
		}
		case 1: {//top side
			std::uniform_int_distribution<> distribTemp(topAreaTopBound, topAreaBottomBound);
			float yPos = distribTemp(gen1);
			std::shared_ptr<sf::Shape> lineDrawbPtr = std::make_shared<sf::RectangleShape>(sf::Vector2f(GLBVRS::SCREEN_WIDTH, 2.0f));
			lineDrawbPtr->setFillColor(randColor);
			lineDrawbPtr->setOutlineColor(randColor);
			lineDrawbPtr->setPosition(sf::Vector2f(0, yPos));
			std::shared_ptr<Entity> line = std::make_shared<Scenery>(Vector2f(0, yPos), lineDrawbPtr);
			lvlEntitiesPhase1.push_back(line);
			break;
		}
		case 2: {//right side
			std::uniform_int_distribution<> distribTemp(rightAreaLeftBound, rightAreaRightBound);
			float xPos = distribTemp(gen1);
			std::shared_ptr<sf::Shape> lineDrawbPtr = std::make_shared<sf::RectangleShape>(sf::Vector2f(2.0f, GLBVRS::SCREEN_HEIGHT));
			lineDrawbPtr->setFillColor(randColor);
			lineDrawbPtr->setOutlineColor(randColor);
			lineDrawbPtr->setPosition(sf::Vector2f(xPos, 0));
			std::shared_ptr<Entity> line = std::make_shared<Scenery>(Vector2f(xPos, 0), lineDrawbPtr);
			lvlEntitiesPhase1.push_back(line);
			break;
		}
		case 3: {//bottom side
			std::uniform_int_distribution<> distribTemp(lowAreaTopBound, lowAreaLowBound);
			float yPos = distribTemp(gen1);
			std::shared_ptr<sf::Shape> lineDrawbPtr = std::make_shared<sf::RectangleShape>(sf::Vector2f(GLBVRS::SCREEN_WIDTH, 2.0f));
			lineDrawbPtr->setFillColor(randColor);
			lineDrawbPtr->setOutlineColor(randColor);
			lineDrawbPtr->setPosition(sf::Vector2f(0, yPos));
			std::shared_ptr<Entity> line = std::make_shared<Scenery>(Vector2f(0, yPos), lineDrawbPtr);
			lvlEntitiesPhase1.push_back(line);
			break;
		}
		}
	}


}

void Sector::GenerateDeathEffects(Entity * i_entPtr, ANIMTYPE i_animType)
{
	int splatNum = 2;
	int circleSplatNum = 2;
	
	std::random_device rd1;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen1(rd1()); //Standard mersenne_twister_engine seeded with rd()



	std::list<std::shared_ptr<Entity>>::iterator iter = lvlEntitiesPhase1.begin();
	for (int i = 0; i <= 5; ++i) {//want to insert corpses just after the other scenery this could be made much better
		iter++;
	}
	if (i_animType == ENEMY_BURST_DEATH) {
		for (int i = 0; i < splatNum; i++) {
			microSec ms(100000000);
			std::shared_ptr<Entity> anim = std::make_shared<Anim>(i_entPtr->rb.transform.pos, ms, ENEMY_BURST_DEATH, i_entPtr);
			lvlEntitiesPhase1.insert(iter, anim);
			if (i_entPtr->rb.shape->GetType() == Shape::ShapeType::CIRCLE) {
				auto circle = dynamic_cast<Circle*>(i_entPtr->rb.shape.get());
				std::shared_ptr<sf::Shape> corpse = Physics::CreateIrregularPolygon(5, circle->radius * 3)->GetSFMLRepr();
				int corpseR = (i_entPtr->fillColor.r + 0) / 2;
				int corpseG = (i_entPtr->fillColor.g + 0) / 2;
				int corpseB = (i_entPtr->fillColor.b + 0) / 2;
				sf::Color corpseCol(corpseR, corpseG, corpseB, 178);
				corpse->setFillColor(corpseCol);
				corpse->setOutlineColor(corpseCol);
				corpse->setPosition(sf::Vector2f(i_entPtr->rb.transform.pos.x(), i_entPtr->rb.transform.pos.y()));
				std::shared_ptr<Entity> corpseEnt = std::make_shared<Scenery>(i_entPtr->rb.transform.pos, corpse);
				lvlEntitiesPhase1.insert(iter, corpseEnt);
			}
			else if (i_entPtr->rb.shape->GetType() == Shape::ShapeType::RECTANGLE) {
				auto rectangle = dynamic_cast<Rectangle *> (i_entPtr->rb.shape.get());
				std::shared_ptr<sf::Shape> corpse = Physics::CreateIrregularPolygon(5, (rectangle->GetWidth() + rectangle->GetHeight())*1.0f)->GetSFMLRepr();
				int corpseR = (i_entPtr->fillColor.r + 0) / 2;
				int corpseG = (i_entPtr->fillColor.g + 0) / 2;
				int corpseB = (i_entPtr->fillColor.b + 0) / 2;
				sf::Color corpseCol(corpseR, corpseG, corpseB, 178);
				corpse->setFillColor(corpseCol);
				corpse->setOutlineColor(corpseCol);
				corpse->setPosition(sf::Vector2f(i_entPtr->rb.transform.pos.x(), i_entPtr->rb.transform.pos.y()));
				std::shared_ptr<Entity> corpseEnt = std::make_shared<Scenery>(i_entPtr->rb.transform.pos, corpse);
				lvlEntitiesPhase1.insert(iter, corpseEnt);
			}
			else if (i_entPtr->rb.shape->GetType() == Shape::ShapeType::POLYGON) {
				auto poly = dynamic_cast<Polygon *> (i_entPtr->rb.shape.get());
				std::shared_ptr<sf::Shape> corpse = Physics::CreateIrregularPolygon(poly->numPoints + 1, poly->GetDistToCorner() * 2)->GetSFMLRepr();
				int corpseR = (i_entPtr->fillColor.r + 0) / 2;
				int corpseG = (i_entPtr->fillColor.g + 0) / 2;
				int corpseB = (i_entPtr->fillColor.b + 0) / 2;
				sf::Color corpseCol(corpseR, corpseG, corpseB, 170);
				corpse->setFillColor(corpseCol);
				corpse->setOutlineColor(corpseCol);
				corpse->setPosition(sf::Vector2f(i_entPtr->rb.transform.pos.x(), i_entPtr->rb.transform.pos.y()));
				std::shared_ptr<Entity> corpseEnt = std::make_shared<Scenery>(i_entPtr->rb.transform.pos, corpse);
				lvlEntitiesPhase1.insert(iter, corpseEnt);
			}
		}
		for (int i = 0; i < circleSplatNum; i++) {
			float circRadius;
			if (i_entPtr->rb.shape->GetType() == Shape::ShapeType::CIRCLE) {
				auto circle = dynamic_cast<Circle*>(i_entPtr->rb.shape.get());
				circRadius = circle->radius * 0.5;
			}
			else if (i_entPtr->rb.shape->GetType() == Shape::ShapeType::RECTANGLE) {
				auto rectangle = dynamic_cast<Rectangle *> (i_entPtr->rb.shape.get());
				circRadius = (rectangle->GetWidth() + rectangle->GetHeight())*0.25f;
			}
			else if (i_entPtr->rb.shape->GetType() == Shape::ShapeType::POLYGON) {
				auto poly = dynamic_cast<Polygon *> (i_entPtr->rb.shape.get());
				circRadius = poly->GetDistToCorner() * 0.75f;
			}
			std::uniform_int_distribution<> transformDistrib(-(int)circRadius, (int)circRadius); //both boundaries are inclusive

			int randPosDiffX = transformDistrib(gen1);
			int randPosDiffY = transformDistrib(gen1);
			int corpseR = ((i_entPtr->fillColor.r + 0) / 2); // +(randPosDiffX / 4.0);
			int corpseG = ((i_entPtr->fillColor.g + 0) / 2); // +(randPosDiffX / 4.0);
			int corpseB = ((i_entPtr->fillColor.b + 0) / 2); // +(randPosDiffX / 4.0);
			sf::Color corpseCol(corpseR, corpseG, corpseB, 170);
			std::shared_ptr<sf::Shape> corpse;
			float radius;
			if (i == 0) {
				radius = (circRadius * 1.5);
				corpse = std::make_shared<sf::CircleShape>(radius);
				corpse->setPosition(sf::Vector2f(i_entPtr->rb.transform.pos.x(), i_entPtr->rb.transform.pos.y()));

			}
			else {
				radius = circRadius * 0.50f + (randPosDiffY * 0.40f);
				corpse = std::make_shared<sf::CircleShape>(radius);
				corpse->setPosition(sf::Vector2f(i_entPtr->rb.transform.pos.x() + (randPosDiffX * 2.0f), i_entPtr->rb.transform.pos.y() + (randPosDiffY * 2.0f)));
			}
			corpse->setOrigin(sf::Vector2f(radius * 0.75f, radius * 0.75f));
			corpse->setFillColor(corpseCol);
			corpse->setOutlineColor(corpseCol);

			std::shared_ptr<Entity> corpseEnt = std::make_shared<Scenery>(i_entPtr->rb.transform.pos, corpse);
			lvlEntitiesPhase1.insert(iter, corpseEnt);
		}
	}
	if (i_animType == CANNED_EXPLOSION) {
		microSec ms(5000000000);
		std::shared_ptr<Entity> anim = std::make_shared<Anim>(i_entPtr->rb.transform.pos, ms, CANNED_EXPLOSION, i_entPtr);
		lvlEntitiesPhase1.insert(iter, anim);
	}
}

void Sector::GenerateIrregularTerrain(int i_numVerts, int i_maxSize, int i_minSize)
{
	std::random_device rd;
	std::mt19937 gen1(rd());
	std::uniform_int_distribution<> centerDist(0, 2);
	int center = centerDist(gen1);
	std::vector<std::tuple<Vector2f, Vector2f>> screenAreas;
	if (center == 0) {
		screenAreas = GetScreenAreas(CENTER);
	}
	else  {
		screenAreas = GetScreenAreas(MARGINS);
		i_minSize += 2000;
	}
	//else {
	//	screenAreas = GetScreenAreas(CORNERS);
	//}

	int leftX = (int)std::get<0>(screenAreas[0])[0];
	int rightX = (int)std::get<1>(screenAreas[0])[0];
	int topY = (int)std::get<0>(screenAreas[0])[1];
	int bottomY = (int)std::get<1>(screenAreas[0])[1];
	std::uniform_int_distribution<> xCordDist(leftX, rightX);
	std::uniform_int_distribution<> yCordDist(topY, bottomY);
	int randXcordInBounds = xCordDist(gen1);
	int randYcordInBounds = yCordDist(gen1);
	Vector2f spawnPos(randXcordInBounds, randYcordInBounds);
	std::shared_ptr<Shape> vertRect1 = Physics::CreateIrregularPolygon(i_numVerts, i_maxSize, i_minSize);
	RigidBody rightWallBody = RigidBody(vertRect1, STATIC);
	std::shared_ptr<Entity> rightWall = std::make_shared<Wall>(
		spawnPos, rightWallBody, colPalA, colPalB, true);
	lvlEntitiesPhase1.push_back(rightWall);
}

void Sector::UnlockRoom()
{
	auto entPointer = GLBVRS::PPTR;
	auto playPtr = dynamic_cast<PlayerChar*>(entPointer.get());
	playPtr->lastAOEFired = std::chrono::high_resolution_clock::now() - std::chrono::minutes(1);
	for (auto doorPtr : sectDoors) {
		doorPtr->Unlock();
	}
}

void Sector::LockRoom()
{
	for (auto doorPtr : sectDoors) {
		doorPtr->Lock();
	}
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
