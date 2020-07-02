#include "Level.h"
#include <stdlib.h>
#include <random>
#include <queue>

struct MapNode {
	MapNode() : isRoom{ false }, up{ true }, down{ true }, left{ true }, right{ true },
		directionDoors{ {false, false, false, false} }, vectIndex{ -1 } {}

	bool isRoom;
	//up down left right
	bool up;
	bool down;
	bool left;
	bool right;
	std::vector<bool>directionDoors;
	int vectIndex;
};

Level::Level(int i_lvlNum, DIFFICULTY i_diff, std::shared_ptr<Entity> i_charPtr) :
	charPtr { i_charPtr }
{

	std::random_device rd1;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen1(rd1()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> distrib(1, 3); //both boundaries are inclusive
	//int randExtra = distrib(gen1);

	dimSize = 10; // + randExtra;
	//numSectors = 1 // + randExtra;

	if (dimSize % 2 == 0) { //Gonna make the dimensions always be odd so I can have a center coord
		dimSize += 1;
	}
	//initialize map and origin location
	sectorMap = std::vector<std::vector<MapNode>>(dimSize, std::vector<MapNode>(dimSize, MapNode()));
	originCoord = MapCoord((dimSize / 2) + 1, (dimSize / 2) + 1);
	CreateSectorAtCoord(originCoord);

	std::queue<MapCoord> fringe;
	fringe.push(originCoord);

	int currNumRooms = 0;
	int numRoomsTotal = 4;
	std::random_device rd2;
	std::mt19937 gen2(rd2());
	std::discrete_distribution<> discreteDist({ 0, 80, 15, 5 }); //percent chance a room will spawn what number of doors

	while ((!fringe.empty()) && (currNumRooms < numRoomsTotal)) {
		//get coord from fringe, check which directions are possible to build
		MapCoord currCoord = fringe.front();
		fringe.pop();
		if (currCoord.y + 1 >= dimSize) {//UP
			sectorMap[currCoord.x][currCoord.y].up = false;
		}
		else if (sectorMap[currCoord.x][currCoord.y+1].isRoom) {
			sectorMap[currCoord.x][currCoord.y].up = false;
		}
		if (currCoord.y - 1 < 0) {//DOWN
			sectorMap[currCoord.x][currCoord.y].down = false;
		}
		else if (sectorMap[currCoord.x][currCoord.y-1].isRoom) {
			sectorMap[currCoord.x][currCoord.y].down = false;
		}
		if (currCoord.x - 1 < 0) {//LEFT
			sectorMap[currCoord.x][currCoord.y].left = false;
		}
		else if (sectorMap[currCoord.x-1][currCoord.y].isRoom) {
			sectorMap[currCoord.x][currCoord.y].left = false;
		}
		if (currCoord.x + 1 >= dimSize) {//RIGHT
			sectorMap[currCoord.x][currCoord.y].right = false;
		}
		else if (sectorMap[currCoord.x+1][currCoord.y].isRoom) {
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
				GetSector(newCoord)->InitializeSector();
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
	//for (std::shared_ptr<Sector> sect : sectorVect) {
	//	sect->InitializeSector();
	//}
	RandomPlaceEndLevelObject();
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
	std::shared_ptr<Sector> newSector = std::make_shared<Sector>(charPtr);
	newSector->AddWallsToLevel();
	//newSector->GenerateLevelCubes();
	sectorVect.push_back(newSector);
	sectorMap[i_coord.x][i_coord.y].vectIndex = sectorVect.size() - 1;
	sectorMap[i_coord.x][i_coord.y].isRoom = true;
}

void Level::CreateOneWayDoor(MapCoord i_CoordA, MapCoord i_CoordB)
{
}

void Level::CreateBidirectionalDoor(MapCoord i_coordA, MapCoord i_coordB)
{
	float doorOutPadding = 150.0f;
	float horizontalMiddle = HOR_MARGIN + (COURT_WIDTH / 2.0f);
	float verticleMiddle = VERT_MARGIN + (COURT_HEIGHT / 2.0f);

	int yDist = i_coordB.y - i_coordA.y;
	int xDist = i_coordB.x - i_coordA.x;

	float doorWidth = 150.0f;
	float doorHeight = 50.0f;

	std::shared_ptr<Shape>doorShapeA;
	std::shared_ptr<Shape>doorShapeB;
	Vector2f startPosA;
	Vector2f startPosB;
	Vector2f outPosA;
	Vector2f outPosB;
	if (yDist == 1) { //UP
		doorShapeA = std::make_shared<Rectangle>(doorWidth, doorHeight);
		doorShapeB = std::make_shared<Rectangle>(doorWidth, doorHeight);
		startPosA = Vector2f(horizontalMiddle, VERT_MARGIN  + COURT_HEIGHT);
		startPosB = Vector2f(horizontalMiddle, VERT_MARGIN);
		outPosA = Vector2f(horizontalMiddle, VERT_MARGIN + doorOutPadding);
		outPosB = Vector2f(horizontalMiddle, VERT_MARGIN + COURT_HEIGHT - doorOutPadding);
	}
	else if (yDist == -1) { //DOWN
		doorShapeA = std::make_shared<Rectangle>(doorWidth, doorHeight);
		doorShapeB = std::make_shared<Rectangle>(doorWidth, doorHeight);
		startPosA = Vector2f(horizontalMiddle, VERT_MARGIN);
		startPosB = Vector2f(horizontalMiddle, VERT_MARGIN + COURT_HEIGHT);
		outPosA = Vector2f(horizontalMiddle, VERT_MARGIN + COURT_HEIGHT - doorOutPadding);
		outPosB = Vector2f(horizontalMiddle, VERT_MARGIN + doorOutPadding);
	}
	else if (xDist == -1) { //LEFT
		doorShapeA = std::make_shared<Rectangle>(doorHeight, doorWidth);
		doorShapeB = std::make_shared<Rectangle>(doorHeight, doorWidth);
		startPosA = Vector2f(HOR_MARGIN, verticleMiddle);
		startPosB = Vector2f(HOR_MARGIN + COURT_WIDTH, verticleMiddle);
		outPosA = Vector2f(COURT_WIDTH + HOR_MARGIN - doorOutPadding, verticleMiddle);
		outPosB = Vector2f(HOR_MARGIN + doorOutPadding, verticleMiddle);
	}
	else if (xDist == 1) { //RIGHT
		doorShapeA = std::make_shared<Rectangle>(doorHeight, doorWidth);
		doorShapeB = std::make_shared<Rectangle>(doorHeight, doorWidth);
		startPosA = Vector2f(HOR_MARGIN + COURT_WIDTH, verticleMiddle);
		startPosB = Vector2f(HOR_MARGIN, verticleMiddle);
		outPosA = Vector2f(HOR_MARGIN + doorOutPadding, verticleMiddle);
		outPosB = Vector2f(COURT_WIDTH + HOR_MARGIN - doorOutPadding, verticleMiddle);
	}
	else {
		//wtf these rooms arent adjacent
	}
	RigidBody doorBodyA(doorShapeA);
	RigidBody doorBodyB(doorShapeB);
	std::shared_ptr<Door> sectDoorA = std::make_shared<Door>(doorBodyA, startPosA, GetSector(i_coordA).get(), i_coordB, outPosA);
	std::shared_ptr<Door> sectDoorB = std::make_shared<Door>(doorBodyB, startPosB, GetSector(i_coordB).get(), i_coordA, outPosB);
	GetSector(i_coordA)->AddEntPtrToSector(sectDoorA);
	GetSector(i_coordB)->AddEntPtrToSector(sectDoorB);
}

void Level::RandomPlaceEndLevelObject()
{
	sectorVect[sectorVect.size() - 1]->AddEndLevelObject();
}

//Material Level::GetMaterial(MATERIAL_TYPE mat)
//{
//	switch (mat)
//	{
//	case ROCK: {
//		return Material(0.6f, 0.1f, 0.6f, 0.3f);
//	}
//	case WOOD: {
//		return Material(0.3f, 0.2f, 0.5f, 0.25f);
//	}
//	case METAL: {
//		return Material(1.2f, 0.05f, 0.4f, 0.2f);
//	}
//	case BOUNCYBALL: {
//		return Material(0.3f, 0.8f, 0.7f, 0.2f);
//	}
//	case SUPERBALL: {
//		return Material(0.3f, 0.95f, 0.5f, 0.25f);
//	}
//	case PILLOW: {
//		return Material(0.1f, 0.2f, 0.4f, 0.2f);
//	}
//	case STATIC: {
//		return Material(0.0f, 0.0f, 0.4f, 0.2f);
//	}
//	case STATICVERYFRICTION: {
//		return Material(0.0f, 0.0f, 0.9f, 0.7f);
//	}
//	case RUBBER: {
//		return Material(0.3f, 0.8f, 0.9f, 0.6f);
//	}
//	case ICE: {
//		return Material(0.3f, 0.1f, 0.7f, 0.05f);
//	}
//	default: {
//		return Material(0.3f, 0.2f, 0.5f, 0.25f);
//	}
//	}
//}