#include "Sector.h"

Sector::Sector(Level* i_lvlPtr, int i_dim_x, int i_dim_y, RESOURCES* i_resources, sf::Color i_colA, sf::Color i_colB, bool i_testSector) :
	myLevel{ i_lvlPtr }, sectorWidth{ i_dim_x }, sectorHeight{ i_dim_y }, resources{ i_resources }, colPalA{ i_colA }, colPalB{ i_colB }
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
	//SwitchToPhaseTwo();
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
		TypeID type = entPtr->GetTypeID();
		for (auto spawnable : entPtr->spawnVect) {
			//if type if enemy dont let it spawn stuff out of bounds
			if (auto enemy = dynamic_cast<Enemy*>(entPtr.get())) { //((type == TypeID::ENEMY_SEEK) || (type == TypeID::ENEMY_RAND) || (type = TypeID::ENEMY_RAND_PUSH) || (type == TypeID::ENEMY_SEEK_PUSH) || (type == TypeID::ENEMY_BOSS)) {
				Vector2f position = spawnable->rb.transform.pos;
				if ((position[0] < 0.0f) || (position[0] > sectorWidth) || (position[1] < 0.0f) || (position[1] > sectorHeight)) {
					//std::cout << "Not spawning out of bounds" << "\n";
				}
				else {
					AddEntPtrToSector(spawnable);
				}
			}
			else {
				AddEntPtrToSector(spawnable);
			}
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
		std::mt19937 genSizeSeed(sizeSeed());
		std::random_device sizeSeedRandEnemy;
		std::mt19937 genSizeSeedRandEnemy(sizeSeed());
		std::discrete_distribution<> radiusModDist({ 1, 4, 3, 2, 1 });
		//std::discrete_distribution<> widthModDist({ 10, 15, 50, 15, 10 });
		//std::discrete_distribution<> heightModDist({ 13, 18, 40, 18, 13 });
		std::discrete_distribution<> sizeIncDist({ 2, 3, 3, 1, 1});

		int randRad = radiusModDist(sizeSeedRandEnemy);
		int randDifRadius = (randRad + i_sizeMod) * 10;
		float sizeInc = (sizeIncDist(genSizeSeed) - 1) * 0.2f;

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
			//std::shared_ptr<Shape> shape = std::make_shared<Rectangle>((65 + ranDifwidth), (65 + randDifHeight));
			//std::shared_ptr<Shape> shape = std::make_shared<Rectangle>(60 + randSizeDiff, 60 + randSizeDiff2);
			std::vector<Vector2f> polyPoints = polyLib.GetRandomVertArray();
			std::shared_ptr<Polygon> poly = std::make_shared<Polygon>(polyPoints);
			poly->ResetSize(1 + sizeInc);
			std::shared_ptr<Shape> shape = poly;
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
			std::random_device rd1;
			std::mt19937 gen1(rd1());
			std::uniform_int_distribution<> distrib(1, 3);
			int triEnemy = distrib(gen1);
			if (triEnemy == 1) { //1 third chance triangle, otherwise its just an irregular poly
				std::vector<Vector2f> polyPoints = polyLib.GetRandomTriangleVertArray();
				std::shared_ptr<Polygon> poly = std::make_shared<Polygon>(polyPoints);
				poly->ResetSize(1 + sizeInc);
				std::shared_ptr<Shape> shape = poly;
				Material Rock = Material(0.6f, 0.1f, 0.6f, 0.3f);
				RigidBody projBody = RigidBody(shape, Rock);
				ent = std::make_shared<Enemy>(i_diff, spawnPos, projBody);
				break;
			}
			else {
				std::vector<Vector2f> polyPoints = polyLib.GetRandomVertArray();
				std::shared_ptr<Polygon> poly = std::make_shared<Polygon>(polyPoints);
				poly->ResetSize(1 + sizeInc);
				std::shared_ptr<Shape> shape = poly;
				Material Rock = Material(0.6f, 0.1f, 0.6f, 0.3f);
				RigidBody projBody = RigidBody(shape, Rock);
				ent = std::make_shared<Enemy>(i_diff, spawnPos, projBody);
				break;
			}
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

	float wall_thickness = 500.0f;
	sf::Color invisible(0.0f, 0.0f, 0.0f, 0.0f);
	//Right wall
	std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(wall_thickness, sectorHeight * 2.0f);
	RigidBody rightWallBody = RigidBody(vertRect1, Static);
	std::shared_ptr<Entity> rightWall = std::make_shared<Wall>(
		Vector2f((sectorWidth) + (wall_thickness / 2.0f), (sectorHeight / 2.0f)), rightWallBody, invisible, invisible);
	lvlEntitiesPhase1.push_back(rightWall);
	//Left wall
	std::shared_ptr<Shape> vertRect2 = std::make_shared<Rectangle>(wall_thickness, sectorHeight * 2.0f);
	RigidBody leftWallBody = RigidBody(vertRect2, Static);
	std::shared_ptr<Entity> leftWall = std::make_shared<Wall>(
		Vector2f((-wall_thickness * 0.5), (sectorHeight / 2.0f)), leftWallBody, invisible, invisible);
	AddEntPtrToSector(leftWall);
	//Top wall
	std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(sectorWidth, wall_thickness);
	RigidBody wallBody1 = RigidBody(horRect1, Static);
	std::shared_ptr<Entity> upperWall = std::make_shared<Wall>(
		Vector2f((sectorWidth / 2.0f), - (wall_thickness * 0.5f)), wallBody1, invisible, invisible);
	lvlEntitiesPhase1.push_back(upperWall);

	//Bottom wall
	std::shared_ptr<Shape> horRect2 = std::make_shared<Rectangle>(sectorWidth, wall_thickness);
	RigidBody lowerWallBody = RigidBody(horRect2, Static);
	std::shared_ptr<Entity> lowerWall = std::make_shared<Wall>(
		Vector2f(Vector2f((sectorWidth / 2.0f), (wall_thickness * 0.5f) + sectorHeight)), lowerWallBody, invisible, invisible);
	lvlEntitiesPhase1.push_back(lowerWall);

	//Wall Border
	//std::shared_ptr<Shape> horRect22 = std::make_shared<Rectangle>(sectorWidth, sectorHeight);
	//RigidBody lowerWallBody12 = RigidBody(horRect22, Static);
	////drawblBorderPtr->setFillColor(sf::Color(0, 0, 0, 0));
	////drawblBorderPtr->setOutlineColor(colPalB);
	////drawblBorderPtr->setOutlineThickness(GLBVRS::DOOR_HEIGHT);
	//std::shared_ptr<Entity> drawblBorderEnt = std::make_shared<Wall>(Vector2f(0.0f, 0.0f), lowerWallBody12, sf::Color(0, 0, 0, 0), colPalB);
	//drawblBorderEnt->intangible = true;
	//drawblBorderEnt->intangible = true;
	//lvlEntitiesPhase1.push_back(drawblBorderEnt);


	float BorderThickness = GLBVRS::DOOR_HEIGHT * 2;
	//colPalB = sf::Color::White;
	std::shared_ptr<sf::Shape> topWallPtr = std::make_shared<sf::RectangleShape>(sf::Vector2f(sectorWidth, BorderThickness));
	topWallPtr->setFillColor(sf::Color::White);
	//topWallPtr->setOutlineColor(sf::Color(0, 0, 0, 0));
	topWallPtr->setOutlineThickness(0);
	float topHeightInverse = 1.0f / topWallPtr->getLocalBounds().height;
	std::shared_ptr<Entity> topWallEnt = std::make_shared<Scenery>(
		Vector2f(0.0f, -BorderThickness),
		topWallPtr, 1, RigidBody(std::make_shared<Circle>(1.0f)), "tile136", 0.5f, topHeightInverse * 64);
	lvlEntitiesPhase1.push_back(topWallEnt);

	std::shared_ptr<sf::Shape> bottompWallPtr = std::make_shared<sf::RectangleShape>(sf::Vector2f(sectorWidth, BorderThickness));
	bottompWallPtr->setFillColor(sf::Color::White);
	bottompWallPtr->setOutlineColor(sf::Color(0, 0, 0, 0));
	bottompWallPtr->setOutlineThickness(BorderThickness);
	bottompWallPtr->setOrigin(sf::Vector2f(0.0f, 0.0f));
	float bottomHeightInverse = 1.0f / bottompWallPtr->getLocalBounds().height;
	std::shared_ptr<Entity> bottomWallEnt = std::make_shared<Scenery>(
		Vector2f(0.0f, sectorHeight),
		bottompWallPtr, 1, RigidBody(std::make_shared<Circle>(1.0f)), "tile136", 0.5, bottomHeightInverse * 64);
	lvlEntitiesPhase1.push_back(bottomWallEnt);

	std::shared_ptr<sf::Shape> rightWallPtr = std::make_shared<sf::RectangleShape>(sf::Vector2f(sectorHeight + (BorderThickness * 2), BorderThickness));
	rightWallPtr->setFillColor(sf::Color::White);
	rightWallPtr->setOutlineColor(sf::Color(0, 0, 0, 0));
	rightWallPtr->setOutlineThickness(BorderThickness);
	rightWallPtr->setOrigin(sf::Vector2f(0.0f, 0.0f));
	rightWallPtr->setRotation(90);
	float rightHeightInverse = 1.0f / rightWallPtr->getLocalBounds().height;
	std::shared_ptr<Entity> rightWallEnt = std::make_shared<Scenery>(
		Vector2f(sectorWidth + BorderThickness, -BorderThickness),
		rightWallPtr, 1, RigidBody(std::make_shared<Circle>(1.0f)), "tile136", 0.5f, rightHeightInverse * 64);
	lvlEntitiesPhase1.push_back(rightWallEnt);

	std::shared_ptr<sf::Shape> leftWallPtr = std::make_shared<sf::RectangleShape>(sf::Vector2f(sectorHeight + (BorderThickness * 2), BorderThickness));
	leftWallPtr->setFillColor(sf::Color::White);
	leftWallPtr->setOutlineColor(sf::Color(0, 0, 0, 0));
	leftWallPtr->setOutlineThickness(BorderThickness);
	leftWallPtr->setOrigin(sf::Vector2f(0.0f, 0.0f));
	leftWallPtr->setRotation(90);
	float leftHeightInverse = 1.0f / leftWallPtr->getLocalBounds().height;
	std::shared_ptr<Entity> leftWallEnt = std::make_shared<Scenery>(
		Vector2f(0.0f, -BorderThickness),
		leftWallPtr, 1, RigidBody(std::make_shared<Circle>(1.0f)), "tile136", 0.5f, leftHeightInverse * 64);
	lvlEntitiesPhase1.push_back(leftWallEnt);

	//WALL CORNERS
	float cornerThicknessInverse = 1.0f / BorderThickness;

	std::shared_ptr<sf::Shape> topLeftWallPtr = std::make_shared<sf::RectangleShape>(sf::Vector2f(BorderThickness, BorderThickness));
	topLeftWallPtr->setFillColor(sf::Color::White);
	topLeftWallPtr->setOutlineThickness(0);
	std::shared_ptr<Entity> topLeftCornerEnt = std::make_shared<Scenery>(
		Vector2f(-BorderThickness, -BorderThickness),
		topLeftWallPtr, 1, RigidBody(std::make_shared<Circle>(1.0f)), "tile140", cornerThicknessInverse * 64, cornerThicknessInverse * 64);
	lvlEntitiesPhase1.push_back(topLeftCornerEnt);

	std::shared_ptr<sf::Shape> topRightWallPtr = std::make_shared<sf::RectangleShape>(sf::Vector2f(BorderThickness, BorderThickness));
	topRightWallPtr->setFillColor(sf::Color::White);
	topRightWallPtr->setOutlineThickness(0);
	std::shared_ptr<Entity> topRightCornerEnt = std::make_shared<Scenery>(
		Vector2f(sectorWidth, -BorderThickness),
		topRightWallPtr, 1, RigidBody(std::make_shared<Circle>(1.0f)), "tile140", cornerThicknessInverse * 64, cornerThicknessInverse * 64);
	lvlEntitiesPhase1.push_back(topRightCornerEnt);

	std::shared_ptr<sf::Shape> bottomRightWallPtr = std::make_shared<sf::RectangleShape>(sf::Vector2f(BorderThickness, BorderThickness));
	bottomRightWallPtr->setFillColor(sf::Color::White);
	bottomRightWallPtr->setOutlineThickness(0);
	std::shared_ptr<Entity> bottomRightCornerEnt = std::make_shared<Scenery>(
		Vector2f(sectorWidth, sectorHeight),
		bottomRightWallPtr, 1, RigidBody(std::make_shared<Circle>(1.0f)), "tile140", cornerThicknessInverse * 64, cornerThicknessInverse * 64);
	lvlEntitiesPhase1.push_back(bottomRightCornerEnt);

	std::shared_ptr<sf::Shape> bottomLeftWallPtr = std::make_shared<sf::RectangleShape>(sf::Vector2f(BorderThickness, BorderThickness));
	bottomLeftWallPtr->setFillColor(sf::Color::White);
	bottomLeftWallPtr->setOutlineThickness(0);
	std::shared_ptr<Entity> bottomLeftCornerEnt = std::make_shared<Scenery>(
		Vector2f(-BorderThickness, sectorHeight),
		bottomLeftWallPtr, 1, RigidBody(std::make_shared<Circle>(1.0f)), "tile140", cornerThicknessInverse * 64, cornerThicknessInverse * 64);
	lvlEntitiesPhase1.push_back(bottomLeftCornerEnt);
}

