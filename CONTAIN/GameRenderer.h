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

	sf::View worldView;
	sf::View hudView;

	//TODO why the fuck is this static
	void Render(sf::RenderWindow* i_window, float i_lerp_fraction, std::list<std::shared_ptr<Entity>>* i_entityList,
		Entity* playerChar, std::vector<sf::Drawable*> i_HUDelem);


	void ResetWorldSize(int width, int height);

	void SetHudSize(int width, int height);
};