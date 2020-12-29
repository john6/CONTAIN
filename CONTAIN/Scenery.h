#pragma once
#include "Entity.h"

class Scenery : public Entity {
private:
	drawablePtrVect drawables;

public:
	int renderLvl;

	Scenery(Vector2f i_startPosition, std::shared_ptr <sf::Shape> i_drawable, int i_renderLvl = 0,
		RigidBody i_rb = RigidBody(std::make_shared<Circle>(1.0f)));
	~Scenery();

	drawablePtrVect CreateDrawables(float i_lerp_fraction) override;
};