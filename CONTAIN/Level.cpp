#include "Level.h"

Level::Level(int i_lvlNum, DIFFICULTY i_diff)
{
	switch (i_diff) {
		case EASY: {
			gameSpeedPerMill = SPEED_PER_MILLISECOND_EASY;
			playerSpeed = PLAYER_SPEED_PER_MILLISECOND_EASY;
			timeToComplete = SECONDS_TO_WIN_EASY;
			break;
		}
		case MEDIUM: {
			gameSpeedPerMill = SPEED_PER_MILLISECOND_MEDIUM;
			playerSpeed = PLAYER_SPEED_PER_MILLISECOND_MEDIUM;
			timeToComplete = SECONDS_TO_WIN_MEDIUM;
			break;
		}
		case HARD: {
			gameSpeedPerMill = SPEED_PER_MILLISECOND_HARD;
			playerSpeed = PLAYER_SPEED_PER_MILLISECOND_HARD;
			timeToComplete = SECONDS_TO_WIN_HARD;
			break;
		}
	}
	switch (i_lvlNum) {
		case 1: {
			//do stuff
			break;
		}
		case 2: {
			//do stuff
			break;
		}
		case 3: {
			//do stuff
			break;
		}
	}
}

Level::~Level()
{
}

int Level::GetLvlNum() { return m_lvl_num; }
std::vector<Entity> Level::GetLvlBoundaries() { return lvlBoundaries; }
std::vector<Entity> Level::GetLvlEntites() { return lvlEntites; }