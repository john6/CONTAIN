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

//TODO: Contact points should be in local space I think, I never convert them

int main()
{
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "CONTAIN");

	int currLvl;
	RESOURCES resources;
	DIFFICULTY difficulty = MEDIUM;
	Game globalGame = Game(&window, &resources);
	Menu menu(&resources);
	YouWonMenu winMenu(&resources);
	YouLostMenu lostMenu(&resources);
	GAME_STATE state = MENU;
	bool justSwitchedBackToMenu = false;

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
				globalGame.Restart();
				difficulty = menu.GetDifficulty();
				globalGame.GenerateLevels(difficulty);
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
	return 0;
}