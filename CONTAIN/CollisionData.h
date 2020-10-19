#pragma once
#include "RigidBody.h"
#include "GLOBAL_CONSTANTS.h"

class Entity;

class CollisionData
{
public:
	CollisionData();
	~CollisionData();

	std::shared_ptr<Entity> entA;  //First entity involved in the collision
	std::shared_ptr<Entity> entB;  //Second entity involved in the collision
	Vector2f norm;     //collision normal, the line drawn from rigidbodyA's point of contact to rigidbodyB's
	float pen;         //penetration: the deepest overlap between shapes
	std::vector<Vector2f> contactPoints; //the points where rigidbodyA and rigidbodyB contact each other
};

