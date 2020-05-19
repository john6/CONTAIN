#pragma once
#include <vector>
#include <list>
#include "GLOBAL_CONSTANTS.h"
#include "Entity.h"
#include "RigidBody.h"

class Level
{
protected:
	int m_lvl_num;

	std::list<Entity> lvlBoundaries;
	std::list<Entity> lvlEntites;
	int enemiesRemaining;
	float gameSpeedPerMill;
	float playerSpeed;
	float timeToComplete;
	microSec timeElapsed;

public:
	Level(int i_levelNum, DIFFICULTY i_diff);
	~Level();

	int GetLvlNum();
	std::list<Entity> GetLvlBoundaries();
	std::list<Entity>* GetLvlEntites();

	void AddEntityToLevel(Entity i_ent);
};

