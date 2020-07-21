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
	//16:10 aspect ratio resolutions: 1280×800, 1440×900, 1680×1050, 1920×1200, and 2560×1600.
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "CONTAIN", sf::Style::Fullscreen);
	//sf::RenderWindow window(sf::VideoMode(1920, 1080), "CONTAIN");
	//sf::RenderWindow window(sf::VideoMode(1440, 900), "CONTAIN");
	//sf::RenderWindow window(sf::VideoMode(1280, 720), "CONTAIN");
	//sf::RenderWindow window(sf::VideoMode(1920, 1080), "CONTAIN");
	GLBVRS::SetGlobalConstants(window.getSize().x, window.getSize().y);
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

	bool notFullScreen = false;
	while (window.isOpen())
	{
		sf::Event currEvent;
		while (window.pollEvent(currEvent)) {
			if (((currEvent.type == sf::Event::Closed) || (state == EXIT_GAME))
				|| sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
				window.close();
			}
			if (currEvent.type == sf::Event::Resized) {
				GLBVRS::SetGlobalConstants(window.getSize().x, window.getSize().y);
			}
			if (currEvent.type == sf::Event::LostFocus) {
				window.create(sf::VideoMode(400, 400), "CONTAIN");
				GLBVRS::SetGlobalConstants(window.getSize().x, window.getSize().y);
				notFullScreen = true;
			}
			//For some reason this is not working, It never seems to call GetFocus()
			//if (currEvent.type == sf::Event::GainedFocus) {
			//	window.create(sf::VideoMode(1920, 1080), "CONTAIN", sf::Style::Fullscreen);
			//	GLBVRS::SetGlobalConstants(window.getSize().x, window.getSize().y);
			//}
		}
		if (window.hasFocus()) {
			if (notFullScreen) {
				notFullScreen = false;
				window.create(sf::VideoMode(1920, 1080), "CONTAIN", sf::Style::Fullscreen);
				GLBVRS::SetGlobalConstants(window.getSize().x, window.getSize().y);
			}
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
					state = IN_GAME;
					break;
				}
				case START_TUTORIAL: {
					currLvl = 0;
					globalGame.InitTutorial();
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
	}
	saveData.SaveDataToFile();
	return 0;
}