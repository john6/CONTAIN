#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "RESOURCES.h"
#include "GLOBAL_CONSTANTS.h"
#include "Button.h"
#include <chrono>


class Menu
{
private:
	DIFFICULTY currDifficulty;
	sf::Font font;
	sf::Text title;
	sf::Text difficultyLabel;
	sf::Text instructions1;
	sf::Text instructions2;
	Button playButton;
	Button playTutorialButton;
	Button exitButton;
	Button easyButton;
	Button mediumButton;
	Button hardButton;
	Button settingsButton;

	bool PollInput(sf::Vector2f mousePosition, Button* button, bool stickyButton = false);

	void  PollButtonTriplet(sf::Vector2f mousePosition);
	void  UpdateButtonTriplet(DIFFICULTY i_difficultySelected);

public:
	sf::View menuView;

	RESOURCES* resources;

	Menu(RESOURCES* i_resources, DIFFICULTY i_defaultDiff = MEDIUM);
	~Menu();

	DIFFICULTY GetDifficulty();

	void ResetMenu();

	GAME_STATE Update(float microSeconds, sf::RenderWindow* window, sf::Vector2f mousePosition);

	void Render(sf::RenderWindow* window);
};

