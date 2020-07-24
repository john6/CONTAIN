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
	GLBVRS::CRT_WDTH = static_cast<float>(GLBVRS::SCREEN_WIDTH * 18.0f / 20.0f);
	GLBVRS::CRT_HGHT = static_cast<float>(GLBVRS::SCREEN_HEIGHT * 17.0f / 20.0f);
	GLBVRS::HR_MRG = static_cast<float>(GLBVRS::SCREEN_WIDTH / 20.0f);     //horizontal margin
	GLBVRS::VRT_MRG = static_cast<float>(GLBVRS::SCREEN_HEIGHT / 20.0f);  //vertical margin
	GLBVRS::SIZE_RAT = static_cast<float>
		(std::min( ((float)GLBVRS::SCREEN_WIDTH / 1920.0f ), ((float)GLBVRS::SCREEN_HEIGHT / 1080.0f)));


	//MENU RESOLUTION
	GLBVRS::BTTN_WDTH = GLBVRS::SCREEN_WIDTH / 3;
	GLBVRS::BTTN_HGHT = GLBVRS::SCREEN_HEIGHT / 10;

	//OBJECT SIZES
	GLBVRS::DOOR_WIDTH = 150.0f * GLBVRS::SIZE_RAT;
	GLBVRS::DOOR_HEIGHT = 50.0f* GLBVRS::SIZE_RAT;
	GLBVRS::WALL_THICKNESS = 100.0f* GLBVRS::SIZE_RAT;
	GLBVRS::PROJECTILE_RADIUS = 10.0f * GLBVRS::SIZE_RAT;

	//SPEED ADJUSTMENTS
	GLBVRS::ENEMYSPEEDEASY = 12.0f * std::pow(GLBVRS::SIZE_RAT, 2);
	GLBVRS::ENEMYSPEEDMED = 15.0f * std::pow(GLBVRS::SIZE_RAT, 2);
	GLBVRS::ENEMYSPEEDHARD = 18.0f * std::pow(GLBVRS::SIZE_RAT, 2);
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
float GLBVRS::SIZE_RAT = 1;

//OBJECT SIZES
float GLBVRS::DOOR_WIDTH = 150.0f;
float GLBVRS::DOOR_HEIGHT = 50.0f;
float GLBVRS::WALL_THICKNESS = 100.0f;
float GLBVRS::PROJECTILE_RADIUS = 10.0f;

float GLBVRS::ENEMYSPEEDEASY = 12.0f;
float GLBVRS::ENEMYSPEEDMED = 15.0f;
float GLBVRS::ENEMYSPEEDHARD = 18.0f;

GLBVRS::~GLBVRS()
{
}
