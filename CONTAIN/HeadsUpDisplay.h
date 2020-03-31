#pragma once

#include <SFML/Graphics.hpp>
#include "RESOURCES.h"
#include "GLOBAL_CONSTANTS.h"

class HeadsUpDisplay
{
private:
	RESOURCES* resources;
	sf::Font font;
	sf::Text lvlNumText;
	sf::Text timeRemainingText;
	sf::Text livesRemainingText;
	sf::Text enemiesRemainingText;

public:
	HeadsUpDisplay(RESOURCES* i_resources);
	~HeadsUpDisplay();

	void UpdateHUD(int i_lvlNum, int i_timeRemaining, int i_livesRemaining);
	
	void RenderHUD();
};

