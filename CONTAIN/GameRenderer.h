#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

class GameRenderer
{
private:
	sf::RenderWindow* window;
	sf::Vector2f screenDimensions;

public:
	GameRenderer();
	~GameRenderer();

	static void Render(sf::RenderWindow* i_window, float i_elapsedMilliseconds, std::vector<RigidBody>* i_entityList);
};