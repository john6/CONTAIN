#include "Level.h"

struct MapNode {
	MapNode() : isRoom{ false }, up{ true }, down{ true }, left{ true }, right{ true },
		directionDoors{ {false, false, false, false} }, vectIndex{ -1 } {}

	bool isRoom;
	bool up;
	bool down;
	bool left;
	bool right;
	std::vector<bool>directionDoors;
	int vectIndex;
};

Level::Level(int i_lvlNum, DIFFICULTY i_diff, std::shared_ptr<Entity> i_charPtr, RESOURCES* i_resources) :
	charPtr { i_charPtr }, resources { i_resources }, m_lvl_num {i_lvlNum}
{
	//settings
	colPalA = GenerateRandomColor();
	colPalB = GenerateRandomColor();
	phaseOne = true;
	timeToComplete = 25.0f + (i_lvlNum * 5) - (i_diff * 5);

	GenerateMapGrip(i_lvlNum, i_diff);
	FillMapWithRooms(i_lvlNum, i_diff);
	PopulateMapRooms(i_lvlNum, i_diff);
}

Level::Level(std::string i_testStr, std::shared_ptr<Entity> i_charPtr, RESOURCES* i_resources) : charPtr{ i_charPtr }, resources{ i_resources }
{
	dimSize = 1;
	numSectors = 1;

	sectorMap = std::vector<std::vector<MapNode>>(dimSize, std::vector<MapNode>(dimSize, MapNode()));
	originCoord = MapCoord(0, 0);
	CreateSectorAtCoord(originCoord);
	GetSector(originCoord)->AddPainfullWallsToLevel();
	//GetSector(originCoord)->GenerateLevelCircles(1);
	//GetSector(originCoord)->GenerateLevelCubes(9);
	//GetSector(originCoord)->AddRandomPainWall(0);
	//GetSector(originCoord)->AddRandomPainWall(1);
	//GetSector(originCoord)->AddRandomPainWall(2);
	//GetSector(originCoord)->AddRandomPainWall(3);
	//GetSector(originCoord)->AddTerrain(0);
	//GetSector(originCoord)->AddTerrain(1);
	//GetSector(originCoord)->AddTerrain(2);
	//GetSector(originCoord)->AddTerrain(3);
	//GetSector(originCoord)->AddTerrain(4);
	//GetSector(originCoord)->AddTerrain(5);
	//GetSector(originCoord)->AddTerrain(6);
	//GetSector(originCoord)->AddTerrain(7);
}


