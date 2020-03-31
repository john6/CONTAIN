#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>
#include "RESOURCES.h"
#include "GLOBAL_CONSTANTS.h"
#include "Button.h"


class Menu
{
private:
	RESOURCES* resources;
	DIFFICULTY currDifficulty;
	sf::Font font;
	sf::Text instructions1;
	Button playButton;
	Button exitButton;
	Button easyButton;
	Button mediumButton;
	Button hardButton;
	bool shouldExit;
	bool shouldStart;

	bool PollInput(sf::Vector2i mousePosition, Button* button, bool stickyButton = false);

	void  PollButtonTriplet(sf::Vector2i mousePosition);
	void  UpdateButtonTriplet(DIFFICULTY i_difficultySelected);

public:
	Menu(RESOURCES* i_resources, DIFFICULTY i_defaultDiff = MEDIUM);
	~Menu();

	DIFFICULTY GetDifficulty();

	GAME_STATE Update(float microSeconds, sf::RenderWindow* window, sf::Vector2i mousePosition);

	void Render(sf::RenderWindow* window);
};

