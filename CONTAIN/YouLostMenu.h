#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "RESOURCES.h"
#include "GLOBAL_CONSTANTS.h"
#include "Button.h"

class YouLostMenu
{
private:

	sf::Font font;
	Button tryAgainButton;
	Button quitButton;
	sf::Text loseText;

public:
	RESOURCES* resources;

	YouLostMenu(RESOURCES* i_resources);
	~YouLostMenu();

	bool PollInput(sf::Vector2i mousePosition, Button* button, bool stickyButton = false);

	GAME_STATE Update(float microSeconds, sf::RenderWindow* window, sf::Vector2i mousePosition);

	void Render(sf::RenderWindow* window);
};

