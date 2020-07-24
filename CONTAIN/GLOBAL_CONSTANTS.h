#pragma once

#include <chrono>
#include <Eigen/Dense>
#include <SFML/Graphics.hpp>
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
const sf::Color BASIL = sf::Color(82, 89, 19);
const sf::Color PEAR = sf::Color(82, 146, 41);
const sf::Color CHARTREUSE = sf::Color(50, 100, 0);
const sf::Color EMERALD = sf::Color(31, 78, 47);
const sf::Color PISTACHIO = sf::Color(58, 77, 45);
const sf::Color SAGE = sf::Color(188, 184, 138);
const sf::Color MOSS = sf::Color(138, 154, 91);
const sf::Color BLUEVIOLET = sf::Color(138, 43, 226);
const sf::Color INDIGO = sf::Color(75, 0, 130);
const sf::Color MEDIUMPURPLE = sf::Color(147, 112, 219);
const sf::Color PURPLE = sf::Color(128, 0, 128);
const sf::Color DARKVIOLET = sf::Color(148, 0, 211);
const sf::Color DIMGRAY = sf::Color(105, 105, 105);
const sf::Color SILVER = sf::Color(192, 192, 192);
const sf::Color SLATEGRAY = sf::Color(112, 128, 144);

//~~DIFFICULTY SETTINGS~~
////
//const float ENEMYSPEEDEASY = 12;
//const float ENEMYSPEEDMED = 15;
//const float ENEMYSPEEDHARD = 18;

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
enum GAME_STATE { MENU, START_GAME, START_TUTORIAL, IN_GAME, EXIT_GAME, WIN, LOSE };
//Leave WALL_BIG as last upgrade I use it as a terminating condition for InitLvl();
enum UPGRADE_TYPE { NONE, RATE_OF_FIRE, WEAP_SPEED, SCATTER, SMALL_SHIP, BIG_SHIP, BLAST, WALL_BIG };
enum MOUSE_STATE { MOUSE_UP, MOUSE_DOWN };
enum DIFFICULTY { EASY, MEDIUM, HARD };
enum SCREEN_AREA { CORNERS, MARGINS, DOORS, CENTER };
enum SCREEN_SIDE {SCREEN_UP, SCREEN_RIGHT, SCREEN_DOWN, SCREEN_LEFT };
enum TERRAIN_AREA { TER_UP, TER_RIGHT, TER_DOWN, TER_LEFT, TER_CENT };

//enum UPGRADE_STATE { EXIT_UP_1, EXIT_UP_2, EXIT_UP_3, CONTINUE };
//enum ENTITY_TYPE { WALL, DOOR, ENEMY, PLAYER, PROJECTILE};
//enum MATERIAL_TYPE {ROCK, WOOD, METAL, BOUNCYBALL, SUPERBALL, PILLOW, STATIC, STATICVERYFRICTION, RUBBER, ICE};
enum TypeID {
	ENTITY_VIRTUAL, PLAYER, PROJ_BASIC, PROJ_WALL, BLAST_STUN, ENEMY_SEEK,
	ENEMY_RAND, ENEMY_BOSS, ENEMY_SEEK_PUSH, ENEMY_RAND_PUSH, WALL_BASIC,
	WALL_FIRE, DOOR_LOCKED, DOOR_OPEN, END_LEVEL, UPGRADE};

//TIME STUFF
typedef std::chrono::high_resolution_clock               hiResTime;
typedef std::chrono::microseconds                        microSec;
typedef std::chrono::high_resolution_clock::time_point   hiRes_time_point;

//SCORE STUFF
const float LEVEL_SCORE_INCREASE = 1000;

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

public:
	//GLOBAL_CONSTANTS(unsigned int i_screenWidth, unsigned int i_screenHeight);

	static void SetGlobalConstants(unsigned int i_screenWidth, unsigned int i_screenHeight);

	static int GetUpgradeMax(UPGRADE_TYPE i_powType);

	GLBVRS();
	~GLBVRS();


};