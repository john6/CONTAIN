#include "HeadsUpDisplay.h"

HeadsUpDisplay::HeadsUpDisplay(RESOURCES* i_resources) : resources{ i_resources }
{
	font = resources->GetFont();

	lvlNumText.setFont(font);
	timeRemainingText.setFont(font);
	livesRemainingText.setFont(font);

	lvlNumText.setCharacterSize(15);
	timeRemainingText.setCharacterSize(20);
	livesRemainingText.setCharacterSize(15);

	lvlNumText.setPosition(sf::Vector2f(HORIZONTAL_MARGIN, 5.0f));
	timeRemainingText.setPosition(sf::Vector2f(HORIZONTAL_MARGIN, 25.0f));
	livesRemainingText.setPosition(sf::Vector2f(HORIZONTAL_MARGIN + (COURT_WIDTH / 2), 5.0f));

	lvlNumText.setFillColor(sf::Color::White);
	timeRemainingText.setFillColor(sf::Color::White);
	livesRemainingText.setFillColor(sf::Color::White);

	lvlNumText.setString("LEVEL: INIT");
	timeRemainingText.setString("TIME: INIT");
	livesRemainingText.setString("LIVES: INIT");
}

HeadsUpDisplay::~HeadsUpDisplay()
{
}

void HeadsUpDisplay::UpdateHUD(int i_lvlNum, int i_timeRemaining, int i_livesRemaining) {

	std::string level_num_string = "LEVEL: " + std::to_string(i_lvlNum);
	lvlNumText.setString(level_num_string);

	std::string time_string = "TIME: " + std::to_string(i_timeRemaining);
	timeRemainingText.setString(time_string);

	std::string balls_str = "LIVES: " + std::to_string(i_livesRemaining);
	livesRemainingText.setString(balls_str);

	//VARIOUS OTHER GAME STATE VARIABLES CAN BE UPDATED HERE
}

void HeadsUpDisplay::RenderHUD() {
}