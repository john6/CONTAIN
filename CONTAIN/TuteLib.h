#pragma once
#include "InfoPopUp.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class TuteLib
{
private:
	  sf::RenderWindow* windowPtr;
	  RESOURCES* resources;
	  InfoPopUp begin;
	  InfoPopUp shoot;
	  InfoPopUp wall;
	  InfoPopUp emp;
	  InfoPopUp firewall;
	  InfoPopUp boss;
	  InfoPopUp escape;
	  InfoPopUp pushenemies;
	  InfoPopUp exit;
	  InfoPopUp end;
	  InfoPopUp pauseMenu;

public:
	enum TUTORIALS { TUTORIAL_BEGIN, SHOOTING, WALL, EMP, FIREWALL, BOSS, ESCAPE, PUSH_ENEMIES, TUTORIAL_EXIT, TUTORIAL_END, PAUSE_MENU };
	TUTORIALS currActiveTutorial;

	void PlayTutorial(TUTORIALS i_tute);

	void SetTutorial(TUTORIALS i_tute);

	void RenderCurrTutorial();

	InfoPopUp* GetCurrPopUp();

	bool IsTutorialPlayed(TUTORIALS i_tute);

	TUTORIALS GetTuteFromSect(int x, int y);

	void InitStatics(sf::RenderWindow* i_windowPtr, RESOURCES* i_resources);

	void ResetAllTutes();

	TuteLib(sf::RenderWindow* i_windowPtr, RESOURCES* i_resources);

	~TuteLib();
};