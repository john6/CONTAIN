#pragma once
#include <vector>
#include "GLOBAL_CONSTANTS.h"

class LevelGenerator
{
public:
	LevelGenerator();
	~LevelGenerator();

	static void CreateLvl(int i_lvlNum, DIFFICULTY i_diff);
};

