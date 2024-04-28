#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include "GLOBAL_CONSTANTS.h"
#include "RESOURCES.h"
#include "Button.h"

class SplashScreen
{
private:
	RESOURCES* resources;
	sf::Font font;

	sf::Text title;

	sf::Text mainCreditLabel;
	sf::Text mainCreditName;

	sf::Text portingLabel;
	sf::Text portingList;

	sf::Text texturesLabel;
	sf::Text texturesList;

	sf::Text MusicLabel;
	sf::Text musicList;

	sf::Text playtestersLabel;
	sf::Text playtestersList;

public:
	sf::View splashView;



	


	SplashScreen(RESOURCES* i_resources);
	~SplashScreen();

	void Render(sf::RenderWindow* window);
};

