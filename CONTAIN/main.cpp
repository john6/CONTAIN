#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <chrono>
#include <ctime>    
#include "GLOBAL_CONSTANTS.h"
#include "RESOURCES.h"
#include "Game.h"
#include "Menu.h"
#include "YouWonMenu.h"
#include "YouLostMenu.h"
#include "SaveData.h"

//TODO: Contact points should be in local space I think, I never convert them
//sf::RenderWindow window(sf::VideoMode(1800, 900), "CONTAIN", sf::Style::Fullscreen);
//sf::Vector2u size = window.getSize();
//const extern unsigned int GlobVars::SCREEN_WIDTH = size.x;
//const extern unsigned int GlobVars::SCREEN_HEIGHT = size.y;

int main()
{
	//if (__cplusplus == 201703L) std::cout << "C++17\n";
	//else if (__cplusplus == 201402L) std::cout << "C++14\n";
	//else if (__cplusplus == 201103L) std::cout << "C++11\n";
	//else if (__cplusplus == 199711L) std::cout << "C++98\n";
	//else std::cout << "pre-standard C++\n";

	//https://eigen.tuxfamily.org/dox//TopicMultiThreading.html
	Eigen::initParallel();

	SaveData saveData = SaveData();
	//saveData.DisplayHighScores();


	//sf::RenderWindow window(sf::VideoMode(1800, 900), "CONTAIN", sf::Style::Fullscreen);
	//sf::RenderWindow window(sf::VideoMode(GlobVars::SCREEN_WIDTH, GlobVars::SCREEN_HEIGHT), sf::Style::Fullscreen);
	//sf::Vector2u size = window.getSize();
	//unsigned int windowWidth = size.x;
	//unsigned int windowHeight = size.y;
	//const extern unsigned int GlobVars::SCREEN_WIDTH = size.x;
	//const extern unsigned int GlobVars::SCREEN_HEIGHT = size.y;
	sf::RenderWindow window(sf::VideoMode(1800, 900), "CONTAIN", sf::Style::Fullscreen);
	sf::Vector2u size = window.getSize();
	unsigned int screenWidth = size.x;
	unsigned int screenHeight = size.y;
	//GLOBAL_CONSTANTS globVars();
	GLOBAL_CONSTANTS::SetGlobalConstants(screenWidth, screenHeight);
	////WINDOW SETTINGS
	//unsigned int GLOBAL_CONSTANTS::SCREEN_WIDTH = screenWidth;
	//unsigned int GLOBAL_CONSTANTS::SCREEN_HEIGHT = screenHeight;

	////GAME RESOLUTION
	//float GLOBAL_CONSTANTS::COURT_WIDTH = static_cast<float>(GLOBAL_CONSTANTS::SCREEN_WIDTH * 9.0f / 10.0f);
	//float GLOBAL_CONSTANTS::COURT_HEIGHT = static_cast<float>(GLOBAL_CONSTANTS::SCREEN_HEIGHT * 9.0f / 10.0f);
	//float GLOBAL_CONSTANTS::HOR_MARGIN = static_cast<float>(GLOBAL_CONSTANTS::SCREEN_WIDTH / 20.0f);         //horizontal margin
	//float GLOBAL_CONSTANTS::VERT_MARGIN = static_cast<float>(GLOBAL_CONSTANTS::SCREEN_HEIGHT / 20.0f);       //vertical margin

	////MENU RESOLUTION
	//float GLOBAL_CONSTANTS::BUTTON_WIDTH = GLOBAL_CONSTANTS::SCREEN_WIDTH / 3;
	//float GLOBAL_CONSTANTS::BUTTON_HEIGHT = GLOBAL_CONSTANTS::SCREEN_HEIGHT / 10;



	int currLvl;
	RESOURCES resources;
	DIFFICULTY difficulty = MEDIUM;
	Game globalGame = Game(&window, &resources);
	Menu menu(&resources);
	YouWonMenu winMenu(&resources);
	YouLostMenu lostMenu(&resources);
	GAME_STATE state = MENU;
	bool justSwitchedBackToMenu = false;
	int lastRunScore = 0;
	hiRes_time_point currTime = hiResTime::now();
	const microSec UPDATE_INTERVAL(16666);   //16666.66 microseconds ~~ 16 milliseconds == 60 updates per second
	microSec lag(0);

	while (window.isOpen())
	{
		sf::Event currEvent;
		if (((window.pollEvent(currEvent)) && (currEvent.type == sf::Event::Closed)) || (state == EXIT_GAME)) { window.close(); }
		hiRes_time_point newTime = hiResTime::now();
		microSec currInterval = std::chrono::duration_cast<microSec>(newTime - currTime);
		currTime = newTime;
		lag += currInterval;
		//Set a max for lag if I wanna clamp
		if (lag > UPDATE_INTERVAL * (2)) {
			lag = UPDATE_INTERVAL * (2);
		}
		while (lag >= UPDATE_INTERVAL) {
			switch (state) {
			case MENU: {
				sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
				if (justSwitchedBackToMenu) {
					menu.ResetMenu();
					justSwitchedBackToMenu = false;
				}
				state = menu.Update(static_cast<float>(lag.count()), &window, mousePosition);
				break;
			}
			case IN_GAME: {
				sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
				hiRes_time_point beforePhysicsUpdate = hiResTime::now();
				state = globalGame.Update(static_cast<float>(lag.count()), &window, mousePosition);
				hiRes_time_point afterPhysicsUpdate = hiResTime::now();
				microSec currInterval = std::chrono::duration_cast<microSec>(afterPhysicsUpdate - beforePhysicsUpdate);
				std::string str = "Physics update took " + std::to_string(currInterval.count()) + " microseconds \n";
				std::cout << str;
				if (state == LOSE) {
					saveData.SaveNewHighScore(globalGame.currRunScore);
					lostMenu.SetPrevScore(globalGame.currRunScore);
					lostMenu.SetHighScores(saveData.GetHighScoresString());
				}
				else if (state == WIN) {
					saveData.SaveNewHighScore(globalGame.currRunScore);
					winMenu.SetPrevScore(globalGame.currRunScore);
					winMenu.SetHighScores(saveData.GetHighScoresString());
				}
				break;
			}
			case WIN: {
				sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
				state = winMenu.Update(static_cast<float>(lag.count()), &window, mousePosition);
				if (state == MENU) {
					justSwitchedBackToMenu = true;
				}
				break;
			}
			case LOSE: {
				sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
				state = lostMenu.Update(static_cast<float>(lag.count()), &window, mousePosition);
				if (state == MENU) {
					justSwitchedBackToMenu = true;
				}
				break;
			}
			case START_GAME: {
				currLvl = 0;
				globalGame.InitGame(menu.GetDifficulty());
				//globalGame.loadTestLevel();
				state = IN_GAME;
				break;
			}
			case EXIT_GAME: {
				break;
			}
			}
			lag -= UPDATE_INTERVAL;
		}
		switch (state) {
		case IN_GAME: {
			float percentUpdateElapsed = static_cast<float>(lag.count()) / static_cast<float>(UPDATE_INTERVAL.count());
			hiRes_time_point beforePhysicsUpdate = hiResTime::now();
			globalGame.Render(percentUpdateElapsed);
			hiRes_time_point afterPhysicsUpdate = hiResTime::now();
			microSec currInterval = std::chrono::duration_cast<microSec>(afterPhysicsUpdate - beforePhysicsUpdate);
			std::string str = "Render took " + std::to_string(currInterval.count()) + " microseconds \n \n";
			std::cout << str;
			break;
		}
		case MENU: {
			menu.Render(&window);
			break;
		}
		case WIN: {
			winMenu.Render(&window);
			break;
		}
		case LOSE: {
			lostMenu.Render(&window);
			break;
		}
		}
	}
	saveData.SaveDataToFile();
	return 0;
}