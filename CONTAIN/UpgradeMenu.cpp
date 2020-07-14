#include "UpgradeMenu.h"

UpgradeMenu::UpgradeMenu(RESOURCES * i_resources, DIFFICULTY i_diff, PlayerChar* i_player) :
	resources{ i_resources }
{
	std::random_device rd1;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen1(rd1()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> distrib(0, 1); //both boundaries are inclusive
	upgradeA = distrib(gen1);
	upgradeB = distrib(gen1);
	upgradeC = distrib(gen1);
	//need to make mutually exclusive

	sf::RectangleShape nextLevelRect(sf::Vector2f(GLOBAL_CONSTANTS::BUTTON_WIDTH, GLOBAL_CONSTANTS::BUTTON_HEIGHT));
	nextLevelRect.setPosition(sf::Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN + GLOBAL_CONSTANTS::COURT_WIDTH * 0.5f - GLOBAL_CONSTANTS::BUTTON_WIDTH * 0.5,
											GLOBAL_CONSTANTS::VERT_MARGIN + GLOBAL_CONSTANTS::COURT_HEIGHT * 0.9f));
	nextLevelButton = Button("Next Level", nextLevelRect);
	nextLevelButton.SetColors(sf::Color::Black, sf::Color::White, sf::Color(128, 128, 128));

	sf::RectangleShape upgradeRectA(sf::Vector2f(GLOBAL_CONSTANTS::COURT_WIDTH, GLOBAL_CONSTANTS::BUTTON_HEIGHT));
	upgradeRectA.setPosition(sf::Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN, GLOBAL_CONSTANTS::VERT_MARGIN + GLOBAL_CONSTANTS::BUTTON_HEIGHT * 2.0f));
	upgradeButtonA = Button(GetUpgradeText(static_cast<UPGRADE_TYPE>(upgradeA)), upgradeRectA);
	upgradeButtonA.SetColors(sf::Color::Green, sf::Color::White, sf::Color(128, 128, 128));
	upgradeButtonA.SetState(Button::UP);

	sf::RectangleShape upgradeRectB(sf::Vector2f(GLOBAL_CONSTANTS::COURT_WIDTH, GLOBAL_CONSTANTS::BUTTON_HEIGHT));
	upgradeRectB.setPosition(sf::Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN, GLOBAL_CONSTANTS::VERT_MARGIN + GLOBAL_CONSTANTS::BUTTON_HEIGHT * 4.0f));
	upgradeButtonB = Button(GetUpgradeText(static_cast<UPGRADE_TYPE>(upgradeB)), upgradeRectB);
	upgradeButtonB.SetColors(sf::Color::Yellow, sf::Color::White, sf::Color(128, 128, 128));
	upgradeButtonB.SetState(Button::UP);

	sf::RectangleShape upgradeRectC(sf::Vector2f(GLOBAL_CONSTANTS::COURT_WIDTH, GLOBAL_CONSTANTS::BUTTON_HEIGHT));
	upgradeRectC.setPosition(sf::Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN, GLOBAL_CONSTANTS::VERT_MARGIN + GLOBAL_CONSTANTS::BUTTON_HEIGHT * 6.0f));
	upgradeButtonC = Button(GetUpgradeText(static_cast<UPGRADE_TYPE>(upgradeC)), upgradeRectC);
	upgradeButtonC.SetColors(sf::Color::Red, sf::Color::White, sf::Color(128, 128, 128));
	upgradeButtonC.SetState(Button::UP);


	font = resources->GetFont();

	upgradeText.setFont(font);
	upgradeText.setString("Choose and upgrade before advancing to the next level!");
	upgradeText.setCharacterSize(25);
	upgradeText.setFillColor(sf::Color::White);
	upgradeText.setPosition(sf::Vector2f(GLOBAL_CONSTANTS::HOR_MARGIN, GLOBAL_CONSTANTS::VERT_MARGIN));
	
	firstOpen = true;
	currUpgradeType = static_cast<UPGRADE_TYPE>(upgradeA);
}

