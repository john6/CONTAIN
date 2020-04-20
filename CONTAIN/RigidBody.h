/*
Notes:
	Origin of object space is the center
*/

#pragma once
#include "GLOBAL_CONSTANTS.h"
#include "Shape.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Transform.h"
#include "MassData.h"
#include "Math.h"

class RigidBody
{
private:
	std::shared_ptr<Shape> shape;

	Vector2f velocity;
	Vector2f force;
	float angularVelocity;
	float torque;
	//TODO: friction
	//Material material;

	std::vector<Vector2f> SwitchCoordsToWorldSpace(std::vector<Vector2f> i_objectSpaceCoords);
	//"Center point" should now just be the transform.position
	Vector2f GetCenterPoint();


public:
	int layer;
	Transform transform;
	MassData massData;


	RigidBody(std::shared_ptr<Shape> i_shape);
	~RigidBody();

	//void SetShape(std::shared_ptr<Shape> i_shape);

	std::shared_ptr<Shape> GetShape();
	std::vector<Vector2f> GetVertexCoords();
	std::vector<Vector2f> GetFaceRectNormals();
	Vector2f GetVelocity();
	float GetAngularVelocity();
	void SetVelocity(Vector2f i_vel);
	void SetAngularVelocity(float i_vel);
	void SetOrientation(float i_orient);

	void ApplyImpulse(Vector2f i_imp, Vector2f contactP);
	//void ApplyRotationalImpulse(float i_imp);
	void IntegrateForces();
	void IntegrateVelocity(float i_deltaTime);

	//SFML drawing functions
	std::unique_ptr<sf::Shape> CreateDrawable();
	sf::VertexArray CreatOrientationLine();
	std::vector<sf::CircleShape> CreateStructurePoints();
};