#pragma once
#include "RESOURCES.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <random>
#include <string>
#include "Button.h"
#include "Entity.h"

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

	bool PollInput(sf::Vector2i mousePosition, Button* button, bool stickyButton = false);
	void  PollButtonTriplet(sf::Vector2i mousePosition);
	void  UpdateButtonTriplet(int i_upgradeSelected);

	std::string GetUpgradeText(UPGRADE_TYPE i_type);
public:
	RESOURCES* resources;

	UpgradeMenu(RESOURCES* i_resources, DIFFICULTY i_diff, PlayerChar* i_player);
	~UpgradeMenu();

	UPGRADE_TYPE Update(float microSeconds, sf::RenderWindow* window, sf::Vector2i i_mousePos);

	void Render(sf::RenderWindow* window);
};
