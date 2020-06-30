#pragma once

#include <chrono>
#include <Eigen/Dense>
#include <SFML/Graphics.hpp>

//https://www.fluentcpp.com/2019/07/23/how-to-define-a-global-constant-in-cpp/
//according to this article I should be adding extern here but that means I have to define these variables elsewhere also because I dont use CPP17

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

//WINDOW RESOLUTION
const int SCREEN_WIDTH = 1800;
const int SCREEN_HEIGHT = 900;

//GAME RESOLUTION
const float COURT_WIDTH = static_cast<float>(SCREEN_WIDTH * 8 / 9);
const float COURT_HEIGHT = static_cast<float>(SCREEN_HEIGHT * 8 / 9);
const float HOR_MARGIN = static_cast<float>(SCREEN_WIDTH / 18);         //horizontal margin
const float VERT_MARGIN = static_cast<float>(SCREEN_HEIGHT / 18);       //vertical margin

//GENERAL GAME SETTINGS
const float WALL_THICKNESS = 100.0f;
const float MICROSECS_TO_MILLISECS = 1000.0f;
const float MAX_ROTATION_DEGREES = 45;
const float AVG_MILLISEC_PER_UPDATE = 10.0f;
const float RADIAN_ADJUST = 50.0f;
const float PROJECTILE_RADIUS = 10.0f;

//PHYSICS CONSTANTS
const Vector2f GRAVITY_COEFFICIENT = Vector2f(0.0f, 100.0f);
const float GLOBAL_DECELERATION_LINEAR = 0.999f;
const float GLOBAL_DECELERATION_ANGULAR = 0.999f;
const float PENETRATION_ALLOWANCE = 0.05f;
const float PENETRATION_CORRECTION = 0.4f;
const float ANGULAR_VELOCITY_ADJUSTMENT = 0.1f; //IDK where my angular velocity is getting so fucking jacked

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

//MENU SETTINGS
const float BUTTON_WIDTH = SCREEN_WIDTH / 3;
const float BUTTON_HEIGHT = SCREEN_HEIGHT / 10;


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
enum ENTITY_TYPE { WALL, DOOR, ENEMY, PLAYER, PROJECTILE};
enum MATERIAL_TYPE {ROCK, WOOD, METAL, BOUNCYBALL, SUPERBALL, PILLOW, STATIC, STATICVERYFRICTION, RUBBER, ICE};
//TIME STUFF
typedef std::chrono::high_resolution_clock               hiResTime;
typedef std::chrono::microseconds                        microSec;
typedef std::chrono::high_resolution_clock::time_point   hiRes_time_point;