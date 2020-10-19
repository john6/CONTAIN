#pragma once

#include <SFML/Graphics.hpp>
#include "RESOURCES.h"
#include "Entity.h"
#include "PlayerChar.h"
#include "Level.h"
#include "GLOBAL_CONSTANTS.h"
#include <chrono>

class HeadsUpDisplay
{
private:
	float hudHorMargin;
	float hudHorSize;
	float hudVertSize;
	sf::Vector2f  hudTopLeft;
	sf::Vector2f hudItemsOffset;
	sf::Vector2f hudItemPosition;
	float hudItemSpacing;
	float hudItemRectHeight;
	float hudFillBarHeight;
	float hudOutLineThickness;
	float hudItemOutLineThickness;
	float textHorOffset;
	float textVertOffset;
	float rectHorExtraWidth;
	sf::Vector2f textOffset;

	RESOURCES* resources;
	sf::Font font;
	sf::Text lvlNumText;
	sf::Text spclWpLblTxt;
	sf::Text timeRemainingText;

	sf::RectangleShape backGroundRect;
	sf::RectangleShape lvlNumRect;
	sf::RectangleShape spclWpLblRct;
	sf::RectangleShape specialWeaponRect;
	sf::RectangleShape specialWeaponCurrChargeRect;
	sf::RectangleShape timeRemainingRects;

	sf::Vector2f lvlNumPositionOffset;
	sf::Vector2f healthLabelOffset;
	sf::Vector2f spclWpLblRctOffset;
	sf::Vector2f specialWeapPosOffset;
	sf::Vector2f timeRemPosOffset;

	//HEALTH
	sf::Vector2f healthRemainingPosOffset;
	int prevHealth;
	float healthRectWidth;
	sf::RectangleShape healthLabelRect;
	sf::Text healthLabelText;
	float segmentMarg;
	sf::RectangleShape healthSegmentsRect;
	std::vector<sf::RectangleShape> healthSegments;

public:
	HeadsUpDisplay(RESOURCES* i_resources);
	~HeadsUpDisplay();

	void Update(int i_lvlNum, int i_timeRemaining, int i_maxTime, PlayerChar* i_player);

	void UpdateAOECharge(PlayerChar* i_player);

	void UpdateHealth(PlayerChar* i_player);

	std::vector<sf::Drawable*> GetDrawables();
};

