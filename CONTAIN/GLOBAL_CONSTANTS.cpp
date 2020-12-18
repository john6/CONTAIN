#include "GLOBAL_CONSTANTS.h"

void GLBVRS::SetGlobalConstants(unsigned int i_screenWidth, unsigned int i_screenHeight,
	RESOURCES* i_resourcesPtr, MessageBus* i_mBusPtr, Game* i_gamePtr, std::shared_ptr<Entity> i_playerPtr, float i_soundLvl)
{
	//WINDOW SETTINGS
	//GLBVRS::SCREEN_WIDTH = 2400;   //i_screenWidth ;
	//GLBVRS::SCREEN_WIDTH = 2880;   //i_screenWidth ;
	GLBVRS::SCREEN_WIDTH = i_screenWidth;
	//GLBVRS::SCREEN_HEIGHT = 1350;    //i_screenHeight;
	GLBVRS::SCREEN_HEIGHT = i_screenHeight;

	//GAME RESOLUTION
	GLBVRS::CRT_WDTH = static_cast<float>(GLBVRS::SCREEN_WIDTH * 18.0f / 20.0f);
	GLBVRS::CRT_HGHT = static_cast<float>(GLBVRS::SCREEN_HEIGHT * 17.0f / 20.0f);
	GLBVRS::HR_MRG = static_cast<float>(GLBVRS::SCREEN_WIDTH / 20.0f);     //horizontal margin
	GLBVRS::VRT_MRG = static_cast<float>(GLBVRS::SCREEN_HEIGHT / 20.0f);  //vertical margin
	GLBVRS::SIZE_RAT = 1;  // This is basically depreciated, I am now using the SFML viewport for all scaling

	//MENU RESOLUTION
	GLBVRS::BTTN_WDTH = GLBVRS::CRT_WDTH / 5.0f;
	GLBVRS::BTTN_HGHT = GLBVRS::CRT_HGHT / 10;

	//OBJECT SIZES
	GLBVRS::DOOR_WIDTH = 150.0f;
	GLBVRS::DOOR_HEIGHT = 50.0f;
	GLBVRS::WALL_THICKNESS = 100.0f;
	GLBVRS::PROJECTILE_RADIUS = 10.0f;

	//SPEED ADJUSTMENTS
	GLBVRS::ENEMYSPEEDEASY = 17.0f;
	GLBVRS::ENEMYSPEEDMED = 20.0f;
	GLBVRS::ENEMYSPEEDHARD = 28.0f;

	//UTILITY
	GLBVRS::RSRCS = i_resourcesPtr;
	GLBVRS::MBUSPTR = i_mBusPtr;
	GLBVRS::GPTR = i_gamePtr;
	GLBVRS::PPTR = i_playerPtr;
	GLBVRS::SOUNDLVL = i_soundLvl;

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
		return 2;
		break;
	}
	case (SCATTER): { //number of shots
		return 3;
		break;
	}
	case (SMALL_SHIP): { //number of shots
		return 2;
		break;
	}
	case (BIG_SHIP): { //number of shots
		return 2;
		break;
	}
	case (BLAST): { //number of shots
		return 3;
		break;
	}
	case (WALL_BIG): { //number of shots
		return 2;
		break;
	}
	}

}

sf::VideoMode GLBVRS::GetVideoMode(RSLTN i_res)
{
	sf::VideoMode mode;
	switch (i_res) {
	case TWLV_SVN: {
		mode = sf::VideoMode(1280, 720);
		break;
	}
	case FRTN_NNTY : {
		mode = sf::VideoMode(1440, 900);
		break;
	}
	case NNTN_TN: {
		mode = sf::VideoMode(1920, 1080);
		break;
	}
	case FLL_SCRN: {
		sf::VideoMode(1920, 1080);
		break;
	}
	}
	return mode;
}

void GLBVRS::SetTextOriginCenter(sf::Text * i_textPtr)
{
	auto rect = i_textPtr->getLocalBounds();
	auto rect2 = i_textPtr->getGlobalBounds();
	float halfWidth = rect.width / 2.0f;
	float halfHeight = rect.height / 2.0f;
	i_textPtr->setOrigin(sf::Vector2f(halfWidth, halfHeight));
}

GLBVRS::GLBVRS()
{
}

//WINDOW SETTINGS
unsigned int GLBVRS::SCREEN_WIDTH = 1920;
unsigned int GLBVRS::SCREEN_HEIGHT = 1080;

//GAME RESOLUTION
float GLBVRS::CRT_WDTH = static_cast<float>(GLBVRS::SCREEN_WIDTH * 9.0f / 10.0f);
float GLBVRS::CRT_HGHT = static_cast<float>(GLBVRS::SCREEN_HEIGHT * 9.0f / 10.0f);
float GLBVRS::HR_MRG = static_cast<float>(GLBVRS::SCREEN_WIDTH / 20.0f);         //horizontal margin
float GLBVRS::VRT_MRG = static_cast<float>(GLBVRS::SCREEN_HEIGHT / 20.0f);       //vertical margin

//MENU RESOLUTION
float GLBVRS::BTTN_WDTH = GLBVRS::CRT_WDTH / 4.0f;
float GLBVRS::BTTN_HGHT = GLBVRS::CRT_HGHT / 10;
float GLBVRS::SIZE_RAT = 1;

//OBJECT SIZES
float GLBVRS::DOOR_WIDTH = 150.0f;
float GLBVRS::DOOR_HEIGHT = 50.0f;
float GLBVRS::WALL_THICKNESS = 100.0f;
float GLBVRS::PROJECTILE_RADIUS = 10.0f;

float GLBVRS::ENEMYSPEEDEASY = 12.0f;
float GLBVRS::ENEMYSPEEDMED = 15.0f;
float GLBVRS::ENEMYSPEEDHARD = 18.0f;

//UTILITY
RESOURCES* GLBVRS::RSRCS = NULL;
MessageBus* GLBVRS::MBUSPTR = NULL;
Game* GLBVRS::GPTR = NULL;
std::shared_ptr<Entity> GLBVRS::PPTR = NULL;
float GLBVRS::SOUNDLVL = 75.0f;

GLBVRS::~GLBVRS()
{
}
