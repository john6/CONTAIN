#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <chrono>
#include <ctime>    
#include "GLOBAL_CONSTANTS.h"
#include "RESOURCES.h"
#include "Game.h"
#include "Menu.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "BREAKOUT");

	int currLvl;
	RESOURCES resources = RESOURCES();
	DIFFICULTY difficulty = MEDIUM;
	Game* game = &Game(&resources);
	Menu menu(&resources);
	GAME_STATE state = MENU;

	hiRes_time_point currTime = hiResTime::now();
	const microSec UPDATE_INTERVAL(10000);
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
				state = menu.Update((float)UPDATE_INTERVAL.count(), &window, mousePosition);
				break;
			}
			case IN_GAME: {
				state = game->Update((float)UPDATE_INTERVAL.count());
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
			lag -= UPDATE_INTERVAL;
		}
		if (state == IN_GAME) {
			//std::cout << "render game\n";
			game->Render(&window);
		}
		else {
			//std::cout << "render menu\n";
			menu.Render(&window);
		}
	}
	return 0;
}