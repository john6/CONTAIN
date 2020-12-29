#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <random>
#include <string>
#include "RESOURCES.h"
#include "Button.h"
#include "Entity.h"
#include "PlayerChar.h"

class UpgradeMenu
{

private:
	sf::Font font;
	sf::Text upgradeText;
	int upgradeA;
	int upgradeB;
	int upgradeC;
	Button upgradeButtonA;
	Button upgradeButtonB;
	Button upgradeButtonC;
	Button nextLevelButton;
	bool firstOpen;
	UPGRADE_TYPE currUpgradeType;
	std::vector<int> upgrades;

	bool PollInput(sf::Vector2f mousePosition, Button* button, bool stickyButton = false);
	void  PollButtonTriplet(sf::Vector2f mousePosition);
	void  UpdateButtonTriplet(int i_upgradeSelected);

	sf::View upgradeMenuView;

	std::string GetUpgradeText(UPGRADE_TYPE i_type);
public:
	RESOURCES* resources;

	UpgradeMenu(RESOURCES* i_resources, DIFFICULTY i_diff, PlayerChar* i_player);
	~UpgradeMenu();

	UPGRADE_TYPE Update(float microSeconds, sf::RenderWindow* window, sf::Vector2f i_mousePos);

	void Render(sf::RenderWindow* window);
};

