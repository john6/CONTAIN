#pragma once

#include <chrono>
#include <memory>
#include <Eigen/Dense>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include "Material.h"

//https://www.fluentcpp.com/2019/07/23/how-to-define-a-global-constant-in-cpp/
//according to this article I should be adding extern here but that means I have to define these variables elsewhere also because I dont use CPP17
//07/13/2020- I am now using CPP17, things are still working without extern, maybe I should switch it still. TODO

//EIGEN STUFF
//Using a typedef jus to remove one level of namespace might be a little gratuitous but everyone keeps telling me namespaces are bad sooo
typedef Eigen::Vector2f Vector2f;
typedef Eigen::Matrix<float, 2, 2> Matrix2f;
//https://stackoverflow.com/questions/612328/difference-between-struct-and-typedef-struct-in-c
//This is an anonymous structure, mapCoord exists in typedef namespace but not tag namespace,
//this means if I want to forward declare it I have to name it first, thats fine because 
//GLOBAL CONSTANTS isnt gonna be forward delcared anywhere I dont think
//apparently I cant use mapcoords in constructors fuuuck
typedef struct MapCoord 
{ 
	MapCoord() {
		x = -1;
		y = -1;
	}

	MapCoord(int i_x, int i_y) {
		x = i_x;
		y = i_y;
	}
	int x;
	int y; 
} MapCoord;

//GENERAL GAME SETTINGS
//const float WALL_THICKNESS = 100.0f;
const float MICROSECS_TO_MILLISECS = 1000.0f;
const float MAX_ROTATION_DEGREES = 45;
const float AVG_MILLISEC_PER_UPDATE = 10.0f;
const float RADIAN_ADJUST = 50.0f;
//const float PROJECTILE_RADIUS = 10.0f;
//const float DOOR_WIDTH = 150.0f;
//const float DOOR_HEIGHT = 50.0f;
const float MAX_NUM_BLOCKERS = 10;
//const float AOE_RADIUS = 175.0f;

//PHYSICS CONSTANTS
const Vector2f GRAVITY_COEFFICIENT = Vector2f(0.0f, 100.0f);
const float GLOBAL_DECELERATION_LINEAR = 0.001f;
const float GLOBAL_DECELERATION_ANGULAR = 0.005f;
const float PENETRATION_ALLOWANCE = 0.05f;
const float PENETRATION_CORRECTION = 0.4f;
const float ANGULAR_VELOCITY_ADJUSTMENT = 0.15f; //IDK where my angular velocity is getting so fucking jacked
const float SQRT_TWO = 1.4142857;

//MATERIALS
const Material ROCK = Material(0.6f, 0.1f, 0.6f, 0.3f);
const Material WOOD = Material(0.3f, 0.2f, 0.5f, 0.25f);
const Material METAL = Material(1.2f, 0.05f, 0.4f, 0.2f);
const Material HEAVYBOUNCE = Material(1.2f, 0.6f, 0.4f, 0.2f);
const Material DENSE_METAL = Material(2.2f, 0.05f, 0.4f, 0.2f);
const Material BOUNCYBALL = Material(0.35f, 0.8f, 0.7f, 0.2f);
const Material LESSBOUNCYBALL = Material(0.35f, 0.6f, 0.7f, 0.2f);
const Material HEAVYBALL = Material(0.9f, 0.95f, 0.5f, 0.25f);
const Material SUPERBALL = Material(0.3f, 0.95f, 0.5f, 0.25f);
const Material PILLOW = Material(0.1f, 0.2f, 0.4f, 0.2f);
const Material STATIC = Material(0.0f, 0.0f, 0.4f, 0.2f);
const Material STATICVERYFRICTION = Material(0.0f, 0.0f, 0.9f, 0.7f);
const Material RUBBER = Material(0.3f, 0.8f, 0.9f, 0.6f);
const Material ICE = Material(0.3f, 0.1f, 0.7f, 0.05f);
const Material DEFAULT = Material(0.3f, 0.2f, 0.5f, 0.25f);

//TRIG CONSTANTS
const float DEGREE_RIGHT = 0.0f;
const float DEGREE_UP = 90.0f;
const float DEGREE_LEFT = 180.0f;
const float DEGREE_DOWN = -270.0f;
const float PI = 3.1459f;
const float RADIAN_RIGHT = 0.0f;
const float RADIAN_UP = PI / 2.0f;
const float RADIAN_LEFT = PI;
const float RADIAN_DOWN = 3.0f * PI / 2.0f;
const Vector2f UP(0.0f, 1.0f);
const Vector2f DOWN(0.0f, -1.0f);
const Vector2f LEFT(-1.0f, 0.0f);
const Vector2f RIGHT(1.0f, 0.0f);
const Vector2f NULL_VECTOR(0.0f, 0.0f);

