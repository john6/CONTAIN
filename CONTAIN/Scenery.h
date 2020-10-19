#pragma once
#include "Entity.h"

class Scenery : public Entity {
private:
	drawablePtrVect drawables;

public:

	Scenery(Vector2f i_startPosition, std::shared_ptr <sf::Shape> i_drawable,
		RigidBody i_rb = RigidBody(std::make_shared<Circle>(1.0f)));
	~Scenery();

	drawablePtrVect CreateDrawables(float i_lerp_fraction) override;
};