Level::Level(std::shared_ptr<Entity> i_charPtr, RESOURCES * i_resources) : //TUTORIAL LEVEL
	charPtr{ i_charPtr }, resources{ i_resources }, m_lvl_num{ 0 }
{
	//init globals
	colPalA = GenerateRandomColor();
	colPalB = GenerateRandomColor();
	dimSize = 6;
	numSectors = 6;
	phaseOne = true;
	timeToComplete = 999.0f;
	sectorMap = std::vector<std::vector<MapNode>>(dimSize, std::vector<MapNode>(dimSize, MapNode()));

	//init map coords
	originCoord = MapCoord(0, 0);
	CreateSectorAtCoord(originCoord);
	MapCoord room2;
	room2 = MapCoord(1, 0);
	CreateSectorAtCoord(room2);
	MapCoord room3;
	room3 = MapCoord(2, 0);
	CreateSectorAtCoord(room3);
	MapCoord room4;
	room4 = MapCoord(3, 0);
	CreateSectorAtCoord(room4);
	MapCoord room5;
	room5 = MapCoord(4, 0);
	CreateSectorAtCoord(room5);
	bossRoom = MapCoord(5, 0);
	CreateSectorAtCoord(bossRoom);

	//init doors
	CreateBidirectionalDoor(originCoord, room2);
	CreateBidirectionalDoor(room2, room3);
	CreateBidirectionalDoor(room3, room4);
	CreateBidirectionalDoor(room4, room5);
	CreateBidirectionalDoor(room5, bossRoom);

	//ROOM 1 ENTRANCE
	GetSector(originCoord)->PopulateEntranceRoom();
	
	//ROOM 2 LEARN TO SHOOT
	GetSector(room2)->GenerateEnemies(1, ENEMY_SEEK, MARGINS, 1, (DIFFICULTY)0, 2);
	//GetSector(room2)->GenerateEnemies(numRandosPhase1, ENEMY_RAND, CENTER, 1, (DIFFICULTY)i_diff, 0);
	GetSector(room2)->GenerateEnemies(3, ENEMY_SEEK_PUSH, CENTER, 2, (DIFFICULTY)0, 0);
	GetSector(room2)->GenerateEnemies(3, ENEMY_RAND_PUSH, CORNERS, 2, (DIFFICULTY)0, 0);
	GetSector(room2)->AddTerrain(0, false);
	
	//ROOM 3 LEARN TO USE WALL
	//GetSector(room3)->GenerateEnemies(1, ENEMY_SEEK, MARGINS, 1, (DIFFICULTY)0, 1);
	GetSector(room3)->GenerateEnemies(2, ENEMY_SEEK, MARGINS, 1, (DIFFICULTY)0, 1);
	//GetSector(room2)->GenerateEnemies(numRandosPhase1, ENEMY_RAND, CENTER, 1, (DIFFICULTY)i_diff, 0);
	GetSector(room3)->GenerateEnemies(3, ENEMY_SEEK_PUSH, CENTER, 2, (DIFFICULTY)0, 0);
	GetSector(room3)->GenerateEnemies(3, ENEMY_RAND_PUSH, CORNERS, 2, (DIFFICULTY)0, 0);

	//ROOM 4 LEARN TO USE EMP 
	GetSector(room4)->GenerateEnemies(3, ENEMY_SEEK, MARGINS, 1, (DIFFICULTY)0, 2);
	//GetSector(room4)->GenerateEnemies(1, ENEMY_RAND, MARGINS, 1, (DIFFICULTY)0, 1);
	//GetSector(room2)->GenerateEnemies(numRandosPhase1, ENEMY_RAND, CENTER, 1, (DIFFICULTY)i_diff, 0);
	GetSector(room4)->GenerateEnemies(3, ENEMY_SEEK_PUSH, CENTER, 2, (DIFFICULTY)0, 0);
	GetSector(room4)->GenerateEnemies(3, ENEMY_RAND_PUSH, CORNERS, 2, (DIFFICULTY)0, 0);

	//ROOM 5 LEARN ABOUT PAIN WALLS
	//GetSector(room4)->GenerateEnemies(4, ENEMY_SEEK, MARGINS, 1, (DIFFICULTY)0, 1);
	GetSector(room5)->GenerateEnemies(4, ENEMY_RAND, MARGINS, 1, (DIFFICULTY)0, 1);
	//GetSector(room2)->GenerateEnemies(numRandosPhase1, ENEMY_RAND, CENTER, 1, (DIFFICULTY)i_diff, 0);
	GetSector(room5)->GenerateEnemies(3, ENEMY_SEEK_PUSH, CENTER, 2, (DIFFICULTY)0, 0);
	GetSector(room5)->GenerateEnemies(3, ENEMY_RAND_PUSH, CORNERS, 2, (DIFFICULTY)0, 0);
	GetSector(room5)->AddRandomPainWall(0);
	GetSector(room5)->AddRandomPainWall(1);
	GetSector(room5)->AddRandomPainWall(4);
	GetSector(room5)->AddRandomPainWall(5);
	
	//5 BOSS ROOM
	GetSector(bossRoom)->PopulateBossRoom("TUTORIAL");

}

Level::~Level()
{
}

std::shared_ptr<Sector> Level::GetSector(MapCoord i_coord)
{ 
	return sectorVect[sectorMap[i_coord.x][i_coord.y].vectIndex];
}

void Level::CreateSectorAtCoord(MapCoord i_coord)
{
	std::shared_ptr<Sector> newSector = std::make_shared<Sector>(this, resources, colPalA, colPalB);
	sectorVect.push_back(newSector);
	sectorMap[i_coord.x][i_coord.y].vectIndex = sectorVect.size() - 1;
	sectorMap[i_coord.x][i_coord.y].isRoom = true;
	allCoords.push_back(i_coord);
}

