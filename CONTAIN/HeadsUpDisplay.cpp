#include "HeadsUpDisplay.h"

HeadsUpDisplay::HeadsUpDisplay(RESOURCES* i_resources) : resources{ i_resources }
{
	font = resources->GetFont();

	lvlNumText.setFont(font);
	timeRemainingText.setFont(font);
	healthRemainingText.setFont(font);
	BasicWeaponCharge.setFont(font);
	specialWeaponAmmo.setFont(font);

	lvlNumText.setCharacterSize(15);
	timeRemainingText.setCharacterSize(25);
	healthRemainingText.setCharacterSize(15);
	BasicWeaponCharge.setCharacterSize(15);
	specialWeaponAmmo.setCharacterSize(15);

	lvlNumText.setPosition(sf::Vector2f(GLBVRS::HR_MRG * (1.0f/5.0f), GLBVRS::VRT_MRG * (1.0f / 4.0f)));
	timeRemainingText.setPosition(sf::Vector2f(+(GLBVRS::CRT_WDTH * (4.0f / 5.0f)), GLBVRS::VRT_MRG * (1.0f/5.0f)));
	healthRemainingText.setPosition(sf::Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH * (1.0f / 20.0f)), GLBVRS::VRT_MRG * (1.0f / 4.0f)));
	BasicWeaponCharge.setPosition(sf::Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH) * (3.0f / 5.0f), GLBVRS::VRT_MRG * (1.0f / 4.0f)));
	specialWeaponAmmo.setPosition(sf::Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH * (5.0f/20.0f)), GLBVRS::VRT_MRG * (1.0f / 4.0f)));

	lvlNumText.setFillColor(sf::Color::White);
	timeRemainingText.setFillColor(sf::Color::White);
	healthRemainingText.setFillColor(sf::Color::White);
	BasicWeaponCharge.setFillColor(sf::Color::White);
	specialWeaponAmmo.setFillColor(sf::Color::White);

	lvlNumText.setString("LEVEL: INIT");
	timeRemainingText.setString("TIME: INIT");
	healthRemainingText.setString("HEALTH: INIT");
	BasicWeaponCharge.setString("PRIMARY WEAPON: READY");
	specialWeaponAmmo.setString("SPECIAL WEAPON AMMO: 3");
}

HeadsUpDisplay::~HeadsUpDisplay()
{
}

void HeadsUpDisplay::Update(int i_lvlNum, int i_timeRemaining, int i_maxTime, std::shared_ptr<Entity> i_player) {
	PlayerChar* playerPtr = dynamic_cast<PlayerChar*>(i_player.get());
	std::string level_num_string = "LEVEL " + std::to_string((int)i_lvlNum);
	lvlNumText.setString(level_num_string);

	std::string time_string = "TIME: " + std::to_string((int)i_timeRemaining);
	timeRemainingText.setString(time_string);
	if (i_timeRemaining >= i_maxTime) {
		timeRemainingText.setFillColor(sf::Color::White);
	}
	else {
		timeRemainingText.setFillColor(sf::Color::Red);
	}

	std::string balls_str = "HEALTH " + std::to_string((int)playerPtr->GetCurrHealth()) + "/" + std::to_string((int)playerPtr->maxHealth);
	healthRemainingText.setString(balls_str);

	//	std::string primWeapStr = "PRIMARY WEAPON : " + std::to_string((int)playerPtr->weaponDelay);
	std::string primWeapStr = "";

	BasicWeaponCharge.setString(primWeapStr);

	std::string secWeapStr = "STUN BLAST " + std::to_string((int)playerPtr->currSpecialAmmo) + "/" + std::to_string((int)playerPtr->maxSpecialAmmo);
	specialWeaponAmmo.setString(secWeapStr);
}

std::vector<sf::Drawable*> HeadsUpDisplay::GetDrawables()
{
	std::vector<sf::Drawable*> drawables;
	drawables.push_back(&lvlNumText);
	drawables.push_back(&timeRemainingText);
	drawables.push_back(&healthRemainingText);
	drawables.push_back(&BasicWeaponCharge);
	drawables.push_back(&specialWeaponAmmo);
	return drawables;
}

//void HeadsUpDisplay::RenderHUD(sf::RenderWindow * i_window, float i_lerp_fraction, std::shared_ptr<Entity> i_player, Level* i_lvl)
//{
//	i_window->draw(lvlNumText);
//	i_window->draw(timeRemainingText);
//	i_window->draw(livesRemainingText);
//	i_window->draw(enemiesRemainingText);
//
//	//i_window->display();
//}