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
const float WALL_THICKNESS = 100.0f;
const float MICROSECS_TO_MILLISECS = 1000.0f;
const float MAX_ROTATION_DEGREES = 45;
const float AVG_MILLISEC_PER_UPDATE = 10.0f;
const float RADIAN_ADJUST = 50.0f;
const float PROJECTILE_RADIUS = 10.0f;
const float DOOR_WIDTH = 150.0f;
const float DOOR_HEIGHT = 50.0f;
const float AOE_RADIUS = 175.0f;

//PHYSICS CONSTANTS
const Vector2f GRAVITY_COEFFICIENT = Vector2f(0.0f, 100.0f);
const float GLOBAL_DECELERATION_LINEAR = 0.001f;
const float GLOBAL_DECELERATION_ANGULAR = 0.005f;
const float PENETRATION_ALLOWANCE = 0.05f;
const float PENETRATION_CORRECTION = 0.4f;
const float ANGULAR_VELOCITY_ADJUSTMENT = 0.15f; //IDK where my angular velocity is getting so fucking jacked

//MATERIALS
const Material ROCK = Material(0.6f, 0.1f, 0.6f, 0.3f);
const Material WOOD = Material(0.3f, 0.2f, 0.5f, 0.25f);
const Material METAL = Material(1.2f, 0.05f, 0.4f, 0.2f);
const Material DENSE_METAL = Material(1.9f, 0.05f, 0.4f, 0.2f);
const Material BOUNCYBALL = Material(0.3f, 0.8f, 0.7f, 0.2f);
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

//~~DIFFICULTY SETTINGS~~
//
//EASY SETTINGS
const float SPEED_MULTIPLIER_EASY = 0.40f;
const float PLAYER_SPEED_MULTIPLIER_EASY = 0.6f;
const float LEVEL_TIME_LIMIT_EASY = 300.0f;
//MEDIUM SETTINGS
const float SPEED_MULTIPLIER_MEDIUM = 0.7f;
const float PLAYER_SPEED_MULTIPLIER_MEDIUM = 0.6f;
const float LEVEL_TIME_LIMIT_MEDIUM = 180.0f;
//HARD SETTINGS
const float SPEED_MULTIPLIER_HARD = 1.0f;
const float PLAYER_SPEED_MULTIPLIER_HARD = 0.9f;
const float LEVEL_TIME_LIMIT_HARD = 135.0f;

//ENUMS
enum GAME_STATE { MENU, START_GAME, IN_GAME, EXIT_GAME, WIN, LOSE };
enum MOUSE_STATE { MOUSE_UP, MOUSE_DOWN };
enum DIFFICULTY { EASY, MEDIUM, HARD };
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

class GLOBAL_CONSTANTS
{
public:
	//WINDOW SETTINGS
	static unsigned int SCREEN_WIDTH;
	static unsigned int SCREEN_HEIGHT;

	//GAME RESOLUTION
	static float COURT_WIDTH;
	static float COURT_HEIGHT;
	static float HOR_MARGIN;   //horizontal margin
	static float VERT_MARGIN;  //vertical margin

	//MENU RESOLUTION
	static float BUTTON_WIDTH;
	static float BUTTON_HEIGHT;


public:
	//GLOBAL_CONSTANTS(unsigned int i_screenWidth, unsigned int i_screenHeight);

	static void SetGlobalConstants(unsigned int i_screenWidth, unsigned int i_screenHeight);

	GLOBAL_CONSTANTS();
	~GLOBAL_CONSTANTS();


};