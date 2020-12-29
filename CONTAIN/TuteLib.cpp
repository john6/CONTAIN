#include "TuteLib.h"



void TuteLib::PlayTutorial(TUTORIALS i_tute)
{
	switch (i_tute) {
	case TUTORIAL_BEGIN: {
		 begin.RenderPopUp(windowPtr);
		break;
	}
	case SHOOTING: {
		 shoot.RenderPopUp(windowPtr);
		break;
	}
	case WALL: {
		 wall.RenderPopUp(windowPtr);
		break;
	}
	case EMP: {
		 emp.RenderPopUp(windowPtr);
		break;
	}
	case FIREWALL: {
		 firewall.RenderPopUp(windowPtr);
		break;
	}
	case BOSS: {
		 boss.RenderPopUp(windowPtr);
		break;
	}
	case ESCAPE: {
		 escape.RenderPopUp(windowPtr);
		break;
	}
	case PUSH_ENEMIES: {
		pushenemies.RenderPopUp(windowPtr);
		break;
	}
	case TUTORIAL_EXIT: {
		exit.RenderPopUp(windowPtr);
		break;
	}
	case TUTORIAL_END: {
		end.RenderPopUp(windowPtr);
		break;
	}
	case PAUSE_MENU: {
		return pauseMenu.RenderPopUp(windowPtr);
		break;
	}
	}
}

void TuteLib::SetTutorial(TUTORIALS i_tute)
{
	currActiveTutorial = i_tute;
}

void TuteLib::RenderCurrTutorial()
{
	PlayTutorial(currActiveTutorial);
}

InfoPopUp * TuteLib::GetCurrPopUp()
{
	switch (currActiveTutorial) {
	case TUTORIAL_BEGIN: {
		return &begin;
		break;
	}
	case SHOOTING: {
		return &shoot;
		break;
	}
	case WALL: {
		return &wall;
		break;
	}
	case EMP: {
		return &emp;
		break;
	}
	case FIREWALL: {
		return &firewall;
		break;
	}
	case BOSS: {
		return &boss;
		break;
	}
	case ESCAPE: {
		return &escape;
		break;
	}
	case PUSH_ENEMIES: {
		return &pushenemies;
		break;
	}
	case TUTORIAL_EXIT: {
		return &exit;
		break;
	}
	case TUTORIAL_END: {
		return &end;
		break;
	}
	case PAUSE_MENU: {
		return &pauseMenu;
	}
	}
	return nullptr;
}

bool TuteLib::IsTutorialPlayed(TUTORIALS i_tute)
{
	switch (i_tute) {
	case TUTORIAL_BEGIN: {
		return begin.played;
		break;
	}
	case SHOOTING: {
		return shoot.played;
		break;
	}
	case WALL: {
		return wall.played;
		break;
	}
	case EMP: {
		return emp.played;
		break;
	}
	case FIREWALL: {
		return firewall.played;
		break;
	}
	case BOSS: {
		return boss.played;
		break;
	}
	case ESCAPE: {
		return escape.played;
		break;
	}
	case PUSH_ENEMIES: {
		return pushenemies.played;
		break;
	}
	case TUTORIAL_EXIT: {
		return exit.played;
		break;
	}
	case TUTORIAL_END: {
		return end.played;
		break;
	}
	case PAUSE_MENU: {
		return pauseMenu.played;
	}
	}
	return false;
}

TuteLib::TUTORIALS TuteLib::GetTuteFromSect(int x, int y)
{
	TUTORIALS tute;
	if ((x == 0) && (y == 0)) {
		return TUTORIAL_BEGIN;
	}
	else if ((x == 1) && (y == 0)) {
		return SHOOTING;
	}
	else if ((x == 2) && (y == 0)) {
		return WALL;
	}
	else if ((x == 3) && (y == 0)) {
		return EMP;
	}
	else if ((x == 4) && (y == 0)) {
		return FIREWALL;
	}
	else if ((x == 5) && (y == 0)) {
		return BOSS;
	}
}

TuteLib::TuteLib(sf::RenderWindow* i_windowPtr, RESOURCES* i_resources) :
	windowPtr { i_windowPtr }, resources {i_resources}
{
	InitStatics(i_windowPtr, i_resources);
}

