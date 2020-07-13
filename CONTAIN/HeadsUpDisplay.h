#pragma once

#include <SFML/Graphics.hpp>
#include "RESOURCES.h"
#include "Entity.h"
#include "Level.h"
#include "GLOBAL_CONSTANTS.h"

class HeadsUpDisplay
{
private:
	RESOURCES* resources;
	sf::Font font;
	sf::Text lvlNumText;
	sf::Text timeRemainingText;
	sf::Text healthRemainingText;
	sf::Text enemiesRemainingText;
	sf::Text BasicWeaponCharge;
	sf::Text specialWeaponAmmo;

public:
	HeadsUpDisplay(RESOURCES* i_resources);
	~HeadsUpDisplay();

	void Update(int i_lvlNum, int i_timeRemaining, std::shared_ptr<Entity> i_player);
	/*
	void RenderHUD(sf::RenderWindow* i_window, float i_lerp_fraction, std::shared_ptr<Entity> i_player, Level* i_lvl);*/

	std::vector<sf::Drawable*> GetDrawables();
};

