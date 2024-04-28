#include "Level.h"
#include "PowerUp.h"

struct MapNode {
	MapNode() : isRoom{ false }, up{ true }, down{ true }, left{ true }, right{ true },
		directionDoors{ {false, false, false, false} }, vectIndex{ -1 } {}

	bool isRoom;
	bool up;
	bool down;
	bool left;
	bool right;
	Vector2f dims;
	std::vector<bool>directionDoors;
	int vectIndex;
};

Level::Level(int i_lvlNum, DIFFICULTY i_diff) :
	m_lvl_num {i_lvlNum}
{
	resources = GLBVRS::RSRCS;
	charPtr = GLBVRS::PPTR;
	//settings
	colPalA = Physics::GenerateRandomColor(40, 128);
	colPalB = Physics::GenerateRandomColor(40, 128);
	phaseOne = true;
	timeToComplete = 29.0f + (i_lvlNum * 5) - (i_diff * 5);
	enemiesKilled = 0;
	//baseLevelWidth = MAXSECTORWIDTH;
	//baseLevelHeight = MAXSECTORHEIGHT;
	baseLevelWidth = (MAX_SECTOR_WIDTH + MIN_SECTOR_WIDTH) / 2;
	baseLevelHeight = (MAX_SECTOR_HEIGHT + MIN_SECTOR_HEIGHT) / 2;

	GenerateMapGrip(i_lvlNum, i_diff);
	FillMapWithRooms(i_lvlNum, i_diff);
	PopulateMapRooms(i_lvlNum, i_diff);
}

