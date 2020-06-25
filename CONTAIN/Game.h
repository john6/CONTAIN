#pragma once

#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <execution>
#include <concurrent_vector.h>
#include <SFML/Audio.hpp>
#include "GLOBAL_CONSTANTS.h"
#include "RESOURCES.h"
#include "Physics.h"
#include "GameRenderer.h"
#include "HeadsUpDisplay.h"
#include "Level.h"
#include "LevelGenerator.h"
#include "Entity.h"

enum PLAY_STATE { GENERAL_GAMEPLAY, WON_LEVEL, DEAD };

class Game
{
private:
	RESOURCES* resources;
	PLAY_STATE playState;
	sf::Font font;
	int currLvl;
	int currSector;
	int numLvls;  //Not being used yet
	std::vector<Level*> levels;
	HeadsUpDisplay HUD;
	int livesRemaining;
	int enemiesRemaining;
	float gameSpeedPerMill;
	float playerSpeed;

	hiRes_time_point lastShotFired;
	microSec timeSinceFired;
	float shipRateOfFire;

	hiRes_time_point beginTime;
	microSec timeElapsed;
	float timeToComplete;

	GAME_STATE UpdateGeneral(float i_stepSize, sf::Vector2i i_mousePos);
	GAME_STATE UpdateLvlEntities(std::list<std::shared_ptr<Entity>>* i_lvlEnts, float i_stepSize);
	void UpdateHUD();
	void PollKeys(float i_step, sf::Vector2i i_mousePos);
	void DeleteLevels();

public:
	std::shared_ptr<Entity> playerChar;

	Game(RESOURCES* i_resources, DIFFICULTY i_difficulty = MEDIUM);
	~Game();

	GAME_STATE Update(float i_microSecs, sf::RenderWindow* i_window, sf::Vector2i i_mousePos);

	void Render(sf::RenderWindow* i_window, float i_elapsedMilliseconds);

	void TestCollisionsInList();

	void TestCollision(std::shared_ptr<Entity> entA, std::shared_ptr<Entity> entB, std::vector<CollisionData>* collisionList);

	void GenerateLevels(DIFFICULTY i_diff);

	const int GetCurrLvl();

	const int GetNumLvls();
};

