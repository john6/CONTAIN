#pragma once

#include <SFML/Graphics.hpp>
#include "RESOURCES.h"
#include "Entity.h"
#include "Level.h"
#include "GLOBAL_CONSTANTS.h"

#include <chrono>

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

	sf::RectangleShape lvlNumRect;
	sf::RectangleShape healthRemainingRect;
	sf::RectangleShape specialWeaponAmmoRect;
	sf::RectangleShape specialWeaponChargeRect;
	sf::RectangleShape timeRemainingRects;

public:
	HeadsUpDisplay(RESOURCES* i_resources);
	~HeadsUpDisplay();

	void Update(int i_lvlNum, int i_timeRemaining, int i_maxTime, std::shared_ptr<Entity> i_player);
	/*
	void RenderHUD(sf::RenderWindow* i_window, float i_lerp_fraction, std::shared_ptr<Entity> i_player, Level* i_lvl);*/

	void UpdateAOECHarge(std::shared_ptr<Entity> i_player);

	std::vector<sf::Drawable*> GetDrawables();
};

