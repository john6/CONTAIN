#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include "Entity.h"

class GameRenderer
{
private:
	sf::RenderWindow* window;
	sf::Vector2f screenDimensions;

public:
	GameRenderer();
	~GameRenderer();

	static void Render(sf::RenderWindow* i_window, float i_lerp_fraction, std::list<Entity>* i_entityList, Entity playerChar, std::vector<sf::VertexArray> i_sections);

};