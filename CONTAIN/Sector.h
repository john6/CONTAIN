#pragma once
#include <list>
#include <vector>
#include "GLOBAL_CONSTANTS.h"
#include "RESOURCES.h"
#include "Entity.h"
#include "RigidBody.h"

class Sector
{
	//copying this class leads to errors because it creates Enemies that have pointers back to it stored,
	//Im going to make the copy operator private for now, since I dont want to create a smart copy operator

public:

	std::list<std::shared_ptr<Entity>> lvlEntities;
	int sectEnemyNum;
	std::shared_ptr<Entity> playerChar;

	std::list<std::shared_ptr<Entity>>* GetSectorEntities();

	void AddEntPtrToSector(std::shared_ptr<Entity> i_entPtr);

	void GenerateLevelCubes(int i_numCubes);

	void GenerateLevelCircles(int i_numCircs);

	void AddWallsToLevel();

	void AddPainfullWallsToLevel();

	void AddRandomPainWall(int i_index);

	void AddEndLevelObject();

	void RemoveDestroyedEntities();

	void AddTerrain(int i_terrainType);

	void InitializeSector();

	void PlaySound(int i_soundNum);

	Sector(std::shared_ptr<Entity> i_charPtr, RESOURCES* i_resources);
	~Sector();

private:
	RESOURCES* resources;

	Sector(const Sector& that);
	Sector& operator=(const Sector& that) = delete;
};

