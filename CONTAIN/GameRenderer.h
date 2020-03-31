#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

class GameRenderer
{
private:
	sf::RenderWindow* window;
	sf::Vector2f screenDimensions;

public:
	GameRenderer(sf::Vector2f i_screenDimensions, sf::RenderWindow* i_window);
	~GameRenderer();

	static void Render(float i_elapsedMilliseconds, std::vector<Entity> i_entityList);

	static sf::RectangleShape CreateDrawableEntity(Entity i_entity);
};