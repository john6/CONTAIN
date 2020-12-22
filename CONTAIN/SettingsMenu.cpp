#include "SettingsMenu.h"

SettingsMenu::SettingsMenu(RESOURCES * i_resources, RSLTN* resolution, bool* fullScreen, SaveData* i_saveDataPtr) :
	resources{ i_resources }, resolutionPtr { resolution }, fullScreenPtr { fullScreen }
{
	saveDatePtr = i_saveDataPtr;
	font = resources->GetFont();
	ResetButtons();
	lastButtonPressed = std::chrono::high_resolution_clock::now();
	buttonClickDelay = 0.9f;
	timeSinceButtonClick = buttonClickDelay;
	somethingWasClicked = false;

	LoadConfig();
	//Testing Settings
	//if (TESTING) {
	//	UpdateSoundQuad(LOW25);
	//	music.SetState(Button::UP);
	//	resources->TurnMusicOn(false);
	//	resources->SetSoundLevel(0.25f);
	//}
}




bool SettingsMenu::PollInput(sf::Vector2f mousePosition, Button * button, bool stickyButton)
{
	float halfWidth = (button->GetRect().getSize().x / 2);
	float halfHeight = (button->GetRect().getSize().y / 2);
	float distX = abs(mousePosition.x - (button->GetRect().getPosition().x + halfWidth));
	float distY = abs(mousePosition.y - (button->GetRect().getPosition().y + halfHeight));
	if ((distX <= halfWidth) && (distY <= halfHeight)) {
		if (!stickyButton) { button->SetState(Button::HOVER); }
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			button->SetState(Button::DOWN);
			somethingWasClicked = true;
			return true;
		}
	}
	else if (!stickyButton) {
		button->SetState(Button::UP);
	}
	return false;
}

bool SettingsMenu::PollInputToggle(sf::Vector2f mousePosition, Button * button)
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

void SettingsMenu::PollSoundQuad(sf::Vector2f mousePosition)
{
	if (PollInput(mousePosition, &sound25P, true)) {
		somethingWasClicked = true;
		saveDatePtr->EditSettingsConfig(0, 0);
		UpdateSoundQuad(LOW25); }
	else if (PollInput(mousePosition, &sound50P, true)) { 
		somethingWasClicked = true;
		saveDatePtr->EditSettingsConfig(0, 1);
		UpdateSoundQuad(MEDIUM50); }
	else if (PollInput(mousePosition, &sound75P, true)) { 
		somethingWasClicked = true;
		saveDatePtr->EditSettingsConfig(0, 2);
		UpdateSoundQuad(HIGH75); }
	else if (PollInput(mousePosition, &sound100P, true)) { 
		somethingWasClicked = true;
		saveDatePtr->EditSettingsConfig(0, 3);
		UpdateSoundQuad(FULL100); }
}

void SettingsMenu::PollResoQuad(sf::Vector2f mousePosition)
{
	if (PollInput(mousePosition, &screen1272, true)) { 
		somethingWasClicked = true;
		saveDatePtr->EditSettingsConfig(2, 0);
		UpdateResoQuad(TWLV_SVN); }
	else if (PollInput(mousePosition, &screen1490, true)) { 
		somethingWasClicked = true;
		saveDatePtr->EditSettingsConfig(2, 1);
		UpdateResoQuad(FRTN_NNTY); }
	else if (PollInput(mousePosition, &screen1910, true)) { 
		saveDatePtr->EditSettingsConfig(2, 2);
		somethingWasClicked = true;
		UpdateResoQuad(NNTN_TN); }
}

void SettingsMenu::UpdateSoundQuad(SND_LVL i_soundSelected)
{
	switch (i_soundSelected) {
	case LOW25: {
		soundLvl = 25.0f;
		sound25P.SetState(Button::DOWN);
		sound50P.SetState(Button::UP);
		sound75P.SetState(Button::UP);
		sound100P.SetState(Button::UP);
		break;
	}
	case MEDIUM50: {
		soundLvl = 50.0f;
		sound25P.SetState(Button::UP);
		sound50P.SetState(Button::DOWN);
		sound75P.SetState(Button::UP);
		sound100P.SetState(Button::UP);
		break;
	}
	case HIGH75: {
		soundLvl = 75.0f;
		sound25P.SetState(Button::UP);
		sound50P.SetState(Button::UP);
		sound75P.SetState(Button::DOWN);
		sound100P.SetState(Button::UP);
		break;
	}
	case FULL100: {
		soundLvl = 100.0f;
		sound25P.SetState(Button::UP);
		sound50P.SetState(Button::UP);
		sound75P.SetState(Button::UP);
		sound100P.SetState(Button::DOWN);
		break;
	}
	}
}

