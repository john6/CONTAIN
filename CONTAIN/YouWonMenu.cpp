#include "YouWonMenu.h"


YouWonMenu::YouWonMenu(RESOURCES * i_resources) :
	resources{ i_resources }
{
	sf::RectangleShape playAgainButtonRect(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
	playAgainButtonRect.setPosition(sf::Vector2f(100, 100));
	playAgainButton = Button("Play Again?", playAgainButtonRect);
	playAgainButton.SetColors(sf::Color::Black, sf::Color::White, sf::Color(128, 128, 128));

	sf::RectangleShape quitButtonRect(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
	quitButtonRect.setPosition(sf::Vector2f(BUTTON_WIDTH * 1.35f, 100));
	quitButton = Button("Exit", quitButtonRect);
	quitButton.SetColors(sf::Color::Black, sf::Color::White, sf::Color(128, 128, 128));

	winText.setFont(font);
	winText.setString("You beat the game, wow congrats bud!\n");
	winText.setCharacterSize(25);
	winText.setFillColor(sf::Color::White);
	winText.setPosition(sf::Vector2f(100, 500));

	font = resources->GetFont();
}

YouWonMenu::~YouWonMenu()
{
}

bool YouWonMenu::PollInput(sf::Vector2i mousePosition, Button * button, bool stickyButton){
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

GAME_STATE YouWonMenu::Update(float microSeconds, sf::RenderWindow * window, sf::Vector2i mousePosition)
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
		return WIN;
	}
}

void YouWonMenu::Render(sf::RenderWindow * window)
{
	sf::Text playAgainText = playAgainButton.GetText();
	playAgainText.setFont(font);
	sf::Text quitButtonText = quitButton.GetText();
	quitButtonText.setFont(font);

	window->clear();

	window->draw(playAgainButton.GetRect());
	window->draw(playAgainText);
	window->draw(quitButton.GetRect());
	window->draw(quitButtonText);
	window->draw(winText);

	window->display();
}
