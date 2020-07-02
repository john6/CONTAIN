#include "YouLostMenu.h"


YouLostMenu::YouLostMenu(RESOURCES * i_resources) :
	resources{ i_resources }
{
	sf::RectangleShape playAgainButtonRect(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
	playAgainButtonRect.setPosition(sf::Vector2f(100, 100));
	tryAgainButton = Button("Try Again?", playAgainButtonRect);
	tryAgainButton.SetColors(sf::Color::Black, sf::Color::White, sf::Color(128, 128, 128));

	sf::RectangleShape quitButtonRect(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
	quitButtonRect.setPosition(sf::Vector2f(BUTTON_WIDTH * 1.35f, 100));
	quitButton = Button("Exit", quitButtonRect);
	quitButton.SetColors(sf::Color::Black, sf::Color::White, sf::Color(128, 128, 128));

	loseText.setFont(font);
	loseText.setString("You lost, Damn u suck!\n");
	loseText.setCharacterSize(25);
	loseText.setFillColor(sf::Color::White);
	loseText.setPosition(sf::Vector2f(100, 500));

	font = resources->GetFont();
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

	window->display();
}
