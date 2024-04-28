#include "Menu.h"
#include "GLOBAL_CONSTANTS.h"

Menu::Menu(RESOURCES* i_resources, DIFFICULTY i_defaultDiff) :
	resources {i_resources}
{
	font = resources->GetFont();

	lastButtonPressed = std::chrono::high_resolution_clock::now();
	buttonClickDelay = 0.35f;
	timeSinceButtonClick = buttonClickDelay;
	somethingWasClicked = false;

	sf::Vector2f topLeft(GLBVRS::HR_MRG * 4.5f, GLBVRS::VRT_MRG * 4.0f);
	float bttnMrgHr = (GLBVRS::HR_MRG / 4.0f);
	float bttnMrgVrt = (GLBVRS::VRT_MRG / 3.0f);
	float bttnOffsetHor = GLBVRS::BTTN_WDTH + bttnMrgHr;
	float bttnOffsetVert = GLBVRS::BTTN_HGHT + bttnMrgVrt;
	//top row
	sf::RectangleShape playButtonRect(sf::Vector2f(GLBVRS::BTTN_WDTH, GLBVRS::BTTN_HGHT));
	playButtonRect.setPosition(topLeft + sf::Vector2f(bttnOffsetHor, 0));
	playButton = Button("Play", playButtonRect, &font);
	playButton.SetColors(sf::Color::Black, sf::Color::White, sf::Color(128, 128, 128));


	//row 2
	sf::RectangleShape easyButtonRect(sf::Vector2f(GLBVRS::BTTN_WDTH, GLBVRS::BTTN_HGHT));
	easyButtonRect.setPosition(topLeft + sf::Vector2f(0, bttnOffsetVert * 1.5f));
	easyButton = Button("Easy", easyButtonRect, &font);
	easyButton.SetColors(sf::Color::Green, sf::Color::White, sf::Color(128, 128, 128));
	easyButton.SetState(Button::UP);

	sf::RectangleShape mediumButtonRect(sf::Vector2f(GLBVRS::BTTN_WDTH, GLBVRS::BTTN_HGHT));
	mediumButtonRect.setPosition(topLeft + sf::Vector2f(bttnOffsetHor * 1, bttnOffsetVert * 1.5f));
	mediumButton = Button("Medium", mediumButtonRect, &font);
	mediumButton.SetColors(sf::Color::Yellow, sf::Color::White, sf::Color(128, 128, 128));
	mediumButton.SetState(Button::UP);

	sf::RectangleShape hardButtonRect(sf::Vector2f(GLBVRS::BTTN_WDTH, GLBVRS::BTTN_HGHT));
	hardButtonRect.setPosition(topLeft + sf::Vector2f(bttnOffsetHor * 2, bttnOffsetVert * 1.5f));
	hardButton = Button("Hard", hardButtonRect, &font);
	hardButton.SetColors(sf::Color::Red, sf::Color::White, sf::Color(128, 128, 128));
	hardButton.SetState(Button::UP);


	//row three
	sf::RectangleShape godButtonRect(sf::Vector2f(GLBVRS::BTTN_WDTH * 1.2f, GLBVRS::BTTN_HGHT));
	godButtonRect.setPosition(topLeft + sf::Vector2f(bttnOffsetHor * 0.2, bttnOffsetVert * 3.0f));
	godModeButton = Button("God Mode", godButtonRect, &font);
	godModeButton.SetColors(sf::Color::Black, sf::Color::White, sf::Color(128, 128, 128));
	godModeButton.SetState(Button::UP);

	sf::RectangleShape endlessModeButtonRect(sf::Vector2f(GLBVRS::BTTN_WDTH * 1.2f, GLBVRS::BTTN_HGHT));
	endlessModeButtonRect.setPosition(topLeft + sf::Vector2f(bttnOffsetHor * 1.6, bttnOffsetVert * 3.0f));
	endlessModeButton = Button("Endless Mode", endlessModeButtonRect, &font);
	endlessModeButton.SetColors(sf::Color::Black, sf::Color::White, sf::Color(128, 128, 128));
	endlessModeButton.SetState(Button::UP);

	//row four
	sf::RectangleShape playTutorialButtonRect(sf::Vector2f(GLBVRS::BTTN_WDTH, GLBVRS::BTTN_HGHT));
	playTutorialButtonRect.setPosition(topLeft + sf::Vector2f(0, bttnOffsetVert * 4.5f));
	playTutorialButton = Button("Tutorial", playTutorialButtonRect, &font);
	playTutorialButton.SetColors(sf::Color::Black, sf::Color::White, sf::Color(128, 128, 128));

	sf::RectangleShape settingsButtonRect(sf::Vector2f(GLBVRS::BTTN_WDTH, GLBVRS::BTTN_HGHT));
	settingsButtonRect.setPosition(topLeft + sf::Vector2f(bttnOffsetHor * 1, bttnOffsetVert * 4.5f));
	settingsButton = Button("Settings", settingsButtonRect, &font);
	settingsButton.SetColors(sf::Color::Black, sf::Color::White, sf::Color(128, 128, 128));
	settingsButton.SetState(Button::UP);

	sf::RectangleShape exitButtonRect(sf::Vector2f(GLBVRS::BTTN_WDTH, GLBVRS::BTTN_HGHT));
	exitButtonRect.setPosition(topLeft + sf::Vector2f(bttnOffsetHor * 2, bttnOffsetVert * 4.5f));
	exitButton = Button("Exit", exitButtonRect, &font);
	exitButton.SetColors(sf::Color::Black, sf::Color::White, sf::Color(128, 128, 128));

	currDifficulty = i_defaultDiff;
	UpdateButtonTriplet(i_defaultDiff);


	title.setFont(font);
	title.setString("CONTAIN");
	title.setCharacterSize(75);
	title.setFillColor(sf::Color::White);
	GLBVRS::SetTextOriginCenter(&title);
	title.setPosition(sf::Vector2f(GLBVRS::HR_MRG + GLBVRS::CRT_WDTH / 2.0f, GLBVRS::VRT_MRG * 1.5f));

	instructions1.setFont(font);
	instructions1.setString("Controls: \n#WASD or LEFT STICK# to move\n#LEFT CLICK or RIGHT TRIGGER# to shoot\n#RIGHT CLICK or LEFT TRIGGER# to deploy a wall\n#SPACEBAR or RIGHT STICK CLICK# to create an stun blast\n");
	instructions1.setCharacterSize(25);
	instructions1.setFillColor(sf::Color::White);
	instructions1.setPosition(topLeft + sf::Vector2f(0, bttnOffsetVert * 6));
}

