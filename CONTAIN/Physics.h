#pragma once
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include "GLOBAL_CONSTANTS.h"
#include "Math.h"
#include "Entity.h"
#include "RigidBody.h"
#include "CollisionData.h"


class Physics
{
private:
	static Vector2f GetMaxProjection(Vector2f i_dir, std::vector<Vector2f> i_vertices);

	static std::vector<float> FindAxisLeastPenetration(RigidBody* i_entA, RigidBody* i_entB);

	static std::vector<Vector2f> FindIncidentFace(RigidBody* i_refEnt, RigidBody* i_incidentEnt, int i_refIndex);

	static int Clip(Vector2f normal, float c, std::vector<Vector2f>* face);

	static bool ResolveCircleToCircleCollision(CollisionData* i_data);

	static bool ResolveRectToCircleCollision(CollisionData* i_data);

	//static bool ResolveCircleToRectCollision(CollisionData* i_data);

	static bool ResolveRectToRectCollision(CollisionData* i_data);

	static std::function<void(void)> SaveImpulse(RigidBody* body, Vector2f a1, Vector2f a2);

public:
	Physics();
	~Physics();

	static bool CheckCollision(CollisionData* i_collision);

	static void CreateCollisionImpulse(CollisionData* i_data);

	static void InfiniteMassCorrection(CollisionData* i_data);

	static void PositionalCorrection(CollisionData* i_data);

	static std::shared_ptr<Polygon> CreateRegularPolygon(int i_numVerts, float i_size);

	static std::shared_ptr<Polygon> CreateIrregularPolygon(int i_numVerts, float i_size, float i_minSize =1);

	static sf::Color GenerateRandomColor(int i_min = 0, int i_max = 255);
};