//Colors
//SHADES OF GRAY ship colors
const sf::Color WHITE = sf::Color(255, 255, 255);
const sf::Color BLACK = sf::Color(0, 0, 0);
const sf::Color OFFBLACK0 = sf::Color(5, 5, 5);
const sf::Color OFFBLACK1 = sf::Color(10, 10, 10);
const sf::Color OFFBLACK2 = sf::Color(15, 15, 15);
const sf::Color OFFBLACK3 = sf::Color(20, 20, 20);
const sf::Color OFFBLACK4 = sf::Color(25, 25, 25);
const sf::Color OFFBLACK5 = sf::Color(30, 30, 30);
const sf::Color OFFBLACK6 = sf::Color(35, 35, 35);
const sf::Color OFFBLACK7 = sf::Color(40, 40, 40);
const sf::Color OFFBLACK8 = sf::Color(45, 45, 45);
const sf::Color OFFBLACK9 = sf::Color(50, 50, 50);
const sf::Color METALLICSILVER = sf::Color(170, 169, 173);
const sf::Color SONICSILVER = sf::Color(117, 117, 117);
const sf::Color DARKSILVER = sf::Color(113, 112, 110);
const sf::Color DARKCHRYSLERSILVER = sf::Color(108, 122, 134);
const sf::Color DIMGRAY = sf::Color(105, 105, 105);
const sf::Color SILVER = sf::Color(192, 192, 192);
const sf::Color LIGHTSILVER = sf::Color(216, 216, 216);
const sf::Color FORDSILVER = sf::Color(197, 206, 212);
const sf::Color SLATEGRAY = sf::Color(112, 128, 144);
const sf::Color GUNMETAL = sf::Color(42, 52, 57);
//GREENS - generally an enemy color
const sf::Color BASIL = sf::Color(82, 89, 19);
const sf::Color PEAR = sf::Color(82, 146, 41);
const sf::Color CHARTREUSE = sf::Color(50, 100, 0);
const sf::Color EMERALD = sf::Color(31, 78, 47);
const sf::Color PISTACHIO = sf::Color(58, 77, 45);
const sf::Color SAGE = sf::Color(188, 184, 138);
const sf::Color MOSS = sf::Color(138, 154, 91);
const sf::Color MIDNIGHTMOSS = sf::Color(24, 40, 30);
const sf::Color ARMY = sf::Color(75, 83, 32);
const sf::Color PARAKEET = sf::Color(182, 215, 44);
const sf::Color SEAFOAM = sf::Color(113, 238, 184);
//BLUES
const sf::Color BLUEVIOLET = sf::Color(138, 43, 226);
const sf::Color INDIGO = sf::Color(75, 0, 130);
const sf::Color MEDIUMPURPLE = sf::Color(147, 112, 219);
const sf::Color PURPLE = sf::Color(128, 0, 128);
const sf::Color DARKVIOLET = sf::Color(148, 0, 211);
const sf::Color CYAN = sf::Color(0, 255, 255);
//YELLOWS
const sf::Color YELLOWCYBER = sf::Color(255, 211, 0);
const sf::Color SUNFLOWERYELLOW = sf::Color(227, 187, 28);
//REDS
const sf::Color RED = sf::Color(255, 0, 0);
const sf::Color DARKRED = sf::Color(139, 0, 0);
const sf::Color FIREBRICK = sf::Color(178, 34, 34);
const sf::Color REDCINNIBAR = sf::Color(246, 65, 45);
const sf::Color MYSTICRED = sf::Color(255, 86, 7);
const sf::Color VIVIDGAMBOGE= sf::Color(255, 152, 0);
const sf::Color AMBER = sf::Color(255, 193, 0);
const sf::Color VIVIDYELLOW= sf::Color(255, 236, 25);
//BROWNS
const sf::Color SADDLEBROWN = sf::Color(139, 69, 19);
const sf::Color WHEATBROWN = sf::Color(245, 222, 173);
const sf::Color BURLYWOODBROWN = sf::Color(222, 184, 135);
const sf::Color SIENNEBROWN = sf::Color(160, 82, 45);
const sf::Color UMBERBROWN = sf::Color(39, 32, 28);
const sf::Color PENNYBROWN = sf::Color(173, 111, 105);
const sf::Color BONE = sf::Color(227, 218, 201);

//EASY SETTINGS
//const float SPEED_MULTIPLIER_EASY = 0.40f;
//const float PLAYER_SPEED_MULTIPLIER_EASY = 0.6f;
//const float LEVEL_TIME_LIMIT_EASY = 300.0f;


