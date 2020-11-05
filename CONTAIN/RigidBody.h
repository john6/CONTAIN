/*
Notes:
Origin of object space is the center
*/

#pragma once
#include "Shape.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Polygon.h"
#include "Transform.h"
#include "MassData.h"
#include "Material.h"
#include "Math.h"


class RigidBody
{
private:
	//std::mutex mtx;

public:
	RigidBody(std::shared_ptr<Shape> i_shape, Material i_material = Material());
	~RigidBody();
	//RigidBody(const RigidBody &i_rb); //manual copy constructor because mutex is pissed

	std::shared_ptr<Shape> shape;
	std::vector<Vector2f> objVerts;
	std::vector<Vector2f> worldVerts;
	std::vector<Vector2f> faceNorms;
	Transform transform;
	MassData massD;
	Material mat;
	Vector2f vel;
	Vector2f force;
	float angVel;
	float torq;
	int layer;
	int numVerts;
	bool ignoreForcesThisStep;

	//This sucks in a number of ways lul, First I'm copying the vectors into it, and also Im creating arrays like three times,
	//I think I should just make a "rotateObjectSpacePoint" one that takes and returns a vector2f, I think it has to craete a new one no matter what. 
	//I guess it can take in a pointer to the original vector to avoid recreating it but then maybe access would take the same amount of time idk
	void RotateVerts();
	//Vector2f RotatePoint(Vector2f i_point);
	void VertsToWorldSpace();
	void UpdateVertsAndNorms();
	void UpdateFaceNorms();
	std::vector<Vector2f> GetVertCords();
	std::vector<Vector2f> GetFaceNorms();
	Vector2f GetInstVel();
	float GetInstAngVel();
	void SetMassData();

	void ApplyImpulse(Vector2f i_imp, Vector2f contactP);
	void AdjustPosition(Vector2f);
	void ResetPosition(Vector2f);
	void ResetOrientation(float i_orient);
	void ResetOrientation(Vector2f i_dirVect);
	void IntegrateForces();
	void IntegrateVelocity(float i_deltaTime);
	Vector2f GetLerpPosition(float i_lerp_fraction);
	float GetLerpOrient(float i_lerp_fraction);
	//Im just gonna give a single function to change the size, the circle class will just ignore the second arg
	void ChangeSizeOfShape(float i_widthOrRadius, float i_heightOrGarbage);

	//SFML drawing functions
	/*std::unique_ptr<sf::Shape> CreateDrawable(float i_lerp_fraction);*/
	sf::VertexArray CreatOrientationLine(float i_lerp_fraction);
	std::vector<sf::CircleShape> CreateStructurePoints(float i_lerp_fraction);
};