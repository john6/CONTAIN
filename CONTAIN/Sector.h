#pragma once
#include <list>
#include <vector>
#include "GLOBAL_CONSTANTS.h"
#include "RESOURCES.h"
#include "Entity.h"
#include "RigidBody.h"
#include "Level.h"

class Sector
{
	//copying this class leads to errors because it creates Enemies that have pointers back to it stored,
	//Im going to make the copy operator private for now, since I dont want to create a smart copy operator

public:

	std::list<std::shared_ptr<Entity>> lvlEntitiesPhase1;
	std::list<std::shared_ptr<Entity>> lvlEntitiesPhase2;

	bool isBossRoom;
	bool firstPhase;
	bool filledIn;

	int sectEnemyNum;
	Level* myLevel;
	sf::Color colPalA;
	sf::Color colPalB;

	std::vector<TERRAIN_AREA> emptyTerrainAreas;

	std::list<std::shared_ptr<Entity>>* GetSectorEntities();

	void AddEntPtrToSector(std::shared_ptr<Entity> i_entPtr);

	void GenerateEnemies(int i_numEnems, TypeID enemyType, SCREEN_AREA i_area, int i_phaseNum, DIFFICULTY i_diff, int i_randMod);

	void AddWallsToLevel();

	void AddPainfullWallsToLevel();

	void AddRandomPainWall(int i_index);

	void PopulateEntranceRoom();

	void PopulateBossRoom(DIFFICULTY i_diff);

	void RemoveDestroyedEntities();

	void AddTerrain(int i_terrainType, bool terrainBig); // -1 input indicates random

	void InitializeSector();

	void PlaySound(int i_soundNum);

	void SwitchToPhaseTwo();

	void SwitchLevelToPhaseTwo();

	std::vector<std::tuple<Vector2f, Vector2f>> GetScreenAreas(SCREEN_AREA i_area);

	Sector(Level* i_lvlPtr, RESOURCES* i_resources, sf::Color i_colA = sf::Color::Black, sf::Color i_colB = sf::Color::White);
	~Sector();

private:
	RESOURCES* resources;

	Sector(const Sector& that);
	Sector& operator=(const Sector& that) = delete;
};