void TuteLib::InitStatics(sf::RenderWindow* i_windowPtr, RESOURCES* i_resources)
{
	 windowPtr = i_windowPtr;
	 resources = i_resources;
	 begin = InfoPopUp(i_resources, GLBVRS::CRT_WDTH, GLBVRS::CRT_HGHT);
	 begin.AddTextBox("Welcome to the force, Captain Spaceship.\nThis abandoned freighter is crawling with aliens!\nYou'll have to clear it out before we can retake\nthe nearby space station.\n\nStart by using WASD to fly\ninto that little blue teleportation door");
	 begin.PlaceBottomBox();
	 shoot = InfoPopUp(i_resources, GLBVRS::CRT_WDTH, GLBVRS::CRT_HGHT);
	 shoot.AddTextBox("My god... look at that disgusting creature.\n\nThe freighter's automated quarantine has activated \nand the portals will remain locked until all enemies are killed.\n\nLeft click to shoot your primary laser");
	 shoot.PlaceBottomBox();
	 wall = InfoPopUp(i_resources, GLBVRS::CRT_WDTH, GLBVRS::CRT_HGHT);
	 wall.AddTextBox("Aliens will hurt you if they collide with your ship. \n\nRight click to deploy an energy wall to stop them.");
	 wall.PlaceBottomBox();
	 emp = InfoPopUp(i_resources, GLBVRS::CRT_WDTH, GLBVRS::CRT_HGHT);
	 emp.AddTextBox("Damn, there's a lot of them in here.\n\nClick the space bar to release a stun blast.\nIt will repulse and stun the aliens.\n\n Your ship's center circle will glow blue when it's charged.");
	 emp.PlaceBottomBox();
	 firewall = InfoPopUp(i_resources, GLBVRS::CRT_WDTH, GLBVRS::CRT_HGHT);
	 firewall.AddTextBox("Be careful, Captain, This sector has some red hot walls!\n\nLooks like this freighter's got some malfunctioning power coils.\nSteer clear.\n\nIt will hurt the aliens too.");
	 firewall.PlaceBottomBox();
	 boss = InfoPopUp(i_resources, GLBVRS::CRT_WDTH, GLBVRS::CRT_HGHT);
	 boss.AddTextBox("Oh baby, this alien looks like trouble.\n\nIt sequestered itself in the engine room.\n\nYou're in for a tough fight!\n");
	 boss.PlaceBottomBox();
	 escape = InfoPopUp(i_resources, GLBVRS::CRT_WDTH, GLBVRS::CRT_HGHT);
	 escape.AddTextBox("Ok lol never mind. Turns out that was just a normal guy.\nThat would have been crazy if he was really strong though.\n\nOnce you defeat a boss, the freighter will overheat,\nRace back to the first room before the it explodes!");
	 escape.PlaceBottomBox();
	 pushenemies = InfoPopUp(i_resources, GLBVRS::CRT_WDTH, GLBVRS::CRT_HGHT);
	 pushenemies.AddTextBox("The aliens are suffering from the heat!\nThey won't hurt you in this state, just slow you down.\n\nThe doors are all stuck open, keep backtracking until you\n get back to the entrance!");
	 pushenemies.PlaceBottomBox();
	 exit = InfoPopUp(i_resources, GLBVRS::CRT_WDTH, GLBVRS::CRT_HGHT);
	 exit.AddTextBox("See that spinning blue thing? That's the exit portal!\n\nFly right into it before the ship explodes!\n");
	 exit.PlaceBottomBox();
	 end = InfoPopUp(i_resources, GLBVRS::CRT_WDTH, GLBVRS::CRT_HGHT);
	 end.AddTextBox("Oh wow, you got out just in time!\n\nYou probably can't see it but there's a HUGE explosion.\n Destory more infested freighters\n and we'll upgrade your ship as a reward!\n\n  Fare well, Captain Spaceship.");
	 end.PlaceBottomBox();
	 pauseMenu = InfoPopUp(i_resources, GLBVRS::CRT_WDTH, GLBVRS::CRT_HGHT);
	 pauseMenu.AddTextBox("You've just paused the game, how neat is that?\n\nPress escape again to unpause,\nor press backspace to exit the game.\n\n   Have fun!    :)");
	 //pauseMenu.PlaceBottomBox();
}

void TuteLib::ResetAllTutes()
{
	begin.played = false;
	shoot.played = false;
	wall.played = false;
	emp.played = false;
	firewall.played = false;
	escape.played = false;
	pushenemies.played = false;
	exit.played = false;
	end.played = false;
}


TuteLib::~TuteLib()
{
}
