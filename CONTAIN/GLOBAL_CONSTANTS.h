#pragma once

#include <chrono>
#include <Eigen/Dense>
#include <SFML/Graphics.hpp>

//https://www.fluentcpp.com/2019/07/23/how-to-define-a-global-constant-in-cpp/
//according to this article I should be adding extern here but that means I have to define these variables elsewhere also because I dont use CPP17

//WINDOW RESOLUTION
const int SCREEN_WIDTH = 1800;
const int SCREEN_HEIGHT = 900;

//GAME RESOLUTION
const float COURT_WIDTH = static_cast<float>(SCREEN_WIDTH * 8 / 9);
const float COURT_HEIGHT = static_cast<float>(SCREEN_HEIGHT * 8 / 9);
const float HORIZONTAL_MARGIN = static_cast<float>(SCREEN_WIDTH / 9);
const float VERTICLE_MARGIN = static_cast<float>(SCREEN_HEIGHT / 9);

//GENERAL GAME SETTINGS
const float WALL_THICKNESS = 15.0f;
const float MICROSECONDS_TO_MILLISECONDS = 1000.0f;
const float MAX_ROTATION_DEGREES = 45;

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

//MENU SETTINGS
const float BUTTON_WIDTH = SCREEN_WIDTH / 3;
const float BUTTON_HEIGHT = SCREEN_HEIGHT / 10;


//~~DIFFICULTY SETTINGS~~
//
//EASY SETTINGS
const float SPEED_PER_MILLISECOND_EASY = 0.40f;
const float PLAYER_SPEED_PER_MILLISECOND_EASY = 0.6f;
const float SECONDS_TO_WIN_EASY = 300.0f;
//MEDIUM SETTINGS
const float SPEED_PER_MILLISECOND_MEDIUM = 0.7f;
const float PLAYER_SPEED_PER_MILLISECOND_MEDIUM = 0.6f;
const float SECONDS_TO_WIN_MEDIUM = 180.0f;
//HARD SETTINGS
const float SPEED_PER_MILLISECOND_HARD = 1.0f;
const float PLAYER_SPEED_PER_MILLISECOND_HARD = 0.9f;
const float SECONDS_TO_WIN_HARD = 135.0f;

//ENUMS
enum GAME_STATE { MENU, START_GAME, IN_GAME, EXIT_GAME, WIN, LOSE };
enum MOUSE_STATE { UP, DOWN };
enum DIFFICULTY { EASY, MEDIUM, HARD };
enum ENTITY_TYPE { WALL, DOOR, ENEMY, PLAYER, PROJECTILE};

//TIME STUFF
typedef std::chrono::high_resolution_clock               hiResTime;
typedef std::chrono::microseconds                        microSec;
typedef std::chrono::high_resolution_clock::time_point   hiRes_time_point;


//EIGEN STUFF
//Using a typedef jus to remove one level of namespace might be a little gratuitous but everyone keeps telling me namespaces are bad sooo
typedef Eigen::Vector2f Vector2f;
typedef Eigen::Matrix<float, 2, 2> Matrix2f;