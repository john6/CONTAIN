#pragma once
#include <vector>
#include "GLOBAL_CONSTANTS.h"
#include "Entity.h"
#include "RigidBody.h"
#include "Sector.h"

struct MapNode;

class Level
{
protected:
	//Sector map will simply hold an int representing the index for the vector of sectors
	std::vector<std::vector<MapNode>> sectorMap;
	//Sector vect owns all sectors
	std::vector<std::shared_ptr<Sector>> sectorVect;
	//the origin is the center coordinates of the map

	int numSectors;
	int dimSize;

public:
	RESOURCES* resources;
	int m_lvl_num;
	MapCoord originCoord;

	Level(int i_levelNum, DIFFICULTY i_diff, std::shared_ptr<Entity> i_charPtr, RESOURCES* i_resources);
	~Level();

	std::shared_ptr<Entity> charPtr;

	std::shared_ptr<Sector> GetSector(MapCoord i_coord);

	//void GenerateSector(int i_sectX, int i_sectY, int i_numCube, int i_numCirc);

	//void ConnectSectors(int i_sectX1, int i_sectY1, int i_sectX2, int i_sectY2);

	//Material GetMaterial(MATERIAL_TYPE mat);

	void CreateSectorAtCoord(MapCoord i_coord);

	void PopulateSectorAtCoord(MapCoord i_coord, int i_diff);

	void CreateOneWayDoor(MapCoord i_coordA, MapCoord i_coordB);

	void CreateBidirectionalDoor(MapCoord i_coordA, MapCoord i_coordB);

	void RandomPlaceEndLevelObject();
};