//MEDIUM SETTINGS
//const float SPEED_MULTIPLIER_MEDIUM = 0.7f;
//const float PLAYER_SPEED_MULTIPLIER_MEDIUM = 0.6f;
//const float LEVEL_TIME_LIMIT_MEDIUM = 180.0f;

//HARD SETTINGS
//const float SPEED_MULTIPLIER_HARD = 1.0f;
//const float PLAYER_SPEED_MULTIPLIER_HARD = 0.9f;
//const float LEVEL_TIME_LIMIT_HARD = 135.0f;

//ENUMS
enum GAME_STATE { MENU, SETTINGS, APPLY, START_GAME, START_TUTORIAL, IN_GAME, EXIT_GAME, WIN, LOSE };
//Leave WALL_BIG as last upgrade I use it as a terminating condition for InitLvl();
//This is required because youre not supposed to tierate over enums
//https://stackoverflow.com/questions/261963/how-can-i-iterate-over-an-enum
enum UPGRADE_TYPE { NONE, RATE_OF_FIRE, WEAP_SPEED, SCATTER, SMALL_SHIP, BIG_SHIP, BLAST, TEMP_HEALTH, WALL_BIG };
enum MOUSE_STATE { MOUSE_UP, MOUSE_DOWN };
enum DIFFICULTY { EASY, MEDIUM, HARD, TUTORIAL };
enum SCREEN_AREA { CORNERS, MARGINS, DOORS, CENTER };
enum SCREEN_SIDE {SCREEN_UP, SCREEN_RIGHT, SCREEN_DOWN, SCREEN_LEFT };
enum TERRAIN_AREA { TER_UP, TER_RIGHT, TER_DOWN, TER_LEFT, TER_CENT };
enum TypeID {
	ENTITY_VIRTUAL, PLAYER, PROJ_BASIC, PROJ_WALL, BLAST_STUN, ENEMY_SEEK,
	ENEMY_RAND, ENEMY_BOSS, ENEMY_SEEK_PUSH, ENEMY_RAND_PUSH, WALL_BASIC,
	WALL_FIRE, DOOR, END_LEVEL, UPGRADE, ANIMATION, SCENERY};
enum ANIMTYPE { CANNED_EXPLOSION, ENEMY_BURST_DEATH};
enum SND_LVL { LOW25, MEDIUM50, HIGH75, FULL100};
enum RSLTN { TWLV_SVN, FRTN_NNTY, NNTN_TN, FLL_SCRN};


//TIME STUFF
typedef std::chrono::high_resolution_clock               hiResTime;
typedef std::chrono::microseconds                        microSec;
typedef std::chrono::high_resolution_clock::time_point   hiRes_time_point;


//TYPEDEFS
typedef std::shared_ptr<std::vector<std::shared_ptr<sf::Drawable>>> drawablePtrVect;


//SCORE STUFF
const float LEVEL_SCORE_INCREASE = 1000;

class RESOURCES;
class Game;
class Entity;

class GLBVRS
{
public:
	//WINDOW SETTINGS
	static unsigned int SCREEN_WIDTH;
	static unsigned int SCREEN_HEIGHT;

	//GAME RESOLUTION
	static float CRT_WDTH; //Court width- amount of non-margin screen space
	static float CRT_HGHT; //Court height- amount of non-margin screen space
	static float HR_MRG;   //horizontal margin
	static float VRT_MRG;  //vertical margin
	static float SIZE_RAT;

	//MENU RESOLUTION
	static float BTTN_WDTH; // Button width 
	static float BTTN_HGHT; // button height

	//OBJECT SIZES
	static float DOOR_WIDTH;
	static float DOOR_HEIGHT;
	static float WALL_THICKNESS;
	static float PROJECTILE_RADIUS;

	//SPEED ADJUSTMENTS
	static float ENEMYSPEEDEASY;
	static float ENEMYSPEEDMED;
	static float ENEMYSPEEDHARD;

	//UTILITY
	static RESOURCES* RSRCS;
	static Game* GPTR;
	static std::shared_ptr<Entity> PPTR;
	static float SOUNDLVL;

public:
	static void SetGlobalConstants(unsigned int i_screenWidth, unsigned int i_screenHeight, RESOURCES* i_resourcesPtr, Game* i_gamePtr, std::shared_ptr<Entity> i_playerPtr, float i_soundLvl);

	static int GetUpgradeMax(UPGRADE_TYPE i_powType);

	static sf::VideoMode GetVideoMode(RSLTN i_res);

	static void SetTextOriginCenter(sf::Text * i_textPtr);

	GLBVRS();
	~GLBVRS();

};