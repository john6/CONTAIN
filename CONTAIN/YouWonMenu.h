#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "RESOURCES.h"
#include "GLOBAL_CONSTANTS.h"
#include "Button.h"

class YouWonMenu
{
private:

	sf::Font font;
	Button playAgainButton;
	Button quitButton;
	sf::Text winText;

public:
	RESOURCES* resources;

	YouWonMenu(RESOURCES* i_resources);
	~YouWonMenu();

	bool PollInput(sf::Vector2i mousePosition, Button* button, bool stickyButton = false);

	GAME_STATE Update(float microSeconds, sf::RenderWindow* window, sf::Vector2i mousePosition);

	void Render(sf::RenderWindow* window);
};

