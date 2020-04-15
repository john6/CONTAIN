#pragma once
#include "Entity.h"
#include "GLOBAL_CONSTANTS.h"

class CollisionData
{
public:
	CollisionData();
	~CollisionData();

	RigidBody* entPtr1;
	RigidBody* entPtr2;
	float penetration;
	Vector2f normal;
};

