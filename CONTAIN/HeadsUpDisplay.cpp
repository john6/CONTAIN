#include "HeadsUpDisplay.h"

HeadsUpDisplay::HeadsUpDisplay(RESOURCES* i_resources) : resources{ i_resources }
{
	font = resources->GetFont();
	lvlNumText.setFont(font);
	timeRemainingText.setFont(font);
	healthRemainingText.setFont(font);
	BasicWeaponCharge.setFont(font);
	specialWeaponAmmo.setFont(font);

	float rectHorOffset = -10.0f;
	float rectHorExtraWidth = 10.0f;

	lvlNumRect = sf::RectangleShape(sf::Vector2f(rectHorExtraWidth + GLBVRS::HR_MRG * (6.0f / 5.0f), GLBVRS::VRT_MRG * (4.0f / 5.0f)));
	healthRemainingRect = sf::RectangleShape(sf::Vector2f(rectHorExtraWidth + GLBVRS::HR_MRG * (10.0f / 5.0f), GLBVRS::VRT_MRG * (4.0f / 5.0f)));
	specialWeaponAmmoRect = sf::RectangleShape(sf::Vector2f(rectHorExtraWidth + GLBVRS::HR_MRG * (12.0f / 5.0f), GLBVRS::VRT_MRG * (4.0f / 5.0f)));
	specialWeaponChargeRect = sf::RectangleShape(sf::Vector2f(rectHorExtraWidth + GLBVRS::HR_MRG * (12.0f / 5.0f), GLBVRS::VRT_MRG * (4.0f / 5.0f)));
	timeRemainingRects = sf::RectangleShape(sf::Vector2f(rectHorExtraWidth + GLBVRS::HR_MRG * (12.0f / 5.0f), GLBVRS::VRT_MRG * (4.0f / 5.0f)));

	float textAdjustment = (GLBVRS::SCREEN_WIDTH * (1.0f / 1920.0f));
	float lessAdjustment = ((textAdjustment + 1) / 2.0f);
	int smallText = 13 * lessAdjustment;
	int bigText = 23 * lessAdjustment;

	lvlNumText.setCharacterSize(smallText);
	timeRemainingText.setCharacterSize(bigText);
	healthRemainingText.setCharacterSize(smallText);
	BasicWeaponCharge.setCharacterSize(smallText);
	specialWeaponAmmo.setCharacterSize(smallText);
	//lvlNumText.setCharacterSize(15);
	//timeRemainingText.setCharacterSize(25);
	//healthRemainingText.setCharacterSize(15);
	//BasicWeaponCharge.setCharacterSize(15);
	//specialWeaponAmmo.setCharacterSize(15);



	lvlNumRect.setPosition(sf::Vector2f(rectHorOffset + GLBVRS::HR_MRG * (1.0f / 5.0f), 0.0f));
	lvlNumText.setPosition(sf::Vector2f(GLBVRS::HR_MRG * (1.0f / 5.0f), GLBVRS::VRT_MRG * (1.0f / 4.0f)));

	healthRemainingRect.setPosition(sf::Vector2f(rectHorOffset + GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH * (1.0f / 22.0f)), 0.0f));
	healthRemainingText.setPosition(sf::Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH * (1.0f / 22.0f)), GLBVRS::VRT_MRG * (1.0f / 4.0f)));

	specialWeaponAmmoRect.setPosition(sf::Vector2f(rectHorOffset + GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH * (4.0f / 22.0f)), 0.0f));
	specialWeaponChargeRect.setPosition(sf::Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH * (4.0f / 22.0f)), 0.0f));

	specialWeaponAmmo.setPosition(sf::Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH * (4.0f / 22.0f)), GLBVRS::VRT_MRG * (1.0f / 4.0f)));

	timeRemainingRects.setPosition(sf::Vector2f(rectHorOffset + (GLBVRS::CRT_WDTH * (20.0f / 22.0f)), 0.0f));
	timeRemainingText.setPosition(sf::Vector2f(+(GLBVRS::CRT_WDTH * (20.0f / 22.0f)), GLBVRS::VRT_MRG * (1.0f / 5.0f)));




	BasicWeaponCharge.setPosition(sf::Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH) * (3.0f / 5.0f), GLBVRS::VRT_MRG * (1.0f / 4.0f)));



	lvlNumRect.setFillColor(sf::Color::Black); 
	healthRemainingRect.setFillColor(sf::Color::Black);
	specialWeaponAmmoRect.setFillColor(sf::Color::Black);
	specialWeaponChargeRect.setFillColor(sf::Color::Red);
	timeRemainingRects.setFillColor(sf::Color::Black);
	lvlNumRect.setOutlineThickness(5);
	healthRemainingRect.setOutlineThickness(5);
	specialWeaponAmmoRect.setOutlineThickness(5);
	timeRemainingRects.setOutlineThickness(5);

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

	UpdateAOECHarge(i_player);


	PlayerChar* playerPtr = dynamic_cast<PlayerChar*>(i_player.get());
	std::string level_num_string = "LEVEL " + std::to_string((int)i_lvlNum+1);
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

void HeadsUpDisplay::UpdateAOECHarge(std::shared_ptr<Entity> i_player)
{
	PlayerChar* playerPtr = dynamic_cast<PlayerChar*>(i_player.get());
	float rectHorOffset = -10.0f;
	float rectHorExtraWidth = 10.0f;
	float weaponDelay = (std::chrono::duration_cast<std::chrono::microseconds>(hiResTime::now() - playerPtr->lastAOEFired)).count() / 1000000.0f;
	float percentCharged = std::min(weaponDelay / playerPtr->shipRateOfAOE, 1.0f);
	specialWeaponChargeRect = sf::RectangleShape((sf::Vector2f(rectHorExtraWidth + GLBVRS::HR_MRG * (12.0f / 5.0f) * percentCharged, GLBVRS::VRT_MRG * (4.0f / 5.0f))));
	specialWeaponChargeRect.setPosition(sf::Vector2f(rectHorOffset + GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH * (4.0f / 22.0f)), 0.0f));
	specialWeaponChargeRect.setFillColor(sf::Color::Red);
}

std::vector<sf::Drawable*> HeadsUpDisplay::GetDrawables()
{
	std::vector<sf::Drawable*> drawables;

	drawables.push_back(&lvlNumRect);
	drawables.push_back(&timeRemainingRects);
	drawables.push_back(&healthRemainingRect);
	drawables.push_back(&specialWeaponAmmoRect);
	drawables.push_back(&specialWeaponChargeRect);

	drawables.push_back(&lvlNumText);
	drawables.push_back(&timeRemainingText);
	drawables.push_back(&healthRemainingText);
	drawables.push_back(&BasicWeaponCharge);
	//drawables.push_back(&specialWeaponAmmo);

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