Menu::~Menu()
{
}

DIFFICULTY Menu::GetDifficulty() { return currDifficulty; }

void Menu::ResetMenu()
{
	//playButton.SetState(Button::UP);
	//playTutorialButton.SetState(Button::UP);
	//exitButton.SetState(Button::UP);
	//easyButton.SetState(Button::UP);
	//mediumButton.SetState(Button::UP);
	//hardButton.SetState(Button::UP);
	//settingsButton.SetState(Button::UP);
	std::this_thread::sleep_for(std::chrono::milliseconds(350));
}

bool Menu::PollInput(sf::Vector2f mousePosition, Button* button, bool stickyButton) {
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

void  Menu::PollButtonTriplet(sf::Vector2f mousePosition) {
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

bool Menu::PollInputToggle(sf::Vector2f mousePosition, Button * button)
{
	float halfWidth = (button->GetRect().getSize().x / 2);
	float halfHeight = (button->GetRect().getSize().y / 2);
	float distX = abs(mousePosition.x - (button->GetRect().getPosition().x + halfWidth));
	float distY = abs(mousePosition.y - (button->GetRect().getPosition().y + halfHeight));
	if ((distX <= halfWidth) && (distY <= halfHeight)) {
		//button->SetState(Button::HOVER);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			somethingWasClicked = true;
			if (button->GetState() == Button::DOWN) {
				button->SetState(Button::UP);
				return false;
			}
			else {
				button->SetState(Button::DOWN);
				return true;
			}
		}

	}
	else {
		if (button->GetState() == Button::DOWN) {
			return true;
		}
		else {
			return false;
		}
	}
}

GAME_STATE Menu::Update(float i_microSecs, sf::RenderWindow* i_window, sf::Vector2f i_mousePos) {
	timeSinceButtonClick = (std::chrono::duration_cast<std::chrono::microseconds>(hiResTime::now() - lastButtonPressed)).count() / 1000000.0f;
	if (GLBVRS::canPressButtonsAgain) {
		bool playButtonPressed = false;
		bool playTutorialButtonPressed = false;
		bool exitButtonPressed = false;
		bool settingsButtonPressed = false;
		if ((timeSinceButtonClick >= buttonClickDelay)) {
			PollButtonTriplet(i_mousePos);
			playButtonPressed = PollInput(i_mousePos, &playButton);
			playTutorialButtonPressed = PollInput(i_mousePos, &playTutorialButton);
			exitButtonPressed = PollInput(i_mousePos, &exitButton);
			settingsButtonPressed = PollInput(i_mousePos, &settingsButton);

			bool godModeBool = PollInputToggle(i_mousePos, &godModeButton);
			bool endlessModeBool = PollInputToggle(i_mousePos, &endlessModeButton);
			GLBVRS::godMode = godModeBool;
			GLBVRS::endlessMode = endlessModeBool;

			if (somethingWasClicked) {
				lastButtonPressed = hiResTime::now();
				somethingWasClicked = false;
			}
		}
		if (playButtonPressed) {
			resources->PlaySound(RESOURCES::OCTAVE_BEEP);
			return START_GAME;
		}
		if (playTutorialButtonPressed) {
			resources->PlaySound(RESOURCES::OCTAVE_BEEP);
			return START_TUTORIAL;
		}
		else if (settingsButtonPressed) {
			resources->PlaySound(RESOURCES::OCTAVE_BEEP);
			return SETTINGS;
		}
		else if (exitButtonPressed) {
			resources->PlaySound(RESOURCES::OCTAVE_BEEP);
			return EXIT_GAME;
		}
		else {
			return MENU;
		}
	}
	else { return MENU; }
}

void Menu::Render(sf::RenderWindow* window) {
	sf::Text play = playButton.GetText();
	
	sf::Text playTutorial = playTutorialButton.GetText();
	sf::Text exit = exitButton.GetText();
	
	sf::Text easy = easyButton.GetText();
	sf::Text medium = mediumButton.GetText();
	sf::Text hard = hardButton.GetText();

	sf::Text godModeText = godModeButton.GetText();
	sf::Text endlessModeText = endlessModeButton.GetText();

	sf::Text settingsText = settingsButton.GetText();

	window->clear();

	window->draw(playButton.GetRect());
	window->draw(playTutorialButton.GetRect());
	window->draw(exitButton.GetRect());
	window->draw(easyButton.GetRect());
	window->draw(mediumButton.GetRect());
	window->draw(hardButton.GetRect());

	window->draw(godModeButton.GetRect());
	window->draw(endlessModeButton.GetRect());


	window->draw(settingsButton.GetRect());
	window->draw(play);
	window->draw(playTutorial);
	window->draw(exit);
	window->draw(easy);
	window->draw(medium);
	window->draw(hard);

	window->draw(godModeText);
	window->draw(endlessModeText);

	window->draw(settingsText);
	window->draw(title);
	window->draw(instructions1);
	window->draw(instructions2);

	window->display();
}
