#include "YouWonMenu.h"


YouWonMenu::YouWonMenu(RESOURCES * i_resources, bool i_won) :
	resources{ i_resources }, won {i_won}
{
	font = resources->GetFont();

	sf::Vector2f topLeft(GLBVRS::HR_MRG * 1.0f, GLBVRS::VRT_MRG * 6.0f);
	float bttnMrgHr = (GLBVRS::HR_MRG / 4.0f);
	float bttnMrgVrt = (GLBVRS::VRT_MRG / 3.0f);
	float bttnOffsetHor = GLBVRS::BTTN_WDTH + bttnMrgHr;
	bttnOffsetVert = GLBVRS::BTTN_HGHT + bttnMrgVrt;
	highScoresListStart = topLeft + sf::Vector2f(bttnOffsetHor * 3.5, bttnOffsetVert * 0.5);

	sf::RectangleShape playAgainButtonRect(sf::Vector2f(GLBVRS::BTTN_WDTH, GLBVRS::BTTN_HGHT));
	playAgainButtonRect.setPosition(topLeft + sf::Vector2f(0.0f, bttnOffsetVert * 5.0f));
	if (won) {
		playAgainButton = Button("Play Again?", playAgainButtonRect, &font);
	}
	else {
		playAgainButton = Button("Try Again?", playAgainButtonRect, &font);
	}
	playAgainButton.SetColors(sf::Color::Black, sf::Color::White, sf::Color(128, 128, 128));
	//playAgainButton.SetPosition(topLeft + sf::Vector2f(0, 0));
	sf::RectangleShape quitButtonRect(sf::Vector2f(GLBVRS::BTTN_WDTH, GLBVRS::BTTN_HGHT));
	quitButtonRect.setPosition(topLeft + sf::Vector2f(bttnOffsetHor, bttnOffsetVert * 5.0f));
	quitButton = Button("Exit", quitButtonRect, &font);
	quitButton.SetColors(sf::Color::Black, sf::Color::White, sf::Color(128, 128, 128));

	winText.setFont(font);
	if (won) {
		winText.setString("You beat the game, wow congrats bud!\n");
	}
	else {
		winText.setString("You lost, Damn u suck!\n");
	}
	winText.setCharacterSize(25);
	winText.setFillColor(sf::Color::White);
	winText.setPosition(topLeft + sf::Vector2f(0, 0));

	prevScoreText.setFont(font);
	std::string finalScoreText = "Your final score was 0!\n";
	prevScoreText.setString(finalScoreText);
	prevScoreText.setCharacterSize(25);
	prevScoreText.setFillColor(sf::Color::White);
	prevScoreText.setPosition(topLeft + sf::Vector2f(0, bttnOffsetVert * 2));

	title.setFont(font);
	std::string resultText;
	if (won) {
		resultText = "YOU WON\n";
	}
	else {
		resultText = "YOU LOST\n";
	}
	title.setString(resultText);
	title.setCharacterSize(100);
	GLBVRS::SetTextOriginCenter(&title);
	title.setFillColor(sf::Color::White);
	title.setPosition(sf::Vector2f(GLBVRS::HR_MRG  + GLBVRS::CRT_WDTH * 0.5f, GLBVRS::VRT_MRG * 3.0f));

	highScoreLabelText.setFont(font);
	highScoreLabelText.setString("High Scores");
	highScoreLabelText.setCharacterSize(50);
	GLBVRS::SetTextOriginCenter(&highScoreLabelText);
	highScoreLabelText.setFillColor(sf::Color::White);
	highScoreLabelText.setPosition(highScoresListStart + sf::Vector2f(0, -bttnMrgVrt));

	highScoreRect = sf::RectangleShape(sf::Vector2f(highScoreLabelText.getLocalBounds().width * 1.1, bttnOffsetVert * 6.0f));
	highScoreRect.setFillColor(sf::Color(0.0f, 0.0f, 0.0f, 0.0f));
	highScoreRect.setOutlineColor(sf::Color::White);
	highScoreRect.setOutlineThickness(10.0f);
	highScoreRect.setOrigin(sf::Vector2f(highScoreRect.getSize().x / 2.0f, 0.0f));
	highScoreRect.setPosition(highScoreLabelText.getPosition() - sf::Vector2f(0.0f, highScoreLabelText.getLocalBounds().height));

}

