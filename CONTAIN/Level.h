#pragma once
#include <vector>
#include "GLOBAL_CONSTANTS.h"
#include "Entity.h"
#include <cmath>
#include "RigidBody.h"
#include <stdlib.h>
#include <random>
#include <queue>
#include "Sector.h"
#include "Entity.h"

struct MapNode;

class Level
{
protected:
	//Sector map will simply hold an int representing the index for the vector of sectors
	std::vector<std::vector<MapNode>> sectorMap;
	//Sector vect owns all sectors
	std::vector<std::shared_ptr<Sector>> sectorVect;
	//I need to hold a reference to all the map coords with rooms in them how was I not doing this
	std::vector<MapCoord> allCoords;
	//the origin is the center coordinates of the map

	int numSectors;
	int dimSize;

public:
	
	bool phaseOne;

	hiRes_time_point beginTime;
	microSec timeElapsed;
	float timeToComplete;

	std::shared_ptr<Entity> charPtr;
	MapCoord bossRoom;
	RESOURCES* resources;
	int m_lvl_num;
	MapCoord originCoord;

	sf::Color colPalA;
	sf::Color colPalB;


	Level(int i_levelNum, DIFFICULTY i_diff);
	Level(std::string i_testStr);
	Level();
	~Level();

	std::shared_ptr<Sector> GetSector(MapCoord i_coord);

	void CreateSectorAtCoord(MapCoord i_coord);

	void PopulateSectorAtCoord(MapCoord i_coord, int i_diff);

	void CreateOneWayDoor(MapCoord i_coordA, MapCoord i_coordB);

	void CreateBidirectionalDoor(MapCoord i_coordA, MapCoord i_coordB);

	float GetTimeLeftInLevel();

	void SwitchSectorsToPhaseTwo();

	void GenerateMapGrip(int i_lvlNum, DIFFICULTY i_diff);

	void ChooseBossRoom();

	void FillMapWithRooms(int i_levelNum, DIFFICULTY i_diff);

	void PopulateMapRooms(int i_levelNum, DIFFICULTY i_diff);

	void AddMiniBosses();

	void UpdateLevel();
};