bool UpgradeMenu::PollInput(sf::Vector2i mousePosition, Button * button, bool stickyButton)
{
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

void UpgradeMenu::PollButtonTriplet(sf::Vector2i mousePosition)
{
	if (PollInput(mousePosition, &upgradeButtonA, true)) { UpdateButtonTriplet(EASY); }
	else if (PollInput(mousePosition, &upgradeButtonB, true)) { UpdateButtonTriplet(MEDIUM); }
	else if (PollInput(mousePosition, &upgradeButtonC, true)) { UpdateButtonTriplet(HARD); }
}

void UpgradeMenu::UpdateButtonTriplet(int i_upgradeSelected)
{
	switch (i_upgradeSelected) {
	case 1: {
		currUpgradeType = static_cast<UPGRADE_TYPE>(upgradeA);
		upgradeButtonA.SetState(Button::DOWN);
		upgradeButtonB.SetState(Button::UP);
		upgradeButtonC.SetState(Button::UP);
		break;
	}
	case 2: {
		currUpgradeType = static_cast<UPGRADE_TYPE>(upgradeB);
		upgradeButtonA.SetState(Button::UP);
		upgradeButtonB.SetState(Button::DOWN);
		upgradeButtonC.SetState(Button::UP);
		break;
	}
	case 3: {
		currUpgradeType = static_cast<UPGRADE_TYPE>(upgradeC);
		upgradeButtonA.SetState(Button::UP);
		upgradeButtonB.SetState(Button::UP);
		upgradeButtonC.SetState(Button::DOWN);
		break;
	}
	}
}

std::string UpgradeMenu::GetUpgradeText(UPGRADE_TYPE i_type)
{
	std::string upgradeString = "";
	switch (i_type) {
	case RATE_OF_FIRE : {
		upgradeString = "double rate of fire";
			break;
		}
	case SCATTER : {
		upgradeString = "Add one bullet to burst";
		break;
	}
	case SMALL_SHIP: {
		upgradeString = "decrease ship size, increase speed";
		break;
	}
	case BLAST : {
		upgradeString = "Make blast larger";
		break;
	}
	case WALL_BIG : {
		upgradeString = "increase wall size";
		break;
	}
	case WALL_HURT: {
		upgradeString = "make walls hurt enemies";
		break;
	}
	}
	return upgradeString;
}

UpgradeMenu::~UpgradeMenu()
{
}

int UpgradeMenu::Update(float microSeconds, sf::RenderWindow * window, sf::Vector2i i_mousePos)
{
	if (firstOpen) {
		firstOpen = false;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	else {
		PollButtonTriplet(i_mousePos);
		bool nextLevelButtonPressed = PollInput(i_mousePos, &nextLevelButton);
		if (nextLevelButtonPressed) {
			resources->PlaySound(RESOURCES::OCTAVE_BEEP);
			return currUpgradeType;
		}
		else {
			return -1;
		}
	}
	return -1;
}

void UpgradeMenu::Render(sf::RenderWindow * window)
{
	sf::Text nextLevelText = nextLevelButton.GetText();
	sf::Text textA = upgradeButtonA.GetText();
	sf::Text textB = upgradeButtonB.GetText();
	sf::Text textC = upgradeButtonC.GetText();
	nextLevelText.setFont(font);
	textA.setFont(font);
	textB.setFont(font);
	textC.setFont(font);

	window->clear();

	window->draw(nextLevelButton.GetRect());
	window->draw(upgradeButtonA.GetRect());
	window->draw(upgradeButtonB.GetRect());
	window->draw(upgradeButtonC.GetRect());
	window->draw(nextLevelText);
	window->draw(textA);
	window->draw(textB);
	window->draw(textC);
	window->draw(upgradeText);

	window->display();
}
