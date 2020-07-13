#include "Menu.h"
#include "GLOBAL_CONSTANTS.h"

Menu::Menu(RESOURCES* i_resources, DIFFICULTY i_defaultDiff) :
	resources {i_resources}
{

	sf::RectangleShape playButtonRect(sf::Vector2f(GLOBAL_CONSTANTS::BUTTON_WIDTH, GLOBAL_CONSTANTS::BUTTON_HEIGHT));
	playButtonRect.setPosition(sf::Vector2f(100, 100));
	playButton = Button("Play", playButtonRect);
	playButton.SetColors(sf::Color::Black, sf::Color::White, sf::Color(128, 128, 128));

	sf::RectangleShape exitButtonRect(sf::Vector2f(GLOBAL_CONSTANTS::BUTTON_WIDTH, GLOBAL_CONSTANTS::BUTTON_HEIGHT));
	exitButtonRect.setPosition(sf::Vector2f(GLOBAL_CONSTANTS::BUTTON_WIDTH * 1.35f, 100));
	exitButton = Button("Exit", exitButtonRect);
	exitButton.SetColors(sf::Color::Black, sf::Color::White, sf::Color(128, 128, 128));

	sf::RectangleShape easyButtonRect(sf::Vector2f(GLOBAL_CONSTANTS::BUTTON_WIDTH * .75f, GLOBAL_CONSTANTS::BUTTON_HEIGHT));
	easyButtonRect.setPosition(sf::Vector2f(70, 150 + GLOBAL_CONSTANTS::BUTTON_HEIGHT));
	easyButton = Button("Easy", easyButtonRect);
	easyButton.SetColors(sf::Color::Green, sf::Color::White, sf::Color(128, 128, 128));
	easyButton.SetState(Button::UP);

	sf::RectangleShape mediumButtonRect(sf::Vector2f(GLOBAL_CONSTANTS::BUTTON_WIDTH * .75f, GLOBAL_CONSTANTS::BUTTON_HEIGHT));
	mediumButtonRect.setPosition(sf::Vector2f(70 + (GLOBAL_CONSTANTS::BUTTON_WIDTH * 0.85f), 150 + GLOBAL_CONSTANTS::BUTTON_HEIGHT));
	mediumButton = Button("Medium", mediumButtonRect);
	mediumButton.SetColors(sf::Color::Yellow, sf::Color::White, sf::Color(128, 128, 128));
	mediumButton.SetState(Button::UP);

	sf::RectangleShape hardButtonRect(sf::Vector2f(GLOBAL_CONSTANTS::BUTTON_WIDTH * .75f, GLOBAL_CONSTANTS::BUTTON_HEIGHT));
	hardButtonRect.setPosition(sf::Vector2f(70 + (GLOBAL_CONSTANTS::BUTTON_WIDTH * 1.70f), 150 + GLOBAL_CONSTANTS::BUTTON_HEIGHT));
	hardButton = Button("Hard", hardButtonRect);
	hardButton.SetColors(sf::Color::Red, sf::Color::White, sf::Color(128, 128, 128));
	hardButton.SetState(Button::UP);

	currDifficulty = i_defaultDiff;
	UpdateButtonTriplet(i_defaultDiff);

	font = resources->GetFont();

	instructions1.setFont(font);
	instructions1.setString("Controls: \n#WASD# to move,\n#Left Click# to shoot a basic basic projectile\n");
	instructions1.setCharacterSize(25);
	instructions1.setFillColor(sf::Color::White);
	instructions1.setPosition(sf::Vector2f(100, 500));
}

Menu::~Menu()
{
}

DIFFICULTY Menu::GetDifficulty() { return currDifficulty; }

void Menu::ResetMenu()
{
	playButton.SetState(Button::UP);
	exitButton.SetState(Button::UP);
	easyButton.SetState(Button::UP);
	mediumButton.SetState(Button::UP);
	hardButton.SetState(Button::UP);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

bool Menu::PollInput(sf::Vector2i mousePosition, Button* button, bool stickyButton) {
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

void  Menu::PollButtonTriplet(sf::Vector2i mousePosition) {
	if (PollInput(mousePosition, &easyButton, true)) { UpdateButtonTriplet(EASY);  }
	else if (PollInput(mousePosition, &mediumButton, true)) { UpdateButtonTriplet(MEDIUM); }
	else if (PollInput(mousePosition, &hardButton, true)) { UpdateButtonTriplet(HARD); }
}

void  Menu::UpdateButtonTriplet(DIFFICULTY i_difficultySelected) {
	switch (i_difficultySelected) {
		case EASY: {
			currDifficulty = EASY;
			easyButton.SetState(Button::DOWN);
			mediumButton.SetState(Button::UP);
			hardButton.SetState(Button::UP);
			break;
		}
		case MEDIUM: {
		currDifficulty = MEDIUM;
		easyButton.SetState(Button::UP);
		mediumButton.SetState(Button::DOWN);
		hardButton.SetState(Button::UP);
		break;
		}
		case HARD: {
		currDifficulty = HARD;
		easyButton.SetState(Button::UP);
		mediumButton.SetState(Button::UP);
		hardButton.SetState(Button::DOWN);
		break;
		}
	}
}

GAME_STATE Menu::Update(float i_microSecs, sf::RenderWindow* i_window, sf::Vector2i i_mousePos) {
	PollButtonTriplet(i_mousePos);
	bool playButtonPressed = PollInput(i_mousePos, &playButton);
	bool exitButtonPressed = PollInput(i_mousePos, &exitButton);

	if (playButtonPressed) {
		resources->PlaySound(RESOURCES::OCTAVE_BEEP);
		return START_GAME;
	}
	else if (exitButtonPressed) {
		resources->PlaySound(RESOURCES::OCTAVE_BEEP);
		return EXIT_GAME;
	}
	else { 
		return MENU; 
	}
}

void Menu::Render(sf::RenderWindow* window) {
	sf::Text play = playButton.GetText();
	sf::Text exit = exitButton.GetText();
	sf::Text easy = easyButton.GetText();
	sf::Text medium = mediumButton.GetText();
	sf::Text hard = hardButton.GetText();
	play.setFont(font);
	exit.setFont(font);
	easy.setFont(font);
	medium.setFont(font);
	hard.setFont(font);

	window->clear();

	window->draw(playButton.GetRect());
	window->draw(exitButton.GetRect());
	window->draw(easyButton.GetRect());
	window->draw(mediumButton.GetRect());
	window->draw(hardButton.GetRect());
	window->draw(play);
	window->draw(exit);
	window->draw(easy);
	window->draw(medium);
	window->draw(hard);
	window->draw(instructions1);

	window->display();
}
