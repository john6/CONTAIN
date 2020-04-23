#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <chrono>
#include <ctime>    
#include "GLOBAL_CONSTANTS.h"
#include "RESOURCES.h"
#include "Game.h"
#include "Menu.h"

//NEED TO ADD CLAMPING
//NEED TO ADD LERP I GUESS

int main()
{
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "CONTAIN");

	int currLvl;
	RESOURCES resources;
	DIFFICULTY difficulty = MEDIUM;
	Game* game = &Game(&resources);
	Menu menu(&resources);
	GAME_STATE state = MENU;

	hiRes_time_point currTime = hiResTime::now();
	const microSec UPDATE_INTERVAL(30000);
	microSec lag(0);

	while (window.isOpen())
	{
		sf::Event currEvent;
		if (((window.pollEvent(currEvent)) && (currEvent.type == sf::Event::Closed)) || (state == EXIT_GAME)) { window.close(); }
		hiRes_time_point newTime = hiResTime::now();
		microSec currInterval = std::chrono::duration_cast<microSec>(newTime - currTime);
		currTime = newTime;
		lag += currInterval;
		while (lag >= UPDATE_INTERVAL) {
			switch (state) {
			case MENU: {
				sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
				state = menu.Update(static_cast<float>(lag.count()), &window, mousePosition);
				break;
			}
			case IN_GAME: {
				state = game->Update(static_cast<float>(lag.count()));
				break;
			}
			case WIN: {
				state = MENU;
				break;
			}
			case LOSE: {
				state = MENU;
				break;
			}
			case START_GAME: {
				currLvl = 0;
				difficulty = menu.GetDifficulty();
				game->GenerateLevels(difficulty);
				state = IN_GAME;
				break;
			}
			}
			//Set a max for lag if I wanna clamp
			if (lag > UPDATE_INTERVAL * (2)) {
				lag = UPDATE_INTERVAL * (2);
			}

			lag -= UPDATE_INTERVAL;
		}
		if (state == IN_GAME) {
			float percentUpdateElapsed = static_cast<float>(lag.count()) / static_cast<float>(UPDATE_INTERVAL.count());
			game->Render(&window, percentUpdateElapsed);
		}
		else {
			menu.Render(&window);
		}
	}
	return 0;
}