void Level::PopulateSectorAtCoord(MapCoord i_coord, int i_diff)
{
	if (GetSector(i_coord)->filledIn == true) { return; } //I have no idea why, but some sectors are getting filled in twice, this should prevent that

	std::random_device rd1;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen1(rd1()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> distrib(0, 3); //both boundaries are inclusive
	int randExtra = distrib(gen1);

	std::random_device rdRoomSeed;
	std::mt19937 genRoomSeed(rdRoomSeed());
	std::discrete_distribution<> extraRoomDist({ 40, 40, 15, 5 });
	int randExtraRooms = extraRoomDist(genRoomSeed);
	int numCubes = 1 + i_diff + randExtraRooms;
	//phase one
	int numSeekersPhase1 = 2 + (i_diff * 1) + ((m_lvl_num+1)/2);
	int numRandosPhase1 = (i_diff * 1) + ((m_lvl_num+1)/2);
	GetSector(i_coord)->GenerateEnemies(numSeekersPhase1, ENEMY_SEEK, MARGINS, 1, (DIFFICULTY)i_diff, 0);
	GetSector(i_coord)->GenerateEnemies(numRandosPhase1, ENEMY_RAND, CENTER, 1, (DIFFICULTY)i_diff, 0);
	//phase two
	int numSeekersPhase2 = 4 + (i_diff * 1) + (m_lvl_num * 1);
	int numRandosPhase2 = 6 + (i_diff * 1) + (m_lvl_num * 1);
	GetSector(i_coord)->GenerateEnemies(numSeekersPhase2, ENEMY_SEEK_PUSH, DOORS, 2, (DIFFICULTY)i_diff, 0);
	GetSector(i_coord)->GenerateEnemies(numRandosPhase2, ENEMY_RAND_PUSH, CORNERS, 2, (DIFFICULTY)i_diff, 0);


	std::discrete_distribution<> smallTerrainDist({ 40, 50, 5, 5 });
	std::discrete_distribution<> BigTerrainDist({ 85, 10, 5 });
	int smallTerrainCount = smallTerrainDist(genRoomSeed);
	int bigTerrainCount = BigTerrainDist(genRoomSeed);
	if (i_diff == HARD) {
		--smallTerrainCount;
		--bigTerrainCount;
	}
	while (smallTerrainCount > 0) {
		//int rando = distrib(gen1);
		GetSector(i_coord)->AddTerrain(-1, false);
		--smallTerrainCount;
	}
	//bigTerrainCount = 6;
	while (bigTerrainCount > 0) {
		//int rando = distrib(gen1);
		GetSector(i_coord)->AddTerrain(-1, true);
		--bigTerrainCount;
	}

	std::random_device rdRoomSeed4;
	std::mt19937 painWallSeed(rdRoomSeed4());
	std::discrete_distribution<> painWallDist({ 50, 50});
	int painWallCount = painWallDist(painWallSeed);
	std::random_device rd6;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen6(rd6()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> distrib07(0, 7); //both boundaries are inclusive

	while (painWallCount > 0) {
		int randPainWall = distrib07(gen6);
		GetSector(i_coord)->AddRandomPainWall(randPainWall);
		--painWallCount;
	}
	//newSector->GenerateLevelCircles(1);

	GetSector(i_coord)->filledIn = true;
}

void Level::CreateOneWayDoor(MapCoord i_CoordA, MapCoord i_CoordB)
{
}

void Level::CreateBidirectionalDoor(MapCoord i_coordA, MapCoord i_coordB)
{
	float doorOutPadding = 150.0f;
	float horizontalMiddle = GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH / 2.0f);
	float verticleMiddle = GLBVRS::VRT_MRG + (GLBVRS::CRT_HGHT / 2.0f);

	int yDist = i_coordB.y - i_coordA.y;
	int xDist = i_coordB.x - i_coordA.x;

	std::shared_ptr<Shape>doorShapeA;
	std::shared_ptr<Shape>doorShapeB;
	Vector2f startPosA;
	Vector2f startPosB;
	Vector2f outPosA;
	Vector2f outPosB;

	SCREEN_SIDE inSide;
	SCREEN_SIDE outSide;

	if (yDist == 1) { //UP
		inSide = SCREEN_UP;
		outSide = SCREEN_DOWN;
		doorShapeA = std::make_shared<Rectangle>(DOOR_WIDTH, DOOR_HEIGHT);
		doorShapeB = std::make_shared<Rectangle>(DOOR_WIDTH, DOOR_HEIGHT);
		startPosA = Vector2f(horizontalMiddle, GLBVRS::VRT_MRG  + GLBVRS::CRT_HGHT + (DOOR_HEIGHT * ( 1.0f / 2.0f)));
		startPosB = Vector2f(horizontalMiddle, GLBVRS::VRT_MRG - (DOOR_HEIGHT * (1.0f / 2.0f)));
		outPosA = Vector2f(horizontalMiddle, GLBVRS::VRT_MRG + doorOutPadding);
		outPosB = Vector2f(horizontalMiddle, GLBVRS::VRT_MRG + GLBVRS::CRT_HGHT - doorOutPadding);
	}
	else if (yDist == -1) { //DOWN
		inSide = SCREEN_DOWN;
		outSide = SCREEN_UP;
		doorShapeA = std::make_shared<Rectangle>(DOOR_WIDTH, DOOR_HEIGHT);
		doorShapeB = std::make_shared<Rectangle>(DOOR_WIDTH, DOOR_HEIGHT);
		startPosA = Vector2f(horizontalMiddle, GLBVRS::VRT_MRG - (DOOR_HEIGHT * (1.0f / 2.0f)));
		startPosB = Vector2f(horizontalMiddle, GLBVRS::VRT_MRG + GLBVRS::CRT_HGHT + (DOOR_HEIGHT * (1.0f / 2.0f)));
		outPosA = Vector2f(horizontalMiddle, GLBVRS::VRT_MRG + GLBVRS::CRT_HGHT - doorOutPadding);
		outPosB = Vector2f(horizontalMiddle, GLBVRS::VRT_MRG + doorOutPadding);
	}
	else if (xDist == -1) { //LEFT
		inSide = SCREEN_LEFT;
		outSide = SCREEN_RIGHT;
		doorShapeA = std::make_shared<Rectangle>(DOOR_HEIGHT, DOOR_WIDTH);
		doorShapeB = std::make_shared<Rectangle>(DOOR_HEIGHT, DOOR_WIDTH);
		startPosA = Vector2f(GLBVRS::HR_MRG - (DOOR_HEIGHT * (1.0f / 2.0f)), verticleMiddle);
		startPosB = Vector2f(GLBVRS::HR_MRG + GLBVRS::CRT_WDTH + (DOOR_HEIGHT * (1.0f / 2.0f)), verticleMiddle);
		outPosA = Vector2f(GLBVRS::CRT_WDTH + GLBVRS::HR_MRG - doorOutPadding, verticleMiddle);
		outPosB = Vector2f(GLBVRS::HR_MRG + doorOutPadding, verticleMiddle);
	}
	else if (xDist == 1) { //RIGHT
		inSide = SCREEN_RIGHT;
		outSide = SCREEN_LEFT;
		doorShapeA = std::make_shared<Rectangle>(DOOR_HEIGHT, DOOR_WIDTH);
		doorShapeB = std::make_shared<Rectangle>(DOOR_HEIGHT, DOOR_WIDTH);
		startPosA = Vector2f(GLBVRS::HR_MRG + GLBVRS::CRT_WDTH + (DOOR_HEIGHT * (1.0f / 2.0f)), verticleMiddle);
		startPosB = Vector2f(GLBVRS::HR_MRG - (DOOR_HEIGHT * (1.0f / 2.0f)), verticleMiddle);
		outPosA = Vector2f(GLBVRS::HR_MRG + doorOutPadding, verticleMiddle);
		outPosB = Vector2f(GLBVRS::CRT_WDTH + GLBVRS::HR_MRG - doorOutPadding, verticleMiddle);
	}
	else {
		//wtf these rooms arent adjacent
	}
	RigidBody doorBodyA(doorShapeA);
	RigidBody doorBodyB(doorShapeB);
	std::shared_ptr<Door> sectDoorA = std::make_shared<Door>(GetSector(i_coordA).get(), i_coordB, startPosA,outPosA, doorBodyA, inSide);
	std::shared_ptr<Door> sectDoorB = std::make_shared<Door>(GetSector(i_coordB).get(), i_coordA, startPosB,outPosB, doorBodyB, outSide);
	GetSector(i_coordA)->AddEntPtrToSector(sectDoorA);
	GetSector(i_coordB)->AddEntPtrToSector(sectDoorB);
}

float Level::GetTimeLeftInLevel()
{
	if (phaseOne) {
		return timeToComplete;
	}
	else {
		timeElapsed = std::chrono::duration_cast<std::chrono::seconds>(hiResTime::now() - beginTime);
		float timeElapsedCount = timeElapsed.count();
		timeElapsedCount = timeElapsedCount / 1000000;
		float timeLeft = timeToComplete - timeElapsedCount;
		return timeLeft;
	}
}

void Level::SwitchSectorsToPhaseTwo()
{
	phaseOne = false;
	beginTime = hiResTime::now();
	for (auto sectorPtr : sectorVect) {
		sectorPtr->SwitchToPhaseTwo();
	}
}

sf::Color Level::GenerateRandomColor()
{
	std::random_device rd1;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen1(rd1()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> distrib(1, 128); //both boundaries are inclusive
	int randCol1 = distrib(gen1);
	int randCol2 = distrib(gen1);
	int randCol3 = distrib(gen1);

	//std::random_device rdRoomSeed;  //Will be used to obtain a seed for the random number engine
	//std::mt19937 genRoomSeed(rdRoomSeed()); //Standard mersenne_twister_engine seeded with rd()
	//std::discrete_distribution<> extraRoomDist({ 25, 50, 25 });
	//int randExtraRooms = extraRoomDist(genRoomSeed);
	return sf::Color(randCol1, randCol2, randCol3);
}

void Level::GenerateMapGrip(int i_lvlNum, DIFFICULTY i_diff)
{
	//decide num rooms, create MapGrid
	std::random_device rdRoomSeed;  //Will be used to obtain a seed for the random number engine
	std::mt19937 genRoomSeed(rdRoomSeed()); //Standard mersenne_twister_engine seeded with rd()
	std::discrete_distribution<> extraRoomDist({ 13, 25, 25, 25, 12 });
	int randExtraRooms = extraRoomDist(genRoomSeed);
	numSectors = 2 + (i_lvlNum * 2) + (i_diff * 2) + randExtraRooms;
	dimSize = (int)std::sqrt(numSectors*2);
	if (dimSize % 2 == 0) { //Gonna make the dimensions always be odd so I can have a center coord
		dimSize += 1;
	}
	//initialize map and origin location
	sectorMap = std::vector<std::vector<MapNode>>(dimSize, std::vector<MapNode>(dimSize, MapNode()));
	originCoord = MapCoord((dimSize / 2) + 1, (dimSize / 2) + 1);
	CreateSectorAtCoord(originCoord);
	allCoords.push_back(originCoord);
}

void Level::ChooseBossRoom()
{
	bossRoom = allCoords[allCoords.size() - 1];
}

void Level::FillMapWithRooms(int i_levelNum, DIFFICULTY i_diff)
{
	std::queue<MapCoord> fringe;
	fringe.push(originCoord);
	int currNumRooms = 0;
	int numRoomsTotal = numSectors;
	std::random_device rd2;
	std::mt19937 gen2(rd2());
	std::discrete_distribution<> discreteDist({ 0, 75, 20, 5 }); //percent chance a room will spawn what number of doors

	while ((!fringe.empty()) && (currNumRooms < numRoomsTotal)) {
		//get coord from fringe, check which directions are possible to build
		MapCoord currCoord = fringe.front();
		fringe.pop();
		if (currCoord.y + 1 >= dimSize) {//UP
			sectorMap[currCoord.x][currCoord.y].up = false;
		}
		else if (sectorMap[currCoord.x][currCoord.y + 1].isRoom) {
			sectorMap[currCoord.x][currCoord.y].up = false;
		}
		if (currCoord.y - 1 < 0) {//DOWN
			sectorMap[currCoord.x][currCoord.y].down = false;
		}
		else if (sectorMap[currCoord.x][currCoord.y - 1].isRoom) {
			sectorMap[currCoord.x][currCoord.y].down = false;
		}
		if (currCoord.x - 1 < 0) {//LEFT
			sectorMap[currCoord.x][currCoord.y].left = false;
		}
		else if (sectorMap[currCoord.x - 1][currCoord.y].isRoom) {
			sectorMap[currCoord.x][currCoord.y].left = false;
		}
		if (currCoord.x + 1 >= dimSize) {//RIGHT
			sectorMap[currCoord.x][currCoord.y].right = false;
		}
		else if (sectorMap[currCoord.x + 1][currCoord.y].isRoom) {
			sectorMap[currCoord.x][currCoord.y].right = false;
		}
		//generate number indicating number of doors to make
		int numDoorsToPlace = discreteDist(gen2);
		//randomly select which available directions to create door in 
		std::vector<int> doorsAvail;
		if (sectorMap[currCoord.x][currCoord.y].up) { doorsAvail.push_back(0); }
		if (sectorMap[currCoord.x][currCoord.y].down) { doorsAvail.push_back(1); }
		if (sectorMap[currCoord.x][currCoord.y].left) { doorsAvail.push_back(2); }
		if (sectorMap[currCoord.x][currCoord.y].right) { doorsAvail.push_back(3); }
		//create new sectos where doors intend to connect, create doors connecting sectors, add MapCoords to fringe and increment num rooms
		std::random_device rd3;
		std::mt19937 gen3(rd3());
		int currDoor = 0;
		while (currDoor < numDoorsToPlace) {
			MapCoord newCoord;
			if (doorsAvail.size() > 0) {
				std::uniform_int_distribution<> evenDistrib(0, doorsAvail.size() - 1);
				int doorDir = doorsAvail[evenDistrib(gen3)];
				switch (doorDir) {
				case (0): {//UP
					newCoord = MapCoord(currCoord.x, currCoord.y + 1);
					break;
				}
				case (1): {//DOWN
					newCoord = MapCoord(currCoord.x, currCoord.y - 1);
					break;
				}
				case (2): {//LEFT
					newCoord = MapCoord(currCoord.x - 1, currCoord.y);
					break;
				}
				case (3): {//RIGHT
					newCoord = MapCoord(currCoord.x + 1, currCoord.y);
					break;
				}
				}
				CreateSectorAtCoord(newCoord);
				CreateBidirectionalDoor(currCoord, newCoord);
				fringe.push(newCoord);
				++currDoor;
				++currNumRooms;
			}
			else {
				break;
			}
		}
	}
	ChooseBossRoom();
}

void Level::PopulateMapRooms(int i_levelNum, DIFFICULTY i_diff)
{
	GetSector(originCoord)->PopulateEntranceRoom();
	for (MapCoord coord : allCoords) { //Populate all sectors except first
		if (((coord.x != originCoord.x) || (coord.y != originCoord.y)) &&
			((coord.x != bossRoom.x) || (coord.y != bossRoom.y))) {
			PopulateSectorAtCoord(coord, i_diff);
		}
	}
	GetSector(bossRoom)->PopulateBossRoom(m_lvl_num, i_diff);
}

void Level::UpdateLevel()
{
	if (!phaseOne) {
		timeElapsed = std::chrono::duration_cast<std::chrono::seconds>(hiResTime::now() - beginTime);
		float timeElapsedCount = timeElapsed.count();
		timeElapsedCount = timeElapsedCount / 1000000;
		float timeLeft = timeToComplete - timeElapsedCount;
		if (timeLeft == 0) {
			resources->PlaySound(RESOURCES::WARNING3);
		}
		if (timeLeft <= 0) {
			dynamic_cast<PlayerChar*>(charPtr.get())->TakeDamage(1);
		}
	}
}
