#include "HeadsUpDisplay.h"

HeadsUpDisplay::HeadsUpDisplay(RESOURCES* i_resources) :
	resources {i_resources}
{
	font = resources->GetFont();
	lvlNumText.setFont(font);
	healthLabelText.setFont(font);
	spclWpLblTxt.setFont(font);
	timeRemainingText.setFont(font);

	hudHorMargin = GLBVRS::HR_MRG * (1.0f / 16.0f);
	hudHorSize = GLBVRS::CRT_WDTH + GLBVRS::HR_MRG * (2.0f) - hudHorMargin * 2.0f;
	hudVertSize = GLBVRS::VRT_MRG * (0.9f);
	hudItemSpacing = hudHorSize * (1.0f / 200.0f);
	hudItemRectHeight = hudVertSize * (6.0f / 10.0f);
	hudFillBarHeight = hudItemRectHeight * (8.0 / 10.0f);
	textHorOffset = 6.0f;
	textVertOffset = hudItemRectHeight / 4.0f;
	textOffset = sf::Vector2f(textHorOffset, textVertOffset);
	rectHorExtraWidth = 4.0f;
	hudOutLineThickness = 1;
	hudItemOutLineThickness = 1;
	hudTopLeft = sf::Vector2f(hudHorMargin, GLBVRS::CRT_HGHT + GLBVRS::VRT_MRG * (3.0f) - hudVertSize - hudOutLineThickness * 3.0f);
	hudItemsOffset = sf::Vector2f(hudItemSpacing, (hudVertSize - hudItemRectHeight) / 2.0f);
	hudItemPosition = hudTopLeft + hudItemsOffset;

	float textAdjustment = (GLBVRS::SCREEN_WIDTH * (1.0f / 1920.0f));
	float lessAdjustment = ((textAdjustment + 1) / 2.0f);
	int smallText = 13 * lessAdjustment;
	int bigText = 23 * lessAdjustment;
	segmentMarg = 1;


	backGroundRect = sf::RectangleShape(sf::Vector2f(hudHorSize, hudVertSize));
	lvlNumRect = sf::RectangleShape(sf::Vector2f(rectHorExtraWidth + hudHorSize * (11.0f / 200.0f), hudItemRectHeight));
	healthLabelRect = sf::RectangleShape(sf::Vector2f(rectHorExtraWidth + hudHorSize * (6.0f / 200.0f), hudItemRectHeight));
	healthRectWidth = rectHorExtraWidth + hudHorSize * (75.0f / 200.0f);
	healthSegmentsRect = sf::RectangleShape(sf::Vector2f(healthRectWidth + (segmentMarg * 3.0f), hudItemRectHeight));
	spclWpLblRct = sf::RectangleShape(sf::Vector2f(rectHorExtraWidth + hudHorSize * (6.0f / 200.0f), hudItemRectHeight));
	specialWeaponRect = sf::RectangleShape(sf::Vector2f(rectHorExtraWidth + hudHorSize * (25.0f / 200.0f), hudItemRectHeight));
	specialWeaponCurrChargeRect = sf::RectangleShape(sf::Vector2f(rectHorExtraWidth + + hudHorSize * (25.0f / 200.0f), hudItemRectHeight));
	timeRemainingRects = sf::RectangleShape(sf::Vector2f(rectHorExtraWidth + hudHorSize * (15.0f / 200.0f), hudItemRectHeight));

	lvlNumText.setCharacterSize(smallText);
	timeRemainingText.setCharacterSize(smallText);
	healthLabelText.setCharacterSize(smallText);
	spclWpLblTxt.setCharacterSize(smallText);


	backGroundRect.setPosition(hudTopLeft);
	lvlNumPositionOffset = sf::Vector2f(0.0f, 0.0f);
	lvlNumRect.setPosition(hudItemPosition + lvlNumPositionOffset);
	lvlNumText.setPosition(hudItemPosition + lvlNumPositionOffset + textOffset);

	healthLabelOffset = sf::Vector2f(lvlNumPositionOffset.x + lvlNumRect.getSize().x + hudItemSpacing * (18.0f), 0.0f);
	healthLabelRect.setPosition(hudItemPosition + healthLabelOffset);
	healthLabelText.setPosition(hudItemPosition + healthLabelOffset + textOffset);

	healthRemainingPosOffset = sf::Vector2f(healthLabelOffset.x + healthLabelRect.getSize().x + hudItemSpacing, 0.0f);
	healthSegmentsRect.setPosition(hudItemPosition + healthRemainingPosOffset);

	spclWpLblRctOffset = sf::Vector2f(healthRemainingPosOffset.x + healthSegmentsRect.getSize().x + hudItemSpacing * (18.0f), 0.0f);
	spclWpLblRct.setPosition(hudItemPosition + spclWpLblRctOffset);
	spclWpLblTxt.setPosition(hudItemPosition + spclWpLblRctOffset + textOffset);

	specialWeapPosOffset = sf::Vector2f(spclWpLblRctOffset.x + spclWpLblRct.getSize().x + hudItemSpacing, 0.0f);
	specialWeaponRect.setPosition(hudItemPosition + specialWeapPosOffset);
	specialWeaponCurrChargeRect.setPosition(hudItemPosition + specialWeapPosOffset + textOffset);

	timeRemPosOffset = sf::Vector2f(backGroundRect.getPosition().x + backGroundRect.getSize().x - timeRemainingRects.getSize().x - hudItemSpacing, hudItemPosition.y);
	timeRemainingRects.setPosition(timeRemPosOffset);
	timeRemainingText.setPosition(timeRemPosOffset + textOffset);

	backGroundRect.setFillColor(sf::Color::Black);
	lvlNumRect.setFillColor(sf::Color::Black); 
	healthLabelRect.setFillColor(sf::Color::Black);
	healthSegmentsRect.setFillColor(sf::Color::Black);
	spclWpLblRct.setFillColor(sf::Color::Black);
	specialWeaponRect.setFillColor(sf::Color::Black);
	specialWeaponCurrChargeRect.setFillColor(sf::Color::White);
	timeRemainingRects.setFillColor(sf::Color::Black);

	backGroundRect.setOutlineThickness(hudOutLineThickness);
	lvlNumRect.setOutlineThickness(hudItemOutLineThickness);
	healthLabelRect.setOutlineThickness(hudItemOutLineThickness);
	healthSegmentsRect.setOutlineThickness(hudItemOutLineThickness);
	spclWpLblRct.setOutlineThickness(hudItemOutLineThickness);
	specialWeaponRect.setOutlineThickness(hudItemOutLineThickness);
	timeRemainingRects.setOutlineThickness(hudItemOutLineThickness);

	lvlNumText.setFillColor(sf::Color::White);
	healthLabelText.setFillColor(sf::Color::White);
	spclWpLblTxt.setFillColor(sf::Color::White);
	timeRemainingText.setFillColor(sf::Color::White);

	lvlNumText.setString("LEVEL: INIT");
	timeRemainingText.setString("TIME: INIT");
	spclWpLblTxt.setString("STUN");
	healthLabelText.setString("HULL");
}

