#pragma once

#include <iostream>
#include <algorithm>
#include <chrono>
//#include <thread>
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
#include "QuadTree.h"

enum PLAY_STATE { GENERAL_GAMEPLAY, WON_LEVEL, DEAD, WON_GAME };

class Game
{
private:
	PLAY_STATE playState;
	sf::Font font;

	int numLvls;  //Not being used yet
	HeadsUpDisplay HUD;
	int livesRemaining;
	float gameSpeedPerMill;

	hiRes_time_point beginTime;
	microSec timeElapsed;
	float timeToComplete;

	GAME_STATE UpdateGeneral(float i_stepSize, sf::Vector2i i_mousePos);
	GAME_STATE UpdateLvlEntities(std::list<std::shared_ptr<Entity>>* i_lvlEnts, float i_stepSize);
	void DeleteLevels();
	bool playerWon;



public:

	int currRunScore;

	RESOURCES* resources;
	MapCoord currSector;
	int currLvl;
	sf::RenderWindow* renderWindow;
	std::vector<Level*> levels;
	std::shared_ptr<Entity> playerChar;

	Game(sf::RenderWindow* i_window, RESOURCES* i_resources, DIFFICULTY i_difficulty = MEDIUM);
	~Game();

	GAME_STATE Update(float i_microSecs, sf::RenderWindow* i_window, sf::Vector2i i_mousePos);

	void Render(float i_elapsedMilliseconds);

	void TestCollision(std::shared_ptr<Entity> entA, std::shared_ptr<Entity> entB, std::vector<CollisionData>* collisionList);

	void GenerateLevels(DIFFICULTY i_diff);

	const int GetCurrLvl();

	const int GetNumLvls();

	void RequestTravelToSector(MapCoord i_destSect);

	void CreatePlayerChar();

	void SpawnProjectile();

	void RequestGoToNextLvl();

	void InitGame(DIFFICULTY i_diff);

	void loadTestLevel(); 
};