void Sector::AddPainWall(int i_index)
{
	Material Static = Material(0.0f, 0.4f, 0.4f, 0.2f);

	float halfCrtVrtWall = (sectorHeight - GLBVRS::DOOR_WIDTH) * 0.5;
	float halfCrtHorWall = (sectorWidth - GLBVRS::DOOR_WIDTH) * 0.5;
	
	float vrtPWallHght = halfCrtVrtWall * 0.8f;
	float vrtPnWllCornOffset = halfCrtVrtWall * 0.1f;

	float horPWallHght = halfCrtHorWall * 0.8f;
	float horPWallCornOffset = halfCrtHorWall * 0.1f;

	float pWallWidth = GLBVRS::DOOR_HEIGHT * 2;

	std::shared_ptr<Entity> wall;

	switch (i_index) {
	case (0): {
		std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(horPWallHght, pWallWidth); //leftmost upper wall
		RigidBody wallBody1 = RigidBody(horRect1, Static);
		wall = std::make_shared<PainWall>(
			Vector2f(horPWallCornOffset + (horPWallHght * 0.5), -(pWallWidth * 0.5f)), wallBody1);
		break;
	}
	case (1): {
		std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(horPWallHght, pWallWidth); //rightmost upper wall;
		RigidBody wallBody1 = RigidBody(horRect1, Static);
		wall = std::make_shared<PainWall>(
			Vector2f((horPWallCornOffset * 3.0f) + GLBVRS::DOOR_WIDTH + (horPWallHght * 1.5), -(pWallWidth * 0.5f)), wallBody1);
		break;
	}
	case (2): {
		std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(pWallWidth, vrtPWallHght); // uppermost right wall 
		RigidBody rightWallBody = RigidBody(vertRect1, Static);
		wall = std::make_shared<PainWall>(
			Vector2f((sectorWidth)+(pWallWidth * 0.5f), vrtPnWllCornOffset + (vrtPWallHght * 0.5f)), rightWallBody);
		break;
	}
	case (3): {
		std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(pWallWidth, vrtPWallHght); // lowermost right wall
		RigidBody rightWallBody = RigidBody(vertRect1, Static);
		wall = std::make_shared<PainWall>(
			Vector2f((sectorWidth)+(pWallWidth * 0.5f), halfCrtVrtWall + GLBVRS::DOOR_WIDTH + vrtPnWllCornOffset + (vrtPWallHght * 0.5f)), rightWallBody);
		break;
	}
	case (4): {
		std::shared_ptr<Shape> horRect2 = std::make_shared<Rectangle>(horPWallHght, pWallWidth); //rightmost bottom wall
		RigidBody lowerWallBody = RigidBody(horRect2, Static);
		wall = std::make_shared<PainWall>(
			Vector2f((horPWallCornOffset * 3.0f) + GLBVRS::DOOR_WIDTH + (horPWallHght * 1.5), sectorHeight + (pWallWidth * 0.5f)), lowerWallBody);
		break;
	}
	case (5): {
		std::shared_ptr<Shape> vertRect2 = std::make_shared<Rectangle>(horPWallHght, pWallWidth); // leftmost bottom wall
		RigidBody leftWallBody = RigidBody(vertRect2, Static);
		wall = std::make_shared<PainWall>(
			Vector2f(horPWallCornOffset + (horPWallHght * 0.5), sectorHeight + (pWallWidth * 0.5f)), leftWallBody);
		break;
	}
	case (6): {
		std::shared_ptr<Shape> horRect2 = std::make_shared<Rectangle>(pWallWidth, vrtPWallHght); //lowermost left wall
		RigidBody lowerWallBody = RigidBody(horRect2, Static);
		wall = std::make_shared<PainWall>(
			Vector2f(-(pWallWidth * 0.5f), (vrtPnWllCornOffset * 3.0f) + GLBVRS::DOOR_WIDTH + (vrtPWallHght * 1.5f)), lowerWallBody);
		break;
	}
	case (7): {
		std::shared_ptr<Shape> vertRect2 = std::make_shared<Rectangle>(pWallWidth, vrtPWallHght); //uppermost left wall
		RigidBody leftWallBody = RigidBody(vertRect2, Static);
		wall = std::make_shared<PainWall>(
			Vector2f(-(pWallWidth * 0.5f), vrtPnWllCornOffset + (vrtPWallHght * 0.5f)), leftWallBody);
		break;
	}
	}

	lvlEntitiesPhase1.push_back(wall);
}

