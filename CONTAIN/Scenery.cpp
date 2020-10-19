#include "Scenery.h"
#include "Entity.h"


Scenery::Scenery(Vector2f i_startPosition, std::shared_ptr <sf::Shape> i_drawable,
	RigidBody i_rb) : Entity(i_startPosition, i_rb, SCENERY)
{
	physicalObject = false;
	intangible = true;
	hasVisuals = true;
	drawables = std::make_shared<std::vector<std::shared_ptr<sf::Drawable>>>();
	drawables->push_back(i_drawable);
}

Scenery::~Scenery()
{
}

drawablePtrVect Scenery::CreateDrawables(float i_lerp_fraction)
{
	return drawables;
}