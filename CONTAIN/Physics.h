#pragma once
#include <algorithm>
#include "Math.h"
#include "GLOBAL_CONSTANTS.h"
#include "RigidBody.h"
#include "CollisionData.h"
#include <iostream>

class Physics
{
private:
	static Vector2f GetMaxProjection(Vector2f i_dir, std::vector<Vector2f> i_vertices);

	static std::vector<float> FindAxisLeastPenetration(RigidBody* i_entA, RigidBody* i_entB);

	static std::vector<Vector2f> FindIncidentFace(RigidBody* i_refEnt, RigidBody* i_incidentEnt, int i_refIndex);

	static int Clip(Vector2f normal, float c, std::vector<Vector2f>* face);

	static bool ResolveCircleToCircleCollision(CollisionData* i_data);

	static bool ResolveRectToCircleCollision(CollisionData* i_data);

	static bool ResolveRectToRectCollision(CollisionData* i_data);

	static void CreateCollisionImpulse(CollisionData* i_data);

public:
	Physics();
	~Physics();

	static void CheckCollision(RigidBody * i_ent1, RigidBody * i_ent2);

	static void PositionalCorrection(CollisionData* i_data);

	//static void TimeStepRigidBody(RigidBody* i_entity, float i_stepSize);
};

