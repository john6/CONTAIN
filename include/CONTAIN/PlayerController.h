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


	std::vector<Input> PollKeys();

	Vector2f LeftClick();
	Vector2f RightClick();
	Vector2f GetMousePos();
	bool ScrollClick();
};