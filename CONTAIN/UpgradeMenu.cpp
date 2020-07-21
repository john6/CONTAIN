#include "UpgradeMenu.h"

UpgradeMenu::UpgradeMenu(RESOURCES * i_resources, DIFFICULTY i_diff, PlayerChar* i_player) :
	resources{ i_resources }
{
	std::random_device rd1;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen1(rd1()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> distrib(1, 6); //both boundaries are inclusive
	auto playerLvls = i_player->GetLvl();

	std::vector<UPGRADE_TYPE> availableUpgrades;
	for (int upInt = 0; upInt != WALL_BIG + 1; ++upInt) {
		if (playerLvls->at((UPGRADE_TYPE)upInt) < GLBVRS::GetUpgradeMax((UPGRADE_TYPE)upInt)) {
			availableUpgrades.push_back((UPGRADE_TYPE)upInt);
		}
	}
	upgradeA = availableUpgrades[distrib(gen1)%availableUpgrades.size()];
	auto iter1 = std::find(availableUpgrades.begin(), availableUpgrades.end(), upgradeA);
	availableUpgrades.erase(iter1);
	upgradeB = availableUpgrades[distrib(gen1) % availableUpgrades.size()];
	auto iter2 = std::find(availableUpgrades.begin(), availableUpgrades.end(), upgradeB);
	availableUpgrades.erase(iter2);
	upgradeC = availableUpgrades[distrib(gen1) % availableUpgrades.size()];
	auto iter3 = std::find(availableUpgrades.begin(), availableUpgrades.end(), upgradeC);
	availableUpgrades.erase(iter3);

	sf::RectangleShape nextLevelRect(sf::Vector2f(GLBVRS::BTTN_WDTH, GLBVRS::BTTN_HGHT));
	nextLevelRect.setPosition(sf::Vector2f(GLBVRS::HR_MRG + GLBVRS::CRT_WDTH * 0.5f - GLBVRS::BTTN_WDTH * 0.5,
											GLBVRS::VRT_MRG + GLBVRS::CRT_HGHT * 0.9f));
	nextLevelButton = Button("Next Level", nextLevelRect);
	nextLevelButton.SetColors(sf::Color::Black, sf::Color::White, sf::Color(128, 128, 128));

	sf::RectangleShape upgradeRectA(sf::Vector2f(GLBVRS::CRT_WDTH, GLBVRS::BTTN_HGHT));
	upgradeRectA.setPosition(sf::Vector2f(GLBVRS::HR_MRG, GLBVRS::VRT_MRG + GLBVRS::BTTN_HGHT * 2.0f));
	upgradeButtonA = Button(GetUpgradeText(static_cast<UPGRADE_TYPE>(upgradeA)), upgradeRectA);
	upgradeButtonA.SetColors(sf::Color::Green, sf::Color::White, sf::Color(128, 128, 128));
	upgradeButtonA.SetState(Button::UP);

	sf::RectangleShape upgradeRectB(sf::Vector2f(GLBVRS::CRT_WDTH, GLBVRS::BTTN_HGHT));
	upgradeRectB.setPosition(sf::Vector2f(GLBVRS::HR_MRG, GLBVRS::VRT_MRG + GLBVRS::BTTN_HGHT * 4.0f));
	upgradeButtonB = Button(GetUpgradeText(static_cast<UPGRADE_TYPE>(upgradeB)), upgradeRectB);
	upgradeButtonB.SetColors(sf::Color::Yellow, sf::Color::White, sf::Color(128, 128, 128));
	upgradeButtonB.SetState(Button::UP);

	sf::RectangleShape upgradeRectC(sf::Vector2f(GLBVRS::CRT_WDTH, GLBVRS::BTTN_HGHT));
	upgradeRectC.setPosition(sf::Vector2f(GLBVRS::HR_MRG, GLBVRS::VRT_MRG + GLBVRS::BTTN_HGHT * 6.0f));
	upgradeButtonC = Button(GetUpgradeText(static_cast<UPGRADE_TYPE>(upgradeC)), upgradeRectC);
	upgradeButtonC.SetColors(sf::Color::Red, sf::Color::White, sf::Color(128, 128, 128));
	upgradeButtonC.SetState(Button::UP);

	font = resources->GetFont();

	upgradeText.setFont(font);
	upgradeText.setString("Choose and upgrade before advancing to the next level!");
	upgradeText.setCharacterSize(25);
	upgradeText.setFillColor(sf::Color::White);
	upgradeText.setPosition(sf::Vector2f(GLBVRS::HR_MRG, GLBVRS::VRT_MRG));
	
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
	if (PollInput(mousePosition, &upgradeButtonA, true)) { UpdateButtonTriplet(1); }
	else if (PollInput(mousePosition, &upgradeButtonB, true)) { UpdateButtonTriplet(2); }
	else if (PollInput(mousePosition, &upgradeButtonC, true)) { UpdateButtonTriplet(3); }
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
	case NONE: {
		upgradeString = "Big oopsie everyone this is an error";
		break;
	}
	case WEAP_SPEED: {
		upgradeString = "Increase bullet speed";
		break;
	}
	case RATE_OF_FIRE : {
		upgradeString = "Increase rate of fire";
			break;
		}
	case SCATTER : {
		upgradeString = "Add one bullet to burst";
		break;
	}
	case SMALL_SHIP: {
		upgradeString = "Decrease ship size, increase speed";
		break;
	}
	case BIG_SHIP: {
		upgradeString = "Increase ship size, increase health";
		break;
	}
	case BLAST : {
		upgradeString = "Make EMP blast larger and more powerful";
		break;
	}
	case WALL_BIG : {
		upgradeString = "Increase wall size";
		break;
	}
	//case WALL_HURT: {
	//	upgradeString = "make walls hurt enemies";
	//	break;
	//}
	}
	return upgradeString;
}

UpgradeMenu::~UpgradeMenu()
{
}

UPGRADE_TYPE UpgradeMenu::Update(float microSeconds, sf::RenderWindow * window, sf::Vector2i i_mousePos)
{
	if (firstOpen) {
		firstOpen = false;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	else {
		PollButtonTriplet(i_mousePos);
		bool nextLevelButtonPressed = PollInput(i_mousePos, &nextLevelButton);
		if (nextLevelButtonPressed) {
			resources->PlaySound(RESOURCES::SOUNDS::MAJORPICKUP);
			return currUpgradeType;
		}
		else {
			return NONE;
		}
	}
	return NONE;
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
