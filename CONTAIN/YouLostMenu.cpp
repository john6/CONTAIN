#include "YouLostMenu.h"


YouLostMenu::YouLostMenu(RESOURCES * i_resources) :
	resources{ i_resources }
{
	font = resources->GetFont();

	resources = i_resources;

	sf::Vector2f topLeft(GLBVRS::HR_MRG * 4.5f, GLBVRS::VRT_MRG * 4.0f);
	float bttnMrgHr = (GLBVRS::HR_MRG / 4.0f);
	float bttnMrgVrt = (GLBVRS::VRT_MRG / 3.0f);
	float bttnOffsetHor = GLBVRS::BTTN_WDTH + bttnMrgHr;
	float bttnOffsetVert = GLBVRS::BTTN_HGHT + bttnMrgVrt;

	sf::RectangleShape playAgainButtonRect(sf::Vector2f(GLBVRS::BTTN_WDTH, GLBVRS::BTTN_HGHT));
	playAgainButtonRect.setPosition(sf::Vector2f(100, 100));
	tryAgainButton = Button("Try Again?", playAgainButtonRect, &font);
	tryAgainButton.ResizeForText();
	tryAgainButton.SetColors(sf::Color::Black, sf::Color::White, sf::Color(128, 128, 128));

	sf::RectangleShape quitButtonRect(sf::Vector2f(GLBVRS::BTTN_WDTH, GLBVRS::BTTN_HGHT));
	quitButtonRect.setPosition(sf::Vector2f(tryAgainButton.GetRect().getSize().x + (GLBVRS::BTTN_WDTH * (0.4f)), 100));
	quitButton = Button("Exit", quitButtonRect, &font);
	quitButton.SetColors(sf::Color::Black, sf::Color::White, sf::Color(128, 128, 128));

	loseText.setFont(font);
	loseText.setString("You lost, Damn u suck!\n");
	loseText.setCharacterSize(25);
	loseText.setFillColor(sf::Color::White);
	loseText.setPosition(sf::Vector2f(100, 400));

	prevScoreText.setFont(font);
	std::string finalScoreText = "Your final score was 0!\n";
	prevScoreText.setString(finalScoreText);
	prevScoreText.setCharacterSize(25);
	prevScoreText.setFillColor(sf::Color::White);
	prevScoreText.setPosition(sf::Vector2f(100, 600));

	highScoresText.setFont(font);
	std::string highScoreListText = "uuuuuh this should have been set woopsies 0!\n";
	highScoresText.setString(highScoreListText);
	highScoresText.setCharacterSize(25);
	highScoresText.setFillColor(sf::Color::White);
	highScoresText.setPosition(sf::Vector2f(100, 700));


}

YouLostMenu::~YouLostMenu()
{
}

bool YouLostMenu::PollInput(sf::Vector2i mousePosition, Button * button, bool stickyButton) {
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

GAME_STATE YouLostMenu::Update(float microSeconds, sf::RenderWindow * window, sf::Vector2i mousePosition)
{
	bool playAgainButtonPressed = PollInput(mousePosition, &tryAgainButton);
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
		return LOSE;
	}
}

void YouLostMenu::SetPrevScore(int i_prevScore)
{
	prevScore = i_prevScore;
	//prevScoreText.setFont(font);
	std::string finalScoreText = "Your final score was " + std::to_string(i_prevScore) + "!\n";
	prevScoreText.setString(finalScoreText);
	//prevScoreText.setCharacterSize(25);
	//prevScoreText.setFillColor(sf::Color::White);
	//prevScoreText.setPosition(sf::Vector2f(100, 600));
}

void YouLostMenu::SetHighScores(std::string i_highScores)
{
	//highScoresText.setFont(font);
	std::string highScoreListText = i_highScores;
	highScoresText.setString(highScoreListText);
	//highScoresText.setCharacterSize(25);
	//highScoresText.setFillColor(sf::Color::White);
	//highScoresText.setPosition(sf::Vector2f(100, 700));
}

void YouLostMenu::Render(sf::RenderWindow * window)
{
	sf::Text tryAgainButtonText = tryAgainButton.GetText();
	tryAgainButtonText.setFont(font);
	sf::Text quitButtonText = quitButton.GetText();
	quitButtonText.setFont(font);

	window->clear();

	window->draw(tryAgainButton.GetRect());
	window->draw(tryAgainButtonText);
	window->draw(quitButton.GetRect());
	window->draw(quitButtonText);
	window->draw(loseText);
	window->draw(prevScoreText);
	window->draw(highScoresText);

	window->display();
}
