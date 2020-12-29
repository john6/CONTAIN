#pragma once
#include <SFML/Graphics.hpp>
#include "GLOBAL_CONSTANTS.h"


class PlayerController
{
private:
	sf::RenderWindow* rWindow;

public:
	enum Input {UP, DOWN, LEFT, RIGHT};

	PlayerController(sf::RenderWindow* i_renderWindow);
	~PlayerController();


	Eigen::Vector2f PollWASDKeys();

	Eigen::Vector2f LeftClick();
	Eigen::Vector2f RightClick();
	Eigen::Vector2f GetMousePos();
	Eigen::Vector2f GetLeftStickVect();
	Eigen::Vector2f GetRightStickVect();
	Eigen::Vector2f GetMovementDir();
	Eigen::Vector2f GetAimDir();

	bool IsRightTriggerPressed();
	bool IsLeftTriggerPressed();
	bool IsControllerConnected();
	bool IsRightStickPressed();
	bool IsStartPressed();

	bool ScrollClick();
};