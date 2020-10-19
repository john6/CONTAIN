#pragma once
#include "Entity.h"


class Blocker : public Entity
{
public:
	Blocker(Vector2f i_startPosition, RigidBody i_rb = RigidBody(std::make_shared<Rectangle>(40, 270), HEAVYBALL));
	~Blocker();
};