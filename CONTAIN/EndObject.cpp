#include "EndObject.h"


EndObject::EndObject(Vector2f i_startPosition, RigidBody i_rb) :
	Entity(i_startPosition, i_rb, END_LEVEL)
{
	Lock();

	sf::Texture texture;
	if (!texture.loadFromFile("Textures/tile113.png", sf::IntRect(0, 0, 64, 64)))
	{
		std::cerr << "failed to load Textures/tile091.png";
	}
	else {
		texturePtr = std::make_shared<sf::Texture>(texture);
	}
	textStretchWidth = 64.0f / textRectWidth;
	textStretchHeight = 64.0f / textRectHeight;
}

EndObject::~EndObject()
{
}

void EndObject::Update(float i_stepSize)
{
}

void EndObject::Lock() {
	active = false;
	fillColor = DARKSILVER;
	outlineColor = SILVER;
}

void EndObject::Unlock() {
	active = true;
	rb.angVel = 1.0f;
	fillColor = sf::Color::Cyan;
	outlineColor = sf::Color::Magenta;
}

