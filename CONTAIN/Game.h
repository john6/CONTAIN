#pragma once

#include <iostream>
#include <algorithm>
#include <chrono>
#include <random>
#include <execution>
#include <concurrent_vector.h>
#include <SFML/Audio.hpp>
#include "GLOBAL_CONSTANTS.h"
#include "RESOURCES.h"
#include "HeadsUpDisplay.h"
#include "UpgradeMenu.h"
#include "Physics.h"
#include "GameRenderer.h"
#include "Level.h"
#include "LevelGenerator.h"
#include "QuadTree.h"
#include "InfoPopUp.h"
#include "TuteLib.h"
#include "Entity.h"
#include "PlayerController.h"

class playerChar;

enum PLAY_STATE { GENERAL_GAMEPLAY, WON_LEVEL, IN_GAME_MENU, DEAD, WON_GAME };

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

	GAME_STATE UpdateGeneral(float i_stepSize, sf::Vector2f i_mousePos);
	GAME_STATE UpdateLvlEntities(std::list<std::shared_ptr<Entity>>* i_lvlEnts, float i_stepSize);
	void DeleteLevels();
	bool playerWon;

	bool playerDied;
	hiRes_time_point pDeadTime;

	std::shared_ptr<UpgradeMenu> currUpgradeMenu;

public:
	PlayerController pController;
	bool tutorial;
	TuteLib tuteLib;
	DIFFICULTY gameDiff;

	bool firstPowerUp;

	int currRunScore;

	RESOURCES* resources;
	MapCoord currSector;
	int currLvl;
	GameRenderer gameRenderer;
	sf::RenderWindow* renderWindow;
	std::vector<Level*> levels;
	std::shared_ptr<Entity> playerChar;

	Game(sf::RenderWindow* i_window, RESOURCES* i_resources, DIFFICULTY i_difficulty = MEDIUM);
	~Game();

	GAME_STATE Update(float i_microSecs, sf::RenderWindow* i_window, sf::Vector2f i_mousePos);

	void Render(float i_elapsedMilliseconds);

	void TestCollision(std::shared_ptr<Entity> entA, std::shared_ptr<Entity> entB, std::vector<CollisionData>* collisionList);

	void GenerateLevels(DIFFICULTY i_diff);

	void GenerateTutorialLevels();

	const int GetCurrLvl();

	const int GetNumLvls();

	void RequestTravelToSector(MapCoord i_destSect);

	void CreatePlayerChar();

	void SpawnProjectile();

	void RequestGoToNextLvl();

	void InitGame(DIFFICULTY i_diff);

	void InitTutorial();

	void loadTestLevel(); 

	void PlayRandomSong();

	Vector2f GetCurrSectorDimensions();
};

