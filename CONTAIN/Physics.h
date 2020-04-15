#pragma once
#include <algorithm>
#include "GLOBAL_CONSTANTS.h"
#include "Entity.h"
#include "CollisionData.h"
#include <iostream>

class Physics
{
private:
	static bool ResolveCircleCollision(CollisionData* i_data);

	static bool ResolveRectangleCollision(CollisionData* i_data);

	static bool ResolveRectToCircleCollision(CollisionData* i_data);

	//SUPPORT FUNCTIONS
	static Vector2f GetMaxProjection(Vector2f i_dir, std::vector<Vector2f> i_vertices);

	static std::vector<float> FindAxisLeastPenetration(RigidBody* i_entA, RigidBody* i_entB);

public:
	Physics();
	~Physics();

	static void TestCollision(RigidBody * i_ent1, RigidBody * i_ent2);

	static void MoveEntity(RigidBody* entity, float i_stepSize);

	static bool CheckForCollisionAlongNormals(std::vector<Vector2f> normalVect, std::vector<Vector2f> vertexVect1, std::vector<Vector2f> vertexVect2);

	static void ApplyImpulse(CollisionData* i_data);

	static std::vector<Vector2f> FindIncidentFace(RigidBody* i_refEnt, RigidBody* i_incidentEnt, int i_refIndex);

};

