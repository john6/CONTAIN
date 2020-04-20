#pragma once
#include "RigidBody.h"
#include "GLOBAL_CONSTANTS.h"

class CollisionData
{
public:
	CollisionData();
	~CollisionData();

	RigidBody* entPtr1;
	RigidBody* entPtr2;
	Vector2f normal;
	float penetration;
	std::vector<Vector2f> contactPoints;
};