HeadsUpDisplay::~HeadsUpDisplay()
{
}

void HeadsUpDisplay::Update(int i_lvlNum, int i_timeRemaining, int i_maxTime, PlayerChar* i_player) {

	UpdateAOECharge(i_player);
	UpdateHealth(i_player);
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
}

void HeadsUpDisplay::UpdateAOECharge(PlayerChar* i_player)
{
	float rectHorOffset = -10.0f;
	//float rectHorExtraWidth = 10.0f; 
	float weaponDelay = (std::chrono::duration_cast<std::chrono::microseconds>(hiResTime::now() - i_player->lastAOEFired)).count() / 1000000.0f;
	float percentCharged = std::min(weaponDelay / i_player->shipRateOfAOE, 1.0f);
	specialWeaponCurrChargeRect = sf::RectangleShape((sf::Vector2f((specialWeaponRect.getSize().x - rectHorExtraWidth) * percentCharged, hudFillBarHeight)));
	specialWeaponCurrChargeRect.setPosition(sf::Vector2f(specialWeaponRect.getPosition().x + rectHorExtraWidth * (1.0f / 2.0f), specialWeaponRect.getPosition().y + hudItemRectHeight * (1.0 / 10.0f)));
	//cyan rgb(0,255,255)
	int gbVals = std::min(weaponDelay / i_player->shipRateOfAOE, 1.0f) * 255;
	specialWeaponCurrChargeRect.setFillColor(sf::Color(0, gbVals, gbVals));
}

void HeadsUpDisplay::UpdateHealth(PlayerChar* i_player)
{ //TODO 
	//if Im reading the heap size allocations right I think recreating this is using a lot of space
	int currHealth = i_player->health;
	int maxHealth = i_player->maxHealth;
	if (currHealth != prevHealth) {
		prevHealth = currHealth;
		healthSegments.clear();
		for (int i = 0; i < currHealth; i++) {
			sf::RectangleShape healthSegment = sf::RectangleShape(sf::Vector2f(((1.0f/ (float)maxHealth) * healthRectWidth) - segmentMarg, hudFillBarHeight));
			healthSegment.setPosition(healthSegmentsRect.getPosition().x + ((1.0f/ (float)maxHealth) * healthRectWidth * i) + (segmentMarg * 2.0f), hudItemPosition.y + specialWeapPosOffset.y + hudItemRectHeight * (1.0 / 10.0f));
			healthSegment.setOutlineThickness(segmentMarg);
			float amountRemaining = (float)currHealth / (float)maxHealth;
			if (amountRemaining > 0.667) {
				healthSegment.setFillColor(sf::Color::Green);
			}
			else if (amountRemaining > 0.334) {
				healthSegment.setFillColor(sf::Color::Yellow);
			}
			else {
				healthSegment.setFillColor(sf::Color::Red);
			}
			healthSegment.setOutlineColor(sf::Color::Black);
			healthSegments.push_back(healthSegment);
		}

	}
}

std::vector<sf::Drawable*> HeadsUpDisplay::GetDrawables()
{
	std::vector<sf::Drawable*> drawables;

	drawables.push_back(&backGroundRect);

	drawables.push_back(&lvlNumRect);
	drawables.push_back(&timeRemainingRects);
	drawables.push_back(&healthLabelRect);
	drawables.push_back(&healthLabelText);
	drawables.push_back(&healthSegmentsRect);
	drawables.push_back(&spclWpLblRct);
	drawables.push_back(&specialWeaponRect);
	drawables.push_back(&specialWeaponCurrChargeRect);

	drawables.push_back(&lvlNumText);
	drawables.push_back(&spclWpLblTxt);
	drawables.push_back(&timeRemainingText);

	for (int i = 0; i < healthSegments.size(); i++) {
		drawables.push_back(&healthSegments[i]);
	}

	return drawables;
}