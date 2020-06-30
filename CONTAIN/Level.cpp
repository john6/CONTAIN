#include "Level.h"
#include <stdlib.h>
#include <random>

Level::Level(int i_lvlNum, DIFFICULTY i_diff, std::shared_ptr<Entity> i_charPtr) :
	charPtr { i_charPtr }
{

	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> distrib(1, 3); //both boundaries are inclusive
	int randExtra = distrib(gen);

	dimSize = 9 + randExtra;
	numSectors = 1 + randExtra;

	if (dimSize % 2 == 0) { //Gonna make the dimensions always be odd so I can have a center coord
		dimSize += 1;
	}
	origin.x = (dimSize/2)+1;
	origin.y = (dimSize / 2) +1;



	sectorMap = std::vector<std::vector<int>>(dimSize, std::vector<int>(dimSize, -1));

	int i = 0;
	int currDimX = origin.x;
	int currDimY = origin.y;

	//populate locations with sectors
	std::vector<MapCoord> indexToMapCoords;
	while (i < numSectors) {
		MapCoord m = MapCoord(currDimX, currDimY);
		indexToMapCoords.push_back(m);
		sectorMap[currDimX][currDimY] = i;
		++currDimY;
		++i;
	}
	//generate sectors
	i = 0;
	while (i < numSectors) {
		std::shared_ptr<Sector> newSector = std::make_shared<Sector>(charPtr);
		sectorVect.push_back(newSector);
		sectorVect[i]->AddWallsToLevel();
		sectorVect[i]->GenerateLevelCubes(1);
		if (i < numSectors - 1) {
			std::shared_ptr<Shape>doorShape = std::make_shared<Rectangle>(50, 200);
			RigidBody doorBody(doorShape);
			Vector2f startPos = Vector2f(HOR_MARGIN, COURT_HEIGHT / 2.0f);
			Vector2f outPos = Vector2f(COURT_WIDTH + HOR_MARGIN, COURT_HEIGHT / 2.0f);
			//this is super FUCKED BTW, Im calling "get()" on a shared pointer and I totally shouldn't be
			std::shared_ptr<Door> sectDoor = std::make_shared<Door>(doorBody, startPos, sectorVect[i].get(), indexToMapCoords[i + 1], outPos);
			sectorVect[i]->AddEntPtrToSector(sectDoor);
		}
		++i;
	}
}

Level::~Level()
{
}

std::shared_ptr<Sector> Level::GetSector(MapCoord i_coord)
{ 
	return sectorVect[sectorMap[i_coord.x][i_coord.y]];
}

void Level::GenerateSector(int i_sectX, int i_sectY, int i_numCube, int i_numCirc)
{

}

void Level::ConnectSectors(int i_sectX1, int i_sectY1, int i_sectX2, int i_sectY2)
{
}

Material Level::GetMaterial(MATERIAL_TYPE mat)
{
	switch (mat)
	{
	case ROCK: {
		return Material(0.6f, 0.1f, 0.6f, 0.3f);
	}
	case WOOD: {
		return Material(0.3f, 0.2f, 0.5f, 0.25f);
	}
	case METAL: {
		return Material(1.2f, 0.05f, 0.4f, 0.2f);
	}
	case BOUNCYBALL: {
		return Material(0.3f, 0.8f, 0.7f, 0.2f);
	}
	case SUPERBALL: {
		return Material(0.3f, 0.95f, 0.5f, 0.25f);
	}
	case PILLOW: {
		return Material(0.1f, 0.2f, 0.4f, 0.2f);
	}
	case STATIC: {
		return Material(0.0f, 0.0f, 0.4f, 0.2f);
	}
	case STATICVERYFRICTION: {
		return Material(0.0f, 0.0f, 0.9f, 0.7f);
	}
	case RUBBER: {
		return Material(0.3f, 0.8f, 0.9f, 0.6f);
	}
	case ICE: {
		return Material(0.3f, 0.1f, 0.7f, 0.05f);
	}
	default: {
		return Material(0.3f, 0.2f, 0.5f, 0.25f);
	}
	}
}