void SettingsMenu::UpdateResoQuad(RSLTN i_resSelected)
{
	switch (i_resSelected) {
	case TWLV_SVN: {
		*resolutionPtr = i_resSelected;
		screen1272.SetState(Button::DOWN);
		screen1490.SetState(Button::UP);
		screen1910.SetState(Button::UP);
		break;
	}
	case FRTN_NNTY: {
		*resolutionPtr = i_resSelected;
		screen1272.SetState(Button::UP);
		screen1490.SetState(Button::DOWN);
		screen1910.SetState(Button::UP);
		break;
	}
	case NNTN_TN: {
		*resolutionPtr = i_resSelected;
		screen1272.SetState(Button::UP);
		screen1490.SetState(Button::UP);
		screen1910.SetState(Button::DOWN);
		break;
	}
	}
}

SettingsMenu::~SettingsMenu()
{
}

void SettingsMenu::ResetMenu()
{
	//sound25P.SetState(Button::UP);
	//sound50P.SetState(Button::UP);
	//sound75P.SetState(Button::UP);
	//sound100P.SetState(Button::UP);
	//music.SetState(Button::UP);
	//screen1272.SetState(Button::UP);
	//screen1490.SetState(Button::UP);
	//screen1910.SetState(Button::UP);
	//FullScreen.SetState(Button::UP);
	//apply.SetState(Button::UP);
	//exit.SetState(Button::UP);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void SettingsMenu::LoadConfig()
{
	std::vector<int> config = saveDatePtr->GetSettingsVect();
	//Configload
	//config 0 is sound setting
	switch (config[0]) {
	case 0: {
		UpdateSoundQuad(LOW25);
		break;
	}
	case 1: {
		UpdateSoundQuad(MEDIUM50);
		break;
	}
	case 2: {
		UpdateSoundQuad(HIGH75);
		break;
	}
	case 3: {
		UpdateSoundQuad(FULL100);
		break;
	}
	}
	//Music on bool is index 1
	if (config[1] == 1) {
		music.SetState(Button::DOWN);
	}
	else {
		music.SetState(Button::UP);
	}
	//reso index 2
	switch (config[2]) {
	case 0: {
		UpdateResoQuad(TWLV_SVN);
		break;
	}
	case 1: {
		UpdateResoQuad(FRTN_NNTY);
		break;
	}
	case 2: {
		UpdateResoQuad(NNTN_TN);
		break;
	}
	}
	//fullscreen bool is index 3
	if (config[3] == 1) {
		FullScreen.SetState(Button::DOWN);
	}
	else {
		FullScreen.SetState(Button::UP);
	}

}

void SettingsMenu::ResetButtons()
{
	sf::Vector2f topLeft(GLBVRS::HR_MRG * 1.5, GLBVRS::VRT_MRG * 1.5f);
	float bttnMrgHr = (GLBVRS::HR_MRG / 4.0f);
	float bttnMrgVrt = (GLBVRS::VRT_MRG / 2.0f);
	float bttnOffsetHor = GLBVRS::BTTN_WDTH + bttnMrgHr;
	float bttnOffsetVert = GLBVRS::BTTN_HGHT + bttnMrgVrt;

	//Volume Options
	sf::RectangleShape playButtonRect(sf::Vector2f(GLBVRS::BTTN_WDTH, GLBVRS::BTTN_HGHT));
	playButtonRect.setPosition(topLeft + sf::Vector2f(0.0f, bttnOffsetVert * 1.0f));
	sound25P = Button("25%", playButtonRect, &font);
	sound25P.SetColors(sf::Color::Black, sf::Color::White, sf::Color(128, 128, 128));

	sf::RectangleShape playButtonRect1(sf::Vector2f(GLBVRS::BTTN_WDTH, GLBVRS::BTTN_HGHT));
	playButtonRect1.setPosition(topLeft + sf::Vector2f(bttnOffsetHor, bttnOffsetVert * 1.0f));
	sound50P = Button("50%", playButtonRect1, &font);
	sound50P.SetColors(sf::Color::Black, sf::Color::White, sf::Color(128, 128, 128));

	sf::RectangleShape playButtonRect2(sf::Vector2f(GLBVRS::BTTN_WDTH, GLBVRS::BTTN_HGHT));
	playButtonRect2.setPosition(topLeft + sf::Vector2f(bttnOffsetHor * 2.0f, bttnOffsetVert * 1.0f));
	sound75P = Button("75%", playButtonRect2, &font);
	sound75P.SetColors(sf::Color::Black, sf::Color::White, sf::Color(128, 128, 128));

	sf::RectangleShape playButtonRect3(sf::Vector2f(GLBVRS::BTTN_WDTH, GLBVRS::BTTN_HGHT));
	playButtonRect3.setPosition(topLeft + sf::Vector2f(bttnOffsetHor * 3.0f, bttnOffsetVert * 1.0f));
	sound100P = Button("100%", playButtonRect3, &font);
	sound100P.SetColors(sf::Color::Black, sf::Color::White, sf::Color(128, 128, 128));

	//Music
	sf::RectangleShape playButtonRect4(sf::Vector2f(GLBVRS::BTTN_WDTH, GLBVRS::BTTN_HGHT));
	playButtonRect4.setPosition(topLeft + sf::Vector2f(0, bttnOffsetVert * 2.0f));
	music = Button("Music On", playButtonRect4, &font);
	music.SetColors(sf::Color::Black, sf::Color::White, sf::Color(128, 128, 128));

	//Resolution
	sf::RectangleShape playButtonRect5(sf::Vector2f(GLBVRS::BTTN_WDTH, GLBVRS::BTTN_HGHT));
	playButtonRect5.setPosition(topLeft + sf::Vector2f(0, bttnOffsetVert * 4.0f));
	screen1272 = Button("1280x720", playButtonRect5, &font);
	screen1272.SetColors(sf::Color::Black, sf::Color::White, sf::Color(128, 128, 128));

	sf::RectangleShape playButtonRect6(sf::Vector2f(GLBVRS::BTTN_WDTH, GLBVRS::BTTN_HGHT));
	playButtonRect6.setPosition(topLeft + sf::Vector2f(bttnOffsetHor * 1.0f, bttnOffsetVert * 4.0f));
	screen1490 = Button("1440x900", playButtonRect6, &font);
	screen1490.SetColors(sf::Color::Black, sf::Color::White, sf::Color(128, 128, 128));

	sf::RectangleShape playButtonRect7(sf::Vector2f(GLBVRS::BTTN_WDTH, GLBVRS::BTTN_HGHT));
	playButtonRect7.setPosition(topLeft + sf::Vector2f(bttnOffsetHor * 2.0f, bttnOffsetVert * 4.0f));
	screen1910 = Button("1920x1080", playButtonRect7, &font);
	screen1910.SetColors(sf::Color::Black, sf::Color::White, sf::Color(128, 128, 128));

	//FullScreen
	sf::RectangleShape playButtonRect8(sf::Vector2f(GLBVRS::BTTN_WDTH, GLBVRS::BTTN_HGHT));
	playButtonRect8.setPosition(topLeft + sf::Vector2f(0, bttnOffsetVert * 5.0f));
	FullScreen = Button("FullScreen", playButtonRect8, &font);
	FullScreen.SetColors(sf::Color::Black, sf::Color::White, sf::Color(128, 128, 128));

	sf::RectangleShape playButtonRect9(sf::Vector2f(GLBVRS::BTTN_WDTH, GLBVRS::BTTN_HGHT));
	playButtonRect9.setPosition(topLeft + sf::Vector2f(0, bttnOffsetVert * 7.0f));
	apply = Button("Apply Changes", playButtonRect9, &font);
	apply.SetColors(sf::Color::Black, sf::Color::White, sf::Color(128, 128, 128));

	sf::RectangleShape playButtonRect10(sf::Vector2f(GLBVRS::BTTN_WDTH, GLBVRS::BTTN_HGHT));
	playButtonRect10.setPosition(topLeft + sf::Vector2f(bttnOffsetHor * 2.0f, bttnOffsetVert * 7.0f));
	exit = Button("Main Menu", playButtonRect10, &font);
	exit.SetColors(sf::Color::Black, sf::Color::White, sf::Color(128, 128, 128));

	title.setString("Settings");
	title.setCharacterSize(50);
	title.setFillColor(sf::Color::White);
	title.setFont(font);
	title.setPosition(sf::Vector2f(bttnOffsetHor * 1.5f, GLBVRS::VRT_MRG * 0.5f));

	soundLevelText.setString("Sound Level: ");
	soundLevelText.setCharacterSize(25);
	soundLevelText.setFillColor(sf::Color::White);
	soundLevelText.setFont(font);
	soundLevelText.setPosition(topLeft + sf::Vector2f(0, bttnOffsetVert * 0.5));

	screenResolutionText.setString("Screen Resolution: ");
	screenResolutionText.setCharacterSize(25);
	screenResolutionText.setFillColor(sf::Color::White);
	screenResolutionText.setFont(font);
	screenResolutionText.setPosition(topLeft + sf::Vector2f(0, bttnOffsetVert * 3.5f));
}

GAME_STATE SettingsMenu::Update(float i_microSecs, sf::RenderWindow * i_window, sf::Vector2f i_mousePos)
{
	timeSinceButtonClick = (std::chrono::duration_cast<std::chrono::microseconds>(hiResTime::now() - lastButtonPressed)).count() / 1000000.0f;
	if (timeSinceButtonClick >= buttonClickDelay) {
		PollSoundQuad(i_mousePos);
		PollResoQuad(i_mousePos);
		bool fullScreenBool = PollInputToggle(i_mousePos, &FullScreen);
		*fullScreenPtr = fullScreenBool;
		if (fullScreenBool) {
			saveDatePtr->EditSettingsConfig(3, 1);
		}
		else {
			saveDatePtr->EditSettingsConfig(3, 0);
		}

		bool musicOnBool = PollInputToggle(i_mousePos, &music);
		resources->TurnMusicOn(musicOnBool);
		if (musicOnBool) {
			saveDatePtr->EditSettingsConfig(1, 1);
		}
		else {
			saveDatePtr->EditSettingsConfig(1, 0);
		}

		resources->SetSoundLevel(soundLvl);
		if (somethingWasClicked) {
			lastButtonPressed = hiResTime::now();
			somethingWasClicked = false;
		}
		if (PollInput(i_mousePos, &apply)) {
			resources->PlaySound(RESOURCES::OCTAVE_BEEP);
			lastButtonPressed = hiResTime::now();
			somethingWasClicked = false;
			return APPLY;
		}
		else if (PollInput(i_mousePos, &exit)) {
			lastButtonPressed = hiResTime::now();
			somethingWasClicked = false;
			return MENU;
		}
	}
	return SETTINGS;
}

void SettingsMenu::Render(sf::RenderWindow * window)
{
	window->clear();
	window->draw(sound25P.GetRect());
	window->draw(sound50P.GetRect());
	window->draw(sound75P.GetRect());
	window->draw(sound100P.GetRect());
	window->draw(music.GetRect());
	window->draw(screen1272.GetRect());
	window->draw(screen1490.GetRect());
	window->draw(screen1910.GetRect());
	window->draw(FullScreen.GetRect());
	window->draw(apply.GetRect());
	window->draw(exit.GetRect());

	sf::Text sound25PText = sound25P.GetText();
	sf::Text sound50PText = sound50P.GetText();
	sf::Text sound75PText = sound75P.GetText();
	sf::Text sound100PText = sound100P.GetText();
	sf::Text musicText = music.GetText();
	sf::Text screen1272Text = screen1272.GetText();
	sf::Text screen1490Text = screen1490.GetText();
	sf::Text screen1910Text = screen1910.GetText();
	sf::Text FullScreenText = FullScreen.GetText();
	sf::Text applyText = apply.GetText();
	sf::Text exitText = exit.GetText();

	//sound25PText.setFont(font);
	//sound50PText.setFont(font);
	//sound75PText.setFont(font);
	//sound100PText.setFont(font);
	//musicText.setFont(font);
	//screen1272Text.setFont(font);
	//screen1490Text.setFont(font);
	//screen1910Text.setFont(font);
	//FullScreenText.setFont(font);
	//applyText.setFont(font);
	//exitText.setFont(font);

	window->draw(sound25PText);
	window->draw(sound50PText);
	window->draw(sound75PText);
	window->draw(sound100PText);
	window->draw(musicText);
	window->draw(screen1272Text);
	window->draw(screen1490Text);
	window->draw(screen1910Text);
	window->draw(FullScreenText);
	window->draw(applyText);
	window->draw(exitText);
	window->draw(title);
	window->draw(soundLevelText);
	window->draw(screenResolutionText);

	window->display();
}
