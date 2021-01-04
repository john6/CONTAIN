#include "Blocker.h"

Blocker::Blocker(Vector2f i_startPosition, RigidBody i_rb) :
	Entity(i_startPosition, i_rb, PROJ_WALL)
{
	fillColor = BLUEVIOLET;
	outlineColor = BLUEVIOLET;

	sf::Texture texture;
	if (!texture.loadFromFile("Textures/tile111.png"))
	{
		std::cerr << "failed to load Textures/tile111.png";
	}
	else {
		texturePtr = std::make_shared<sf::Texture>(texture);
		textStretchHeight = 1;
		textStretchWidth = (32.0f / textRectWidth);
	}
}

Blocker::~Blocker()
{
}