Level::Level(std::string i_testStr)
{//TEST LEVEL
	resources = GLBVRS::RSRCS;
	charPtr = GLBVRS::PPTR;
	//settings
	colPalA = Physics::GenerateRandomColor(5, 128);
	colPalB = Physics::GenerateRandomColor(5, 128);
	phaseOne = true;
	timeToComplete = 50;
	enemiesKilled = 0;
	dimSize = 1;
	numSectors = 1;
	sectorMap = std::vector<std::vector<MapNode>>(dimSize, std::vector<MapNode>(dimSize, MapNode()));
	originCoord = MapCoord(0, 0);
	//CreateSectorAtCoord(originCoord);
	std::shared_ptr<Sector> newSector = std::make_shared<Sector>(this, 1728, 972, resources, colPalA, colPalB, true);
	sectorVect.push_back(newSector);
	sectorMap[originCoord.x][originCoord.y].vectIndex = sectorVect.size() - 1;
	sectorMap[originCoord.x][originCoord.y].isRoom = true;
	allCoords.push_back(originCoord);
	//
	//GetSector(originCoord)->GenerateEnemies(25, ENEMY_SEEK_PUSH, CENTER, 1, HARD, 0);
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


Level::Level() : //TUTORIAL LEVEL
 m_lvl_num{ 0 }
{
	resources = GLBVRS::RSRCS;
	charPtr = GLBVRS::PPTR;


	baseLevelWidth = MAX_SECTOR_WIDTH;
	baseLevelHeight = MAX_SECTOR_HEIGHT;
	//init globals
	colPalA = Physics::GenerateRandomColor(5, 128);
	colPalB = Physics::GenerateRandomColor(5, 128);
	dimSize = 6;
	numSectors = 6;
	phaseOne = true;
	timeToComplete = 999.0f;
	sectorMap = std::vector<std::vector<MapNode>>(dimSize, std::vector<MapNode>(dimSize, MapNode()));
	enemiesKilled = 0;
	//init map coords
	originCoord = MapCoord(0, 0);
	sectorMap[0][0].dims[0] = 1920;
	sectorMap[0][0].dims[1] = 1080;
	CreateSectorAtCoord(originCoord);
	MapCoord room2;
	room2 = MapCoord(1, 0);
	sectorMap[1][0].dims[0] = 1920;
	sectorMap[1][0].dims[1] = 1080;
	CreateSectorAtCoord(room2);
	MapCoord room3;
	room3 = MapCoord(2, 0);
	sectorMap[2][0].dims[0] = 1920;
	sectorMap[2][0].dims[1] = 1080;
	CreateSectorAtCoord(room3);
	MapCoord room4;
	room4 = MapCoord(3, 0);
	sectorMap[3][0].dims[0] = 1920;
	sectorMap[3][0].dims[1] = 1080;
	CreateSectorAtCoord(room4);
	MapCoord room5;
	room5 = MapCoord(4, 0);
	sectorMap[4][0].dims[0] = 1920;
	sectorMap[4][0].dims[1] = 1080;
	CreateSectorAtCoord(room5);
	bossRoom = MapCoord(5, 0);
	sectorMap[5][0].dims[0] = 1920;
	sectorMap[5][0].dims[1] = 1080;
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
	GetSector(room2)->GenerateEnemies(3, ENEMY_SEEK_PUSH, CENTER, 2, (DIFFICULTY)0, 0);
	GetSector(room2)->GenerateEnemies(3, ENEMY_RAND_PUSH, CORNERS, 2, (DIFFICULTY)0, 0);
	GetSector(room2)->AddTerrain(0, false);
	
	//ROOM 3 LEARN TO USE WALL
	GetSector(room3)->GenerateEnemies(2, ENEMY_SEEK, MARGINS, 1, (DIFFICULTY)0, 1);
	GetSector(room3)->GenerateEnemies(3, ENEMY_SEEK_PUSH, CENTER, 2, (DIFFICULTY)0, 0);
	GetSector(room3)->GenerateEnemies(3, ENEMY_RAND_PUSH, CORNERS, 2, (DIFFICULTY)0, 0);

	//ROOM 4 LEARN TO USE EMP 
	GetSector(room4)->GenerateEnemies(3, ENEMY_SEEK, MARGINS, 1, (DIFFICULTY)0, 2);
	GetSector(room4)->GenerateEnemies(3, ENEMY_SEEK_PUSH, CENTER, 2, (DIFFICULTY)0, 0);
	GetSector(room4)->GenerateEnemies(3, ENEMY_RAND_PUSH, CORNERS, 2, (DIFFICULTY)0, 0);

	//ROOM 5 LEARN ABOUT PAIN WALLS
	GetSector(room5)->GenerateEnemies(4, ENEMY_RAND, MARGINS, 1, (DIFFICULTY)0, 1);
	GetSector(room5)->GenerateEnemies(3, ENEMY_SEEK_PUSH, CENTER, 2, (DIFFICULTY)0, 0);
	GetSector(room5)->GenerateEnemies(3, ENEMY_RAND_PUSH, CORNERS, 2, (DIFFICULTY)0, 0);
	GetSector(room5)->AddPainWall(0);
	GetSector(room5)->AddPainWall(1);
	GetSector(room5)->AddPainWall(4);
	GetSector(room5)->AddPainWall(5);
	
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
	std::shared_ptr<Sector> newSector = std::make_shared<Sector>
		(this, sectorMap[i_coord.x][i_coord.y].dims[0], sectorMap[i_coord.x][i_coord.y].dims[1], resources, colPalA, colPalB);
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
	GetSector(i_coord)->GenerateEnemies(numSeekersPhase1, ENEMY_SEEK, CORNERS, 1, (DIFFICULTY)i_diff, 0);
	GetSector(i_coord)->GenerateEnemies(numRandosPhase1, ENEMY_RAND, CENTER, 1, (DIFFICULTY)i_diff, 0);
	//phase two
	int numSeekersPhase2 = 5 + (i_diff * 1) + (m_lvl_num * 1);
	int numRandosPhase2 = 6 + (i_diff * 1) + (m_lvl_num * 1);
	GetSector(i_coord)->GenerateEnemies(numSeekersPhase2, ENEMY_SEEK_PUSH, DOORS, 2, (DIFFICULTY)i_diff, 0);
	GetSector(i_coord)->GenerateEnemies(numRandosPhase2, ENEMY_RAND_PUSH, CORNERS, 2, (DIFFICULTY)i_diff, 0);

	std::random_device rdGenA;
	std::mt19937 genTerrain(rdGenA());
	std::uniform_int_distribution<> distribTerrain(0, 3);
	int makeIrregularTerrain = distribTerrain(genTerrain);
	
	std::uniform_int_distribution<> distribTerrainVerts(5, 7);
	int terrainVerts = distribTerrainVerts(genTerrain);

	std::uniform_int_distribution<> distribTerrainSize(0, 400);
	int terrainSize = distribTerrainSize(genTerrain);

	std::discrete_distribution<> smallTerrainDist({ 40, 50, 5, 5 });
	std::discrete_distribution<> BigTerrainDist({ 85, 10, 5 });
	int smallTerrainCount = smallTerrainDist(genRoomSeed);
	int bigTerrainCount = BigTerrainDist(genRoomSeed);
	if (makeIrregularTerrain == 0) {
		//if (true) {
	GetSector(i_coord)->GenerateIrregularTerrain(terrainVerts, terrainSize, 3000);
	//--bigTerrainCount;
	}
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
		std::discrete_distribution<> painWallDist({ 50, 50 });
		int painWallCount = painWallDist(painWallSeed);
		std::random_device rd6;  //Will be used to obtain a seed for the random number engine
		std::mt19937 gen6(rd6()); //Standard mersenne_twister_engine seeded with rd()
		std::uniform_int_distribution<> distrib07(0, 7); //both boundaries are inclusive

		while (painWallCount > 0) {
			int randPainWall = distrib07(gen6);
			GetSector(i_coord)->AddPainWall(randPainWall);
			--painWallCount;
		}
	GetSector(i_coord)->filledIn = true;
}

void Level::CreateOneWayDoor(MapCoord i_CoordA, MapCoord i_CoordB)
{
}

void Level::CreateBidirectionalDoor(MapCoord i_coordA, MapCoord i_coordB)
{
	float doorOutPadding = 150.0f;

	float courtWidthA = sectorMap[i_coordA.x][i_coordA.y].dims[0];
	float courtHeightA = sectorMap[i_coordA.x][i_coordA.y].dims[1];
	float courtWidthB = sectorMap[i_coordB.x][i_coordB.y].dims[0];
	float courtHeightB = sectorMap[i_coordB.x][i_coordB.y].dims[1];
	float horizontalMiddleA = courtWidthA / 2.0f;
	float horizontalMiddleB = courtWidthB / 2.0f;
	float verticleMiddleA = courtHeightA / 2.0f;
	float verticleMiddleB = courtHeightB / 2.0f;

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
		doorShapeA = std::make_shared<Rectangle>(GLBVRS::DOOR_WIDTH, GLBVRS::DOOR_HEIGHT);
		doorShapeB = std::make_shared<Rectangle>(GLBVRS::DOOR_WIDTH, GLBVRS::DOOR_HEIGHT);
		startPosA = Vector2f(horizontalMiddleA, courtHeightA + (GLBVRS::DOOR_HEIGHT * 0.5f));
		startPosB = Vector2f(horizontalMiddleB, -(GLBVRS::DOOR_HEIGHT * 0.5f));
		outPosA = Vector2f(horizontalMiddleB, doorOutPadding);
		outPosB = Vector2f(horizontalMiddleA, courtHeightA - doorOutPadding);
	}
	else if (yDist == -1) { //DOWN
		inSide = SCREEN_DOWN;
		outSide = SCREEN_UP;
		doorShapeA = std::make_shared<Rectangle>(GLBVRS::DOOR_WIDTH, GLBVRS::DOOR_HEIGHT);
		doorShapeB = std::make_shared<Rectangle>(GLBVRS::DOOR_WIDTH, GLBVRS::DOOR_HEIGHT);
		startPosA = Vector2f(horizontalMiddleA, -(GLBVRS::DOOR_HEIGHT * 0.5f));
		startPosB = Vector2f(horizontalMiddleB, courtHeightB + (GLBVRS::DOOR_HEIGHT * 0.5));
		outPosA = Vector2f(horizontalMiddleB, courtHeightB - doorOutPadding);
		outPosB = Vector2f(horizontalMiddleA, doorOutPadding);
	}
	else if (xDist == -1) { //LEFT
		inSide = SCREEN_LEFT;
		outSide = SCREEN_RIGHT;
		doorShapeA = std::make_shared<Rectangle>(GLBVRS::DOOR_HEIGHT, GLBVRS::DOOR_WIDTH);
		doorShapeB = std::make_shared<Rectangle>(GLBVRS::DOOR_HEIGHT, GLBVRS::DOOR_WIDTH);
		startPosA = Vector2f(-(GLBVRS::DOOR_HEIGHT * 0.5f), verticleMiddleA);
		startPosB = Vector2f(courtWidthB + (GLBVRS::DOOR_HEIGHT * 0.5f), verticleMiddleB);
		outPosA = Vector2f(courtWidthB - doorOutPadding, verticleMiddleB);
		outPosB = Vector2f(doorOutPadding, verticleMiddleA);
	}
	else if (xDist == 1) { //RIGHT
		inSide = SCREEN_RIGHT;
		outSide = SCREEN_LEFT;
		doorShapeA = std::make_shared<Rectangle>(GLBVRS::DOOR_HEIGHT, GLBVRS::DOOR_WIDTH);
		doorShapeB = std::make_shared<Rectangle>(GLBVRS::DOOR_HEIGHT, GLBVRS::DOOR_WIDTH);
		startPosA = Vector2f(courtWidthA + (GLBVRS::DOOR_HEIGHT * 0.5f), verticleMiddleA);
		startPosB = Vector2f(-(GLBVRS::DOOR_HEIGHT * 0.5f), verticleMiddleB);
		outPosA = Vector2f(GLBVRS::HR_MRG + doorOutPadding, verticleMiddleB);
		outPosB = Vector2f(courtWidthA - doorOutPadding, verticleMiddleA);
	}
	else {
		//wtf these rooms arent adjacent
	}
	RigidBody doorBodyA(doorShapeA);
	RigidBody doorBodyB(doorShapeB);
	std::shared_ptr<Door> sectDoorA = std::make_shared<Door>(i_coordB, startPosA, outPosA, doorBodyA, inSide);
	std::shared_ptr<Door> sectDoorB = std::make_shared<Door>(i_coordA, startPosB, outPosB, doorBodyB, outSide);
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

void Level::GenerateMapGrip(int i_lvlNum, DIFFICULTY i_diff)
{
	//decide num rooms, create MapGrid
	std::random_device rdRoomSeed;  //Will be used to obtain a seed for the random number engine
	std::mt19937 genRoomSeed(rdRoomSeed()); //Standard mersenne_twister_engine seeded with rd()
	std::discrete_distribution<> extraRoomDist({ 13, 25, 25, 25, 12 });
	int randExtraRooms = extraRoomDist(genRoomSeed);
	numSectors = 4 + (i_lvlNum * 2) + (i_diff * 2) + randExtraRooms;
	dimSize = (int)std::sqrt(numSectors*2);
	if (dimSize % 2 == 0) { //Gonna make the dimensions always be odd so I can have a center coord
		dimSize += 1;
	}

	//init sector map
	sectorMap = std::vector<std::vector<MapNode>>(dimSize, std::vector<MapNode>(dimSize, MapNode()));
	
	//set all width values
	for (int i = 0; i < dimSize; i++) {
		std::random_device rd1;
		std::mt19937 gen1(rd1());
		std::discrete_distribution<> distrib({ 1, 1, 2, 3, 6, 6, 6, 8, 8, 8, 6, 5, 3, 2, 1, 1});// [0,16]
		float ratio = (float)distrib(gen1) / 16.0f;
		int randomSectorWidth = (MIN_SECTOR_WIDTH * ratio) + (MAX_SECTOR_WIDTH * (1.0f - ratio));
			for (int j = 0; j < dimSize; j++) {
				sectorMap[j][i].dims[0] = randomSectorWidth;
			}
		}
	//set all height values
	for (int i = 0; i < dimSize; i++) {
		std::random_device rd1;
		std::mt19937 gen1(rd1());
		std::discrete_distribution<> distrib({ 1, 1, 2, 3, 6, 6, 6, 8, 8, 8, 6, 5, 3, 2, 1, 1 });// [0,16]
		float ratio = (float)distrib(gen1) / 16.0f;
		int randomSectorHeight = (MIN_SECTOR_HEIGHT * ratio) + (MAX_SECTOR_HEIGHT * (1.0f - ratio));
		int sectorHieght = randomSectorHeight;
		for (int j = 0; j < dimSize; j++) {
			sectorMap[i][j].dims[1] = sectorHieght;
		}
	}

	//init Origin sector
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
	std::discrete_distribution<> discreteDist({ 0, 70, 25, 5 }); //percent chance a room will spawn what number of doors

	while ((!fringe.empty()) && (currNumRooms < numRoomsTotal)) {
		//get coord from fringe, check which directions are possible to build
		MapCoord currCoord = fringe.front();
		fringe.pop();
		if ((currCoord.y + 1 >= dimSize) || (sectorMap[currCoord.x][currCoord.y + 1].isRoom)) {//UP
			sectorMap[currCoord.x][currCoord.y].up = false;
		}
		if ((currCoord.y - 1 < 0) || (sectorMap[currCoord.x][currCoord.y - 1].isRoom)) {//DOWN
			sectorMap[currCoord.x][currCoord.y].down = false;
		}
		if ((currCoord.x - 1 < 0) || (sectorMap[currCoord.x - 1][currCoord.y].isRoom)) {//LEFT
			sectorMap[currCoord.x][currCoord.y].left = false;
		}
		if ((currCoord.x + 1 >= dimSize) || (sectorMap[currCoord.x + 1][currCoord.y].isRoom)) {//RIGHT
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

	int numMiniBosses;
	switch (i_diff) {
	case EASY: {
		numMiniBosses = 1 + (i_levelNum / 5) * 1;
		break;
	}
	case MEDIUM: {
		numMiniBosses = 2 + (i_levelNum / 5) * 2;
		break;
	}
	case HARD: {
		numMiniBosses = 3 + (i_levelNum / 5) * 3;
		break;
	}
	}
	std::random_device rdRoomSeed;  //Will be used to obtain a seed for the random number engine
	std::mt19937 genRoomSeed(rdRoomSeed()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> extraRoomDist(1, sectorVect.size() - 2);
	//int randRoom = extraRoomDist(genRoomSeed);
	std::vector<int> previouslyChosen;
	int i = 0;
	while (i < numMiniBosses) {
		int randRoom = extraRoomDist(genRoomSeed);
		if ((std::find(previouslyChosen.begin(), previouslyChosen.end(), randRoom) == previouslyChosen.end()) &&
		 (previouslyChosen.size() < sectorVect.size() - 2)){
			sectorVect[randRoom]->PopulateBossRoom(m_lvl_num, i_diff, true);
			previouslyChosen.push_back(randRoom);
			i++;
		}
	}
	//Level "four" aka 3 with zero indexing, mini bosses dont drop health so Ima just put health drops in the level
	if ((m_lvl_num % 5) == 3) {
		std::shared_ptr<Entity> smallShipPOW3 = std::make_shared<PowerUp>(
			Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH * (2.0f / 5.0f)), GLBVRS::CRT_HGHT * (3.0f / 4.0f))), TEMP_HEALTH);
		sectorVect[sectorVect.size() - 2]->AddEntPtrToSector(smallShipPOW3);
		std::shared_ptr<Entity> smallShipPOW34 = std::make_shared<PowerUp>(
			Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH * (3.0f / 5.0f)), GLBVRS::CRT_HGHT * (3.0f / 4.0f))), TEMP_HEALTH);
		sectorVect[sectorVect.size() - 4]->AddEntPtrToSector(smallShipPOW34);
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
