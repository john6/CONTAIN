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
	  InfoPopUp end;

public:
	enum TUTORIALS { TUTORIAL_BEGIN, SHOOTING, WALL, EMP, FIREWALL, BOSS, ESCAPE, PUSH_ENEMIES, TUTORIAL_END };

	void PlayTutorial(TUTORIALS i_tute);

	TUTORIALS GetTuteFromSect(int x, int y);

	void InitStatics(sf::RenderWindow* i_windowPtr, RESOURCES* i_resources);

	TuteLib(sf::RenderWindow* i_windowPtr, RESOURCES* i_resources);
	//TuteLib();
	~TuteLib();
};