#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include "RESOURCES.h"
#include "GLOBAL_CONSTANTS.h"
#include "Button.h"

class SettingsMenu
{
	sf::Font font;
	sf::Text title;
	sf::Text soundLevelText;
	sf::Text screenResolutionText;

	RESOURCES* resources;
	RSLTN* resolutionPtr;
	bool* fullScreenPtr;
	float soundLvl;
	hiRes_time_point lastButtonPressed;
	float timeSinceButtonClick;
	float buttonClickDelay;
	bool somethingWasClicked;


	//Sound
	Button sound25P;
	Button sound50P;
	Button sound75P;
	Button sound100P;
	Button music;

	//Resolution
	Button screen1272;
	Button screen1490;
	Button screen1910;
	Button FullScreen;

	//Navigation
	Button apply;
	Button exit;

	bool PollInput(sf::Vector2f mousePosition, Button* button, bool stickyButton = false);
	bool PollInputToggle(sf::Vector2f mousePosition, Button* button);

	void  PollSoundQuad(sf::Vector2f mousePosition);
	void  PollResoQuad(sf::Vector2f mousePosition);
	void  UpdateSoundQuad(SND_LVL i_soundSelected);
	void  UpdateResoQuad(RSLTN i_resSelected);

public:
	SettingsMenu(RESOURCES* i_resources, RSLTN* resolution, bool* fullScreen);
	~SettingsMenu();


public:
	void ResetMenu();


	void ResetButtons();

	GAME_STATE Update(float microSeconds, sf::RenderWindow* window, sf::Vector2f mousePosition);

	void Render(sf::RenderWindow* window);
};