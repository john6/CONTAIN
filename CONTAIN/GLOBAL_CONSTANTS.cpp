#include "GLOBAL_CONSTANTS.h"

//GLOBAL_CONSTANTS::GLOBAL_CONSTANTS(unsigned int i_screenWidth, unsigned int i_screenHeight)
//{
//
//}

void GLBVRS::SetGlobalConstants(unsigned int i_screenWidth, unsigned int i_screenHeight)
{
	//WINDOW SETTINGS
	GLBVRS::SCREEN_WIDTH = i_screenWidth;
	GLBVRS::SCREEN_HEIGHT = i_screenHeight;

	//GAME RESOLUTION
	GLBVRS::CRT_WDTH = static_cast<float>(GLBVRS::SCREEN_WIDTH * 9.0f / 10.0f);
	GLBVRS::CRT_HGHT = static_cast<float>(GLBVRS::SCREEN_HEIGHT * 9.0f / 10.0f);
	GLBVRS::HR_MRG = static_cast<float>(GLBVRS::SCREEN_WIDTH / 20.0f);         //horizontal margin
	GLBVRS::VRT_MRG = static_cast<float>(GLBVRS::SCREEN_HEIGHT / 20.0f);       //vertical margin

	//MENU RESOLUTION
	GLBVRS::BTTN_WDTH = GLBVRS::SCREEN_WIDTH / 3;
	GLBVRS::BTTN_HGHT = GLBVRS::SCREEN_HEIGHT / 10;
}

int GLBVRS::GetUpgradeMax(UPGRADE_TYPE i_powType)
{
	switch (i_powType) {
	case (NONE): { //rate of fire
		return 0;
		break;
	}
	case (RATE_OF_FIRE): { //rate of fire
		return 3;
		break;
	}
	case (WEAP_SPEED): { //number of shots
		return 3;
		break;
	}
	case (SCATTER): { //number of shots
		return 3;
		break;
	}
	case (SMALL_SHIP): { //number of shots
		return 3;
		break;
	}
	case (BIG_SHIP): { //number of shots
		return 3;
		break;
	}
	case (BLAST): { //number of shots
		return 3;
		break;
	}
	case (WALL_BIG): { //number of shots
		return 3;
		break;
	}
	}

}

GLBVRS::GLBVRS()
{
}

//WINDOW SETTINGS
unsigned int GLBVRS::SCREEN_WIDTH = 0;
unsigned int GLBVRS::SCREEN_HEIGHT = 0;

//GAME RESOLUTION
float GLBVRS::CRT_WDTH = static_cast<float>(GLBVRS::SCREEN_WIDTH * 9.0f / 10.0f);
float GLBVRS::CRT_HGHT = static_cast<float>(GLBVRS::SCREEN_HEIGHT * 9.0f / 10.0f);
float GLBVRS::HR_MRG = static_cast<float>(GLBVRS::SCREEN_WIDTH / 20.0f);         //horizontal margin
float GLBVRS::VRT_MRG = static_cast<float>(GLBVRS::SCREEN_HEIGHT / 20.0f);       //vertical margin

//MENU RESOLUTION
float GLBVRS::BTTN_WDTH = GLBVRS::SCREEN_WIDTH / 3;
float GLBVRS::BTTN_HGHT = GLBVRS::SCREEN_HEIGHT / 10;


GLBVRS::~GLBVRS()
{
}
