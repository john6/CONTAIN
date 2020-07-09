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

	int prevScore;
	sf::Text prevScoreText;
	sf::Text highScoresText;

public:
	RESOURCES* resources;

	YouWonMenu(RESOURCES* i_resources);
	~YouWonMenu();

	bool PollInput(sf::Vector2i mousePosition, Button* button, bool stickyButton = false);

	GAME_STATE Update(float microSeconds, sf::RenderWindow* window, sf::Vector2i mousePosition);

	void Render(sf::RenderWindow* window);

	void SetPrevScore(int i_prevScore);

	void SetHighScores(std::string i_highScores);
};

