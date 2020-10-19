#include "EndObject.h"


EndObject::EndObject(Vector2f i_startPosition, RigidBody i_rb) :
	Entity(i_startPosition, i_rb, END_LEVEL)
{
	Lock();
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

