#pragma once
#include <vector>
#include "GLOBAL_CONSTANTS.h"
#include "Entity.h"

class Level
{
protected:
	int m_lvl_num;
	std::vector<Entity> lvlBoundaries;
	std::vector<Entity> lvlEntites;
	int enemiesRemaining;
	float gameSpeedPerMill;
	float playerSpeed;
	float timeToComplete;
	microSec timeElapsed;

public:
	Level(int i_levelNum, DIFFICULTY i_diff);
	~Level();

	int GetLvlNum();
	std::vector<Entity> GetLvlBoundaries();
	std::vector<Entity> GetLvlEntites();
};