void Sector::PopulateEntranceRoom()
{
	std::shared_ptr<Entity> lowerWall = std::make_shared<EndObject>(
				Vector2f(Vector2f((sectorWidth / 2.0f), sectorHeight / 2.0)));
	AddEntPtrToSector(lowerWall);

	//AddPainWall(0);
	//AddPainWall(1);
	//AddPainWall(2);
	//AddPainWall(3);
	//AddPainWall(4);
	//AddPainWall(5);
	//AddPainWall(6);
	//AddPainWall(7);

	//std::shared_ptr<Entity> smallShipPOW2 = std::make_shared<PowerUp>(
	//	Vector2f(Vector2f(GLBVRS::HR_MRG + (sectorWidth * (1.0f / 5.0f)), sectorHeight / 2.0)), RATE_OF_FIRE);
	//lvlEntitiesPhase1.push_back(smallShipPOW2);

	//std::shared_ptr<Entity> smallShipPOW23 = std::make_shared<PowerUp>(
	//	Vector2f(Vector2f(GLBVRS::HR_MRG + (sectorWidth * (1.0f / 5.0f)), sectorHeight / 2.0)), RATE_OF_FIRE);
	//lvlEntitiesPhase1.push_back(smallShipPOW23);

	//std::shared_ptr<Entity> smallShipPOW24 = std::make_shared<PowerUp>(
	//	Vector2f(Vector2f(GLBVRS::HR_MRG + (sectorWidth * (1.0f / 5.0f)), sectorHeight / 2.0)), RATE_OF_FIRE);
	//lvlEntitiesPhase1.push_back(smallShipPOW24);

	//std::shared_ptr<Entity> smallShipPOW21 = std::make_shared<PowerUp>(
	//	Vector2f(Vector2f(GLBVRS::HR_MRG + (sectorWidth * (1.0f / 5.0f)), sectorHeight / 2.0)), SCATTER);
	//lvlEntitiesPhase1.push_back(smallShipPOW21);

	//std::shared_ptr<Entity> smallShipPOW278 = std::make_shared<PowerUp>(
	//	Vector2f(Vector2f(GLBVRS::HR_MRG + (sectorWidth * (1.0f / 5.0f)), sectorHeight / 2.0)), SCATTER);
	//lvlEntitiesPhase1.push_back(smallShipPOW278);

	//std::shared_ptr<Entity> smallShipPOW2123 = std::make_shared<PowerUp>(
	//	Vector2f(Vector2f(GLBVRS::HR_MRG + (sectorWidth * (1.0f / 5.0f)), sectorHeight / 2.0)), SCATTER);
	//lvlEntitiesPhase1.push_back(smallShipPOW2123);

	//std::shared_ptr<Entity> smallSh1 = std::make_shared<PowerUp>(
	//	Vector2f(Vector2f(GLBVRS::HR_MRG + (sectorWidth * (1.0f / 5.0f)), sectorHeight / 2.0)), SMALL_SHIP);
	//lvlEntitiesPhase1.push_back(smallSh1);

	//std::shared_ptr<Entity> smallSh11 = std::make_shared<PowerUp>(
	//	Vector2f(Vector2f(GLBVRS::HR_MRG + (sectorWidth * (1.0f / 5.0f)), sectorHeight / 2.0)), SMALL_SHIP);
	//lvlEntitiesPhase1.push_back(smallSh11);

	//std::shared_ptr<Entity> smallSh21231 = std::make_shared<PowerUp>(
	//	Vector2f(Vector2f(GLBVRS::HR_MRG + (sectorWidth * (1.0f / 5.0f)), sectorHeight / 2.0)), SMALL_SHIP);
	//lvlEntitiesPhase1.push_back(smallSh21231);

	//std::shared_ptr<Entity> smallSh1123 = std::make_shared<PowerUp>(
	//	Vector2f(Vector2f(GLBVRS::HR_MRG + (sectorWidth * (1.0f / 5.0f)), sectorHeight / 2.0)), WEAP_SPEED);
	//lvlEntitiesPhase1.push_back(smallSh1123);

	//std::shared_ptr<Entity> smal123lSh11 = std::make_shared<PowerUp>(
	//	Vector2f(Vector2f(GLBVRS::HR_MRG + (sectorWidth * (1.0f / 5.0f)), sectorHeight / 2.0)), WEAP_SPEED);
	//lvlEntitiesPhase1.push_back(smal123lSh11);

	//std::shared_ptr<Entity> small234234Sh21231 = std::make_shared<PowerUp>(
	//	Vector2f(Vector2f(GLBVRS::HR_MRG + (sectorWidth * (1.0f / 5.0f)), sectorHeight / 2.0)), WEAP_SPEED);
	//lvlEntitiesPhase1.push_back(small234234Sh21231);

	//std::shared_ptr<Entity> smallSh1123sadfasd = std::make_shared<PowerUp>(
	//	Vector2f(Vector2f(GLBVRS::HR_MRG + (sectorWidth * (1.0f / 5.0f)), sectorHeight / 2.0)), BLAST);
	//lvlEntitiesPhase1.push_back(smallSh1123sadfasd);

	//std::shared_ptr<Entity> smal123lasdxzcvSh11 = std::make_shared<PowerUp>(
	//	Vector2f(Vector2f(GLBVRS::HR_MRG + (sectorWidth * (1.0f / 5.0f)), sectorHeight / 2.0)), BLAST);
	//lvlEntitiesPhase1.push_back(smal123lasdxzcvSh11);

	//std::shared_ptr<Entity> sma234234ll234234Sh21231 = std::make_shared<PowerUp>(
	//	Vector2f(Vector2f(GLBVRS::HR_MRG + (sectorWidth * (1.0f / 5.0f)), sectorHeight / 2.0)), BLAST);
	//lvlEntitiesPhase1.push_back(sma234234ll234234Sh21231);


	//auto ent = std::make_shared<BossBurst>(EASY, Vector2f(100, 100));
	//lvlEntitiesPhase1.push_back(ent);

	////auto ent = std::make_shared<BossSpawn>(EASY, Vector2f(100, 100), true);
	//auto ent = std::make_shared<BossStream>(EASY, Vector2f(100, 100));
	////lvlEntitiesPhase1.push_back(ent);

	////auto ent = std::make_shared<BossBurst>(EASY, Vector2f(100, 100));
	//lvlEntitiesPhase1.push_back(ent);

	//if (TESTING) {
	//	std::shared_ptr<Entity> smallShipPOW2 = std::make_shared<PowerUp>(
	//		Vector2f(Vector2f(GLBVRS::HR_MRG + (sectorWidth * (1.0f / 5.0f)), sectorHeight / 2.0)), TEMP_HEALTH);
	//	lvlEntitiesPhase1.push_back(smallShipPOW2);


	//	//auto ent = std::make_shared<BossSpawn>(MEDIUM, Vector2f(100, 100));
	//	//auto ent = std::make_shared<BossSplit>(MEDIUM, 4, 6, false, Vector2f(100, 100));


	//	//auto ent = std::make_shared<BossSplit>(MEDIUM, Vector2f(100, 100));
	//	auto ent = std::make_shared<BossBurst>(EASY, Vector2f(100, 100));
	//	lvlEntitiesPhase1.push_back(ent);


	//	//std::shared_ptr<Entity> smallShipPOW3 = std::make_shared<PowerUp>(this,
	//	//	Vector2f(Vector2f(GLBVRS::HR_MRG + (sectorWidth * (2.0f / 5.0f)), sectorHeight / 2.0)), SMALL_SHIP);
	//	//lvlEntitiesPhase1.push_back(smallShipPOW3);

	//}

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
	int bossChoice = i_lvlNum % 5;
	switch (bossChoice) {
	case 0: {
		if (!i_isMiniBoss) {
			ent = std::make_shared<BossBurst>(i_diff, spawnPos, false, i_lvlNum);
		}
		else {
			ent = std::make_shared<BossBurst>(i_diff, spawnPos, true, i_lvlNum);
		}
		break;
	}
	case 1: {
		if (!i_isMiniBoss) {
			ent = std::make_shared<BossRush>(i_diff, spawnPos, false, i_lvlNum);
			AddPainWall(0);
			AddPainWall(1);
			AddPainWall(2);
			AddPainWall(3);
			AddPainWall(4);
			AddPainWall(5);
			AddPainWall(6);
			AddPainWall(7);
		}
		else {
			ent = std::make_shared<BossRush>(i_diff, spawnPos, true, i_lvlNum);
			//AddRandomPainWall()
		}
		break;
	}
	case 2: {
		if (!i_isMiniBoss) {
			ent = std::make_shared<BossStream>(i_diff, spawnPos, false, i_lvlNum);
		}
		else {
			ent = std::make_shared<BossStream>(i_diff, spawnPos, true, i_lvlNum);
		}
		break;
	}
	case 3: {
		if (!i_isMiniBoss) {
			ent = std::make_shared<BossSplit>(i_diff, 4, 6, false, spawnPos, i_lvlNum);
		}
		else {
		//std::shared_ptr<Shape> shape1 = std::make_shared<Circle>(30);
		//RigidBody projBody1 = RigidBody(shape1, ROCK);

		std::shared_ptr<Shape> shape1 = std::make_shared<Circle>(50);
		RigidBody projBody1 = RigidBody(shape1, LESSBOUNCYBALL);

		ent = std::make_shared<BossSplit>(i_diff, 1, 6, false, spawnPos, i_lvlNum, projBody1);
		}
		break;
	}
	case 4: {
		if (!i_isMiniBoss) {
			ent = std::make_shared<BossSpawn>(i_diff, spawnPos, false, i_lvlNum);
			AddTerrain(0, 1);
			AddTerrain(1, 0);
			AddTerrain(2, 1);
			AddTerrain(3, 0);
		}
		else {
			//remove all terrain from level TODO, then add exactly two
			ent = std::make_shared<BossSpawn>(i_diff, spawnPos, true, i_lvlNum);
			//AddTerrain(1, 0);
			//AddTerrain(3, 0);
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
	Vector2f spawnPos(sectorWidth, sectorHeight);
	std::shared_ptr<Shape> shape = std::make_shared<Rectangle>(100, 100);
	Material Rock = Material(0.6f, 0.1f, 0.6f, 0.3f);
	RigidBody projBody = RigidBody(shape, Rock);
	std::shared_ptr<Entity> ent = std::make_shared<Enemy>((DIFFICULTY)0, spawnPos, projBody);
	//auto enemy = dynamic_cast<Enemy*>(ent.get());
	lvlEntitiesPhase1.push_back(ent);
	++sectEnemyNum;

}

void Sector::RemoveDestroyedEntities() {
	if ((sectEnemyNum > 0) && (firstPhase)) {
		std::list<std::shared_ptr<Entity>>::iterator boundsCheckIter = lvlEntitiesPhase1.begin();
		while (boundsCheckIter != lvlEntitiesPhase1.end()) {
			Vector2f position = boundsCheckIter._Ptr->_Myval->rb.transform.pos;
			int entType = boundsCheckIter._Ptr->_Myval->GetTypeID();
			if ((entType == ENEMY_SEEK) || (entType == ENEMY_RAND) || (entType == ENEMY_SEEK) ||
				(entType == ENEMY_SEEK_PUSH) || (entType == ENEMY_RAND_PUSH) || (entType == ENEMY_BOSS)) {
				if ((position[0] < 0.0f) || (position[0] > sectorWidth) || (position[1] < 0.0f) || (position[1] > sectorHeight)) {
					boundsCheckIter._Ptr->_Myval->Destroy();
				}
			}
			boundsCheckIter++;
		}
	}
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
	if ((isBossRoom) && (sectEnemyNum <= 0) && (firstPhase)) {
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
	float smallWidth = 40.0f;
	float smallHeight = sectorHeight * 0.25f;
	float bigWidth = 40.0f;
	// A wall must always be small enough that a ship can fit on either side, player width == 150
	float bigHeight = std::min((sectorHeight * 0.666f), sectorHeight - 300.0f);
	//if a wall must be extremely small to fit just dont put it in
	if (bigHeight < 150) {
		return;
	}
	//also if the player can't squeeze between the terrain and the doors dont put it in
	if ((sectorWidth * (1.0f / 5.0f)) < 160) {
		return;
	}
	if ((sectorHeight * (1.0f / 5.0f)) < 160) {
		return;
	}

	switch (terrType) {
		//small walls
	case 0: {
		std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(smallWidth, smallHeight);
		RigidBody rightWallBody = RigidBody(vertRect1, STATIC);
		std::shared_ptr<Entity> rightWall = std::make_shared<Wall>(
			Vector2f((sectorWidth * ( 1.0f / 5.0f)), (sectorHeight / 2.0f)), rightWallBody, colPalA, colPalB, true);
		lvlEntitiesPhase1.push_back(rightWall);
		break;
	}
	case 1: {
		std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(smallWidth, sectorHeight / 4.0f);
		RigidBody rightWallBody = RigidBody(vertRect1, STATIC);
		std::shared_ptr<Entity> rightWall = std::make_shared<Wall>(
			Vector2f((sectorWidth *  (4.0f / 5.0f)), (sectorHeight / 2.0f)), rightWallBody, colPalA, colPalB, true);
		lvlEntitiesPhase1.push_back(rightWall);
		break;
	}
	case 2: {
		std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(sectorWidth / 4.0f, smallWidth);
		RigidBody wallBody1 = RigidBody(horRect1, STATIC);
		std::shared_ptr<Entity> upperWall = std::make_shared<Wall>(
			Vector2f((sectorWidth / 2.0f), (sectorHeight * (1.0f / 5.0f))), wallBody1, colPalA, colPalB, true);
		lvlEntitiesPhase1.push_back(upperWall);
		break;
	}
	case 3: {
		std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(sectorWidth / 4.0f, smallWidth);
		RigidBody wallBody1 = RigidBody(horRect1, STATIC);
		std::shared_ptr<Entity> upperWall = std::make_shared<Wall>(
			Vector2f((sectorWidth / 2.0f), (sectorHeight * (4.0f / 5.0f))), wallBody1, colPalA, colPalB, true);
		lvlEntitiesPhase1.push_back(upperWall);
		break;
	}
	//big walls
	case 4: {
		std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(bigWidth, bigHeight);
		RigidBody rightWallBody = RigidBody(vertRect1, STATIC);
		std::shared_ptr<Entity> rightWall = std::make_shared<Wall>(
			Vector2f((sectorWidth * (1.0f / 5.0f)), (sectorHeight / 2.0f)), rightWallBody, colPalA, colPalB, true);
		lvlEntitiesPhase1.push_back(rightWall);
		break;
	}
	case 5: {
		std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(bigWidth, bigHeight);
		RigidBody rightWallBody = RigidBody(vertRect1, STATIC);
		std::shared_ptr<Entity> rightWall = std::make_shared<Wall>(
			Vector2f((sectorWidth *  (4.0f / 5.0f)), (sectorHeight / 2.0f)), rightWallBody, colPalA, colPalB, true);
		lvlEntitiesPhase1.push_back(rightWall);
		break;
	}
	case 6: {
		std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(bigHeight, bigWidth);
		RigidBody wallBody1 = RigidBody(horRect1, STATIC);
		std::shared_ptr<Entity> upperWall = std::make_shared<Wall>(
			Vector2f((sectorWidth / 2.0f), (sectorHeight * (1.0f / 5.0f))), wallBody1, colPalA, colPalB, true);
		lvlEntitiesPhase1.push_back(upperWall);
		break;
	}
	case 7: {
		std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(bigHeight, bigWidth);
		RigidBody wallBody1 = RigidBody(horRect1, STATIC);
		std::shared_ptr<Entity> upperWall = std::make_shared<Wall>(
			Vector2f((sectorWidth / 2.0f), (sectorHeight * (4.0f / 5.0f))), wallBody1, colPalA, colPalB, true);
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
		else if ((entType == WALL_BASIC) && (!isBossRoom)) {
			//During the phase transition, any wall will have a 2/3 chance to be turned into an irregular poly
			//because the straight walls can block doors and be a pain in the ass to navigate
			auto wall = dynamic_cast<Wall*>((*iter).get());
			Vector2f wallPos = wall->rb.transform.pos;
			if (wall->breakable) {
				std::random_device rd1;
				std::mt19937 gen1(rd1());
				std::uniform_int_distribution<> distrib(1, 3);
				int splitWall = distrib(gen1);
				if (splitWall == 1) {
					wall->TakeDamage(wall->maxHealth * 2);
					++iter;
				}
				else {
					lvlEntitiesPhase1.erase(iter++);
					std::random_device rdGenA;
					std::mt19937 genTerrain(rdGenA());
					std::uniform_int_distribution<> distribTerrainVerts(5, 7);
					int terrainVerts1 = distribTerrainVerts(genTerrain);
					int terrainVerts2 = distribTerrainVerts(genTerrain);
					std::uniform_int_distribution<> distribTerrainSize(100, 300);
					int terrainSize1 = distribTerrainSize(genTerrain) + 100;
					int terrainSize2 = distribTerrainSize(genTerrain);
					GenerateIrregularTerrain(terrainVerts1, terrainSize1, 1000, wallPos);
					GenerateIrregularTerrain(terrainVerts2, terrainSize2, 1000, wallPos);
					//GenerateIrregularTerrain(terrainVerts1, terrainSize1, 3000);
					//GenerateIrregularTerrain(terrainVerts2, terrainSize2, 3000);
				}
			}
			else { 
				wall->exploding = true;
				++iter; 
			}
		}
		else { ++iter; }
	}
	for (std::shared_ptr<Entity> entPtr : lvlEntitiesPhase2) {
		if (auto enemy = dynamic_cast<Enemy*>(entPtr.get())) { 
			enemy->TurnToMetal(); 
		}
		if (auto wall = dynamic_cast<Wall*>(entPtr.get())) { 
			if (wall->breakable) {
				wall->TakeDamage(wall->maxHealth * 2);
			}
		}
		AddEntPtrToSector(entPtr);
	}


	if (sectEndObj) {
		sectEndObj->Unlock();
	}
	AddRedFilter();
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
	float horCenter = sectorWidth * (1.0f / 2.0f);
	float vertCenter = sectorHeight / 2.0;

	std::shared_ptr<sf::Shape> wallDrawPtr = std::make_shared<sf::RectangleShape>(sf::Vector2f(WORLD_SIZE_WINDOW_WIDTH * 2, WORLD_SIZE_WINDOW_HEIGHT * 2));

	wallDrawPtr->setFillColor(colPalA);
	wallDrawPtr->setOrigin(sf::Vector2f(WORLD_SIZE_WINDOW_WIDTH * 0.5f, WORLD_SIZE_WINDOW_HEIGHT * 0.5f));
	std::shared_ptr<Entity> extraWall = std::make_shared<Scenery>(Vector2f(sectorWidth * 0.5f, sectorHeight * 0.5f),
		wallDrawPtr, 0, RigidBody(std::make_shared<Circle>(1.0f)), "tile094", 0.5f, 0.5f);
	lvlEntitiesPhase1.push_back(extraWall);

	sf::Color floorColor(sf::Color(125, 125, 125));
	std::shared_ptr<sf::Shape> drawblPtr = std::make_shared<sf::RectangleShape>(sf::Vector2f(sectorWidth, sectorHeight));
	drawblPtr->setFillColor(floorColor);
	std::shared_ptr<Entity> backgroundColor = std::make_shared<Scenery>(Vector2f(0.0f, 0.0f), drawblPtr, 
		0, RigidBody(std::make_shared<Circle>(1.0f)), "tile117", 0.5f, 0.5f);
	lvlEntitiesPhase1.push_back(backgroundColor);

	int rightAreaLeftBound = sectorWidth * (8.5f / 10.0f);
	int rightAreaRightBound = sectorWidth * (10.0f / 10.0f);
	int leftAreaLeftBound = 0.0f;
	int leftAreaRightBound = sectorWidth * (1.5f / 10.0f);
	int topAreaTopBound = 0.0f;
	int topAreaBottomBound = sectorHeight * (1.5f / 10.0f);
	int  lowAreaLowBound = sectorHeight * (10.0f / 10.0f);
	int lowAreaTopBound = sectorHeight * (8.5f / 10.0f);

	// OFFBLACK0, OFFBLACK1, 
	std::vector<sf::Color> randColors = {OFFBLACK2, OFFBLACK3, OFFBLACK4, OFFBLACK5, OFFBLACK6, OFFBLACK7, OFFBLACK8, OFFBLACK9};
	std::random_device rd1;
	std::mt19937 gen1(rd1());
	std::uniform_int_distribution<> distrib(0, 3);
	std::uniform_int_distribution<> distribColor(0, randColors.size()-1);

	//this draws the lines before I had textures

	//for (int i = 0; i < 4; i++) {
	//	int randLine = distrib(gen1);
	//	sf::Color randColor = randColors[distribColor(gen1)];
	//	switch (randLine) {
	//	case 0: {//left side
	//		std::uniform_int_distribution<> distribTemp(leftAreaLeftBound, leftAreaRightBound);
	//		float xPos = distribTemp(gen1);
	//		std::shared_ptr<sf::Shape> lineDrawbPtr = std::make_shared<sf::RectangleShape>(sf::Vector2f(2.0f, sectorHeight));
	//		lineDrawbPtr->setFillColor(randColor);
	//		lineDrawbPtr->setOutlineColor(randColor);
	//		lineDrawbPtr->setPosition(sf::Vector2f(xPos, 0));
	//		std::shared_ptr<Entity> line = std::make_shared<Scenery>(Vector2f(xPos, 0), lineDrawbPtr);
	//		lvlEntitiesPhase1.push_back(line);
	//		break;
	//	}
	//	case 1: {//top side
	//		std::uniform_int_distribution<> distribTemp(topAreaTopBound, topAreaBottomBound);
	//		float yPos = distribTemp(gen1);
	//		std::shared_ptr<sf::Shape> lineDrawbPtr = std::make_shared<sf::RectangleShape>(sf::Vector2f(sectorWidth, 2.0f));
	//		lineDrawbPtr->setFillColor(randColor);
	//		lineDrawbPtr->setOutlineColor(randColor);
	//		lineDrawbPtr->setPosition(sf::Vector2f(0, yPos));
	//		std::shared_ptr<Entity> line = std::make_shared<Scenery>(Vector2f(0, yPos), lineDrawbPtr);
	//		lvlEntitiesPhase1.push_back(line);
	//		break;
	//	}
	//	case 2: {//right side
	//		std::uniform_int_distribution<> distribTemp(rightAreaLeftBound, rightAreaRightBound);
	//		float xPos = distribTemp(gen1);
	//		std::shared_ptr<sf::Shape> lineDrawbPtr = std::make_shared<sf::RectangleShape>(sf::Vector2f(2.0f, sectorHeight));
	//		lineDrawbPtr->setFillColor(randColor);
	//		lineDrawbPtr->setOutlineColor(randColor);
	//		lineDrawbPtr->setPosition(sf::Vector2f(xPos, 0));
	//		std::shared_ptr<Entity> line = std::make_shared<Scenery>(Vector2f(xPos, 0), lineDrawbPtr);
	//		lvlEntitiesPhase1.push_back(line);
	//		break;
	//	}
	//	case 3: {//bottom side
	//		std::uniform_int_distribution<> distribTemp(lowAreaTopBound, lowAreaLowBound);
	//		float yPos = distribTemp(gen1);
	//		std::shared_ptr<sf::Shape> lineDrawbPtr = std::make_shared<sf::RectangleShape>(sf::Vector2f(sectorWidth, 2.0f));
	//		lineDrawbPtr->setFillColor(randColor);
	//		lineDrawbPtr->setOutlineColor(randColor);
	//		lineDrawbPtr->setPosition(sf::Vector2f(0, yPos));
	//		std::shared_ptr<Entity> line = std::make_shared<Scenery>(Vector2f(0, yPos), lineDrawbPtr);
	//		lvlEntitiesPhase1.push_back(line);
	//		break;
	//	}
	//	}
	//}


}

void Sector::AddRedFilter()
{
	std::shared_ptr<sf::Shape> redFilter = std::make_shared<sf::RectangleShape>(sf::Vector2f(WORLD_SIZE_WINDOW_WIDTH * 2, WORLD_SIZE_WINDOW_HEIGHT * 2));
	redFilter->setFillColor(sf::Color(254, 0, 0, 30));
	redFilter->setOrigin(sf::Vector2f(WORLD_SIZE_WINDOW_WIDTH * 0.5f, WORLD_SIZE_WINDOW_HEIGHT * 0.5f));
	std::shared_ptr<Entity> redFilterScenery = std::make_shared<Scenery>(Vector2f(sectorWidth * 0.5f, sectorHeight * 0.5f),
		redFilter, 2, RigidBody(std::make_shared<Circle>(1.0f)), "", 0.5f, 0.5f);
	lvlEntitiesPhase1.push_back(redFilterScenery);
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

void Sector::GenerateIrregularTerrain(int i_numVerts, int i_maxSize, int i_minSize, Vector2f originCoord) //will always be in phase 2 TODO if I need them in phase one for some reason
{
	Vector2f spawnPos;
	if ((originCoord[0] == -100.0f) && (originCoord[1] == -100.0f)) {

		std::random_device rd;
		std::mt19937 gen1(rd());
		std::mt19937 gen2(rd());
		std::uniform_int_distribution<> centerDist(0, 2);
		int center = centerDist(gen1);
		std::vector<std::tuple<Vector2f, Vector2f>> screenAreas;
		if (center == 0) {
			screenAreas = GetScreenAreas(CENTER);
		}
		else {
			screenAreas = GetScreenAreas(MARGINS);
			i_minSize += 2000;
		}
		//else {
		//	screenAreas = GetScreenAreas(CORNERS);
		//}

		//int leftX = (int)std::get<0>(screenAreas[0])[0];
		//int rightX = (int)std::get<1>(screenAreas[0])[0];
		//int topY = (int)std::get<0>(screenAreas[0])[1];
		//int bottomY = (int)std::get<1>(screenAreas[0])[1];
		//std::uniform_int_distribution<> xCordDist(leftX, rightX);
		//std::uniform_int_distribution<> yCordDist(topY, bottomY);
		//int randXcordInBounds = xCordDist(gen1);
		//int randYcordInBounds = yCordDist(gen1);
		//spawnPos = Vector2f(randXcordInBounds, randYcordInBounds);
		spawnPos = Math::GetRandomCoordInRect(std::get<0>(screenAreas[0]), std::get<1>(screenAreas[0]));
	}
	else {
		spawnPos = originCoord;
	}
	std::shared_ptr<Shape> vertRect1 = Physics::CreateIrregularPolygon(i_numVerts, i_maxSize, i_minSize);
	RigidBody rightWallBody = RigidBody(vertRect1, STATIC);
	std::shared_ptr<Entity> rightWall = std::make_shared<Wall>(
		spawnPos, rightWallBody, colPalA, colPalB, true);
	lvlEntitiesPhase2.push_back(rightWall);
}

void Sector::UnlockRoom()
{
	auto entPointer = GLBVRS::PPTR;
	auto playPtr = dynamic_cast<PlayerChar*>(entPointer.get());
	playPtr->lastAOEFired = std::chrono::high_resolution_clock::now() - std::chrono::minutes(1);
	PlaySound(RESOURCES::SOUNDS::MENUACCEPT2);
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
	
	float crnrWdth = (sectorWidth * (1.0f / 8.0f));
	float crnrHght = (sectorHeight * (1.0f / 8.0f));
	Vector2f tpLftCrn = Vector2f(0.0f, 0.0f);
	Vector2f tpRghtCrn = Vector2f(sectorWidth, 0.0f);
	Vector2f bttmRghtCrn = Vector2f(sectorWidth, sectorHeight);
	Vector2f bttmLftCrn = Vector2f(0.0f, sectorHeight);

	float twoFifthsWidth = (sectorWidth * (2.0f / 5.0f));
	float threeFifthsWidth = (sectorWidth * (3.0f / 5.0f));
	float twoFifthsHeight = (sectorHeight * (2.0f / 5.0f));
	float threeFifthsHeight = (sectorHeight * (3.0f / 5.0f));


	float doorOffsetX = (sectorWidth * (1.0f / 10.0f));
	float doorOffsetY = (sectorHeight * (1.0f / 10.0f));



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
