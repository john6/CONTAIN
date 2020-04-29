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
const float GLOBAL_DECELERATION_LINEAR = 0.99f;
const float GLOBAL_DECELERATION_ANGULAR = 0.99f;
const float PENETRATION_ALLOWANCE = 0.05f;
const float PENETRATION_CORRECTION = 0.4f;
const float ANGULAR_VELOCITY_ADJUSTMENT = 0.7f;

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

//TIME STUFF
typedef std::chrono::high_resolution_clock               hiResTime;
typedef std::chrono::microseconds                        microSec;
typedef std::chrono::high_resolution_clock::time_point   hiRes_time_point;