YouWonMenu::~YouWonMenu()
{
}

bool YouWonMenu::PollInput(sf::Vector2f mousePosition, Button * button, bool stickyButton){
	float halfWidth = (button->GetRect().getSize().x / 2);
	float halfHeight = (button->GetRect().getSize().y / 2);
	float distX = abs(mousePosition.x - (button->GetRect().getPosition().x + halfWidth));
	float distY = abs(mousePosition.y - (button->GetRect().getPosition().y + halfHeight));
	if ((distX <= halfWidth) && (distY <= halfHeight)) {
		if (!stickyButton) { button->SetState(Button::HOVER); }
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			button->SetState(Button::DOWN);
			return true;
		}
	}
	else if (!stickyButton) {
		button->SetState(Button::UP);
	}
	return false;
}

GAME_STATE YouWonMenu::Update(float microSeconds, sf::RenderWindow * window, sf::Vector2f mousePosition)
{
	bool playAgainButtonPressed = PollInput(mousePosition, &playAgainButton);
	bool quitButtonPressed = PollInput(mousePosition, &quitButton);
	if (playAgainButtonPressed) {
		resources->PlaySound(RESOURCES::OCTAVE_BEEP);
		return MENU;
	}
	else if (quitButtonPressed) {
		resources->PlaySound(RESOURCES::OCTAVE_BEEP);
		return EXIT_GAME;
	}
	else {
		if (won) {
			return WIN;
		}
		else {
			return LOSE;
		}

	}
}


void YouWonMenu::SetPrevScore(int i_prevScore)
{
	prevScore = i_prevScore;
	//prevScoreText.setFont(font);
	std::string finalScoreText = "Your final score was " + std::to_string(i_prevScore) + "!\n";
	prevScoreText.setString(finalScoreText);
	//prevScoreText.setCharacterSize(25);
	//prevScoreText.setFillColor(sf::Color::White);
	//prevScoreText.setPosition(sf::Vector2f(100, 600));
}

void YouWonMenu::SetHighScores(std::list<int> i_highScores)
{
	//highScoresText.setFont(font);
	highScoresList.clear();
	highScoresChart.clear();
	for (int score : i_highScores) {
		highScoresList.push_back(score);
	}
	for (int i = 0; i < highScoresList.size(); i++) {
		sf::Text highScoreText = sf::Text();
		highScoreText.setFont(font);
		std::string highScoreListText =  "("+  std::to_string(i) + ")   " + std::to_string(highScoresList[i]) + "\n";
		highScoreText.setString(highScoreListText);
		highScoreText.setCharacterSize(25);
		highScoreText.setFillColor(sf::Color::White);
		GLBVRS::SetTextOriginCenter(&highScoreText);
		highScoreText.setPosition(highScoresListStart + sf::Vector2f(0.0f, (i + 1) * bttnOffsetVert / 2.0f));
		highScoresChart.push_back(highScoreText);
	}
}

void YouWonMenu::Render(sf::RenderWindow * window)
{
	//sf::Text playAgainText = playAgainButton.GetText();
	//playAgainText.setFont(font);
	//sf::Text quitButtonText = quitButton.GetText();
	//quitButtonText.setFont(font);

	window->clear();

	window->draw(title);
	window->draw(playAgainButton.GetRect());
	window->draw(playAgainButton.GetText());
	window->draw(quitButton.GetRect());
	window->draw(quitButton.GetText());
	window->draw(winText);
	window->draw(prevScoreText);
	window->draw(highScoreLabelText);
	window->draw(highScoreRect);

	for (sf::Text highScore : highScoresChart) {
		window->draw(highScore);
	}

	window->display();
}
