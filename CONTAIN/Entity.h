#pragma once
#include "GLOBAL_CONSTANTS.h"
#include "Shape.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Transform.h"
#include "MassData.h"

class RigidBody
{
private:
	std::shared_ptr<Shape> shape;
	//Transform is just gonna straight up point to the center now, that will be the origin in my object space
	std::shared_ptr<Transform> transform;
	int layer;
	//linear stuff
	Vector2f velocity;
	float acceleration;
	//angular stuff
	float orientation; //radians
	float angularVelocity;


	Vector2f force;
	MassData massData;
	//Not yet included
	//Material material;
	//float gravityScale;

	std::vector<Vector2f> SwitchCoordsToWorldSpace(std::vector<Vector2f> i_objectSpaceCoords);

public:
	float torque;


	RigidBody(std::shared_ptr<Shape> i_shape);
	~RigidBody();
	void SetShape(std::shared_ptr<Shape> i_shape);
	void SetLayer(int i_layer);
	void SetVelocity(Vector2f i_velocity);
	void SetForce(Vector2f i_force);
	void SetRotation(float i_rotRads);
	void SetTransform(std::shared_ptr<Transform> i_transform);
	void SetMassData(MassData i_massData);
	Vector2f GetCenterPoint();
	std::shared_ptr<Shape> GetShape();
	int GetLayer();
	Vector2f GetVelocity();
	Vector2f GetForce();
	float GetRotation();
	std::shared_ptr<Transform> GetTransform();
	MassData GetMassData();
	std::vector<Vector2f> GetVertexCoords();
	std::vector<Vector2f> GetFaceNormals();


	//SFML drawing functions
	std::unique_ptr<sf::Shape> CreateDrawable();
	sf::VertexArray CreatOrientationLine();
	std::vector<sf::CircleShape> CreateStructurePoints();


};