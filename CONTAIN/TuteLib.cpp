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
	 begin.AddTextBox("Welcome to the force, Captain Spaceship.\nThis abandoned freighter is crawling with aliens!\nYou'll have to clear it out before we can retake\nthe nearby space station.\n\n Start by flying into that blue teleportation door");
	 begin.PlaceBottomBox();
	 shoot = InfoPopUp(i_resources, GLBVRS::CRT_WDTH, GLBVRS::CRT_HGHT);
	 shoot.AddTextBox("My god... look at that disgusting creature.\nThe freighter's automated quarantine has activated, \nand the doors will remain locked until all enemies are killed.\n\nLeft click to shoot your primary laser");
	 shoot.PlaceBottomBox();
	 wall = InfoPopUp(i_resources, GLBVRS::CRT_WDTH, GLBVRS::CRT_HGHT);
	 wall.AddTextBox("Those aliens will hurt you if they collide with your ship. \n\nRight click to create an energy wall to stop them.\n\nYour primary lasers will bounce off of your energy walls, too!");
	 wall.PlaceBottomBox();
	 emp = InfoPopUp(i_resources, GLBVRS::CRT_WDTH, GLBVRS::CRT_HGHT);
	 emp.AddTextBox("Damn, there's a lot of them in here.\n\nIf you are swarmed by enemies, click the scroll wheel\n or space bar to release a stun blast.\nIt will repulse the aliens, and stun them for a few seconds.\n\n Your ship's border will glow blue when your stun is charged.");
	 emp.PlaceBottomBox();
	 firewall = InfoPopUp(i_resources, GLBVRS::CRT_WDTH, GLBVRS::CRT_HGHT);
	 firewall.AddTextBox("Be careful, Captain, This sector has some red hot walls!\n\nLooks like this freighter's still got some power. Steer clear.\n\nIt will hurt the aliens as well.");
	 firewall.PlaceBottomBox();
	 boss = InfoPopUp(i_resources, GLBVRS::CRT_WDTH, GLBVRS::CRT_HGHT);
	 boss.AddTextBox("Oh baby, this alien looks like trouble. It sequestered itself\nin the engine room. You're in for a tough fight!\n");
	 boss.PlaceBottomBox();
	 escape = InfoPopUp(i_resources, GLBVRS::CRT_WDTH, GLBVRS::CRT_HGHT);
	 escape.AddTextBox("Ok lol never mind. Turns out that was just a normal guy.\nThat would have been crazy if he was really strong though.\n\nOnce you defeat a level's boss, the engine will start\n heating up until the freighter explodes,\n hurry back to the first room!");
	 escape.PlaceBottomBox();
	 pushenemies = InfoPopUp(i_resources, GLBVRS::CRT_WDTH, GLBVRS::CRT_HGHT);
	 pushenemies.AddTextBox("The aliens are suffering from the heat!\nThey won't hurt you in this state,but they're\nblocking you from the exit.\n\nThe doors are all stuck open, keep backtracking until you arrive\n back at the entrance,\n which now has a spinning blue exit teleporter!");
	 pushenemies.PlaceBottomBox();
	 end = InfoPopUp(i_resources, GLBVRS::CRT_WDTH, GLBVRS::CRT_HGHT);
	 end.AddTextBox("Oh wow, you got out just in time!\n\nYou probably can't see it but there's a HUGE explosion. Explode\n more freighters and we'll upgrade your ship as compensation!\n Fare well, Captain Spaceship.\n");
	 end.PlaceBottomBox();
	 pauseMenu = InfoPopUp(i_resources, GLBVRS::CRT_WDTH, GLBVRS::CRT_HGHT);
	 pauseMenu.AddTextBox("You've just paused the game, how neat is that?\n\nPress escape again to unpause,\nor press backspace to exit the game.\n\n   Have fun!    :)");
	 //pauseMenu.PlaceBottomBox();
}


TuteLib::~TuteLib()
{
}
