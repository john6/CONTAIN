#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "RESOURCES.h"
#include "GLOBAL_CONSTANTS.h"
#include "Button.h"

class YouLostMenu
{
private:

	int prevScore;
	sf::Font font;
	Button tryAgainButton;
	Button quitButton;
	sf::Text loseText;
	sf::Text prevScoreText;
	sf::Text highScoresText;

public:
	RESOURCES* resources;

	YouLostMenu(RESOURCES* i_resources);
	~YouLostMenu();

	bool PollInput(sf::Vector2i mousePosition, Button* button, bool stickyButton = false);

	GAME_STATE Update(float microSeconds, sf::RenderWindow* window, sf::Vector2i mousePosition);

	void SetPrevScore(int i_prevScore);

	void SetHighScores(std::string i_highScores);

	void Render(sf::RenderWindow* window);
};

