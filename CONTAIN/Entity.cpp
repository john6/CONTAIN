#include "Entity.h"

RigidBody::RigidBody(std::shared_ptr<Shape> i_shape) :
	shape {i_shape}
{
	transform = std::make_shared<Transform> (Vector2f(0.0f, 0.0f), Vector2f(1.0f, 1.0f), 0.0f);
}

RigidBody::~RigidBody()
{
	//delete transform 
	//I dont understand how this delete can cause an issue if I always create a fresh one when I instantiate the entity
}

void RigidBody::SetShape(std::shared_ptr<Shape> i_shape) { shape = i_shape; }

void RigidBody::SetLayer(int i_layer) { layer = i_layer;  }

void RigidBody::SetVelocity(Vector2f i_velocity) { velocity = i_velocity; }

void RigidBody::SetForce(Vector2f i_force) { force = i_force; }

void RigidBody::SetRotation(float i_rotRads)
{
	orientation = i_rotRads;
}

void RigidBody::SetTransform(std::shared_ptr <Transform> i_transform) { transform = i_transform; }

void RigidBody::SetMassData(MassData i_massData) { massData = i_massData;  }

Vector2f RigidBody::GetCenterPoint() {
	return  GetTransform()->GetPosition();
}

std::shared_ptr<Shape> RigidBody::GetShape() { return shape; }

int  RigidBody::GetLayer() { return layer; }

Vector2f RigidBody::GetVelocity() { return velocity; }

Vector2f RigidBody::GetForce() { return force; }

float RigidBody::GetRotation()
{
	return orientation;
}

std::shared_ptr <Transform> RigidBody::GetTransform() { return transform; }

MassData RigidBody::GetMassData() { return massData; }

std::unique_ptr<sf::Shape> RigidBody::CreateDrawable() {
	std::unique_ptr<sf::Shape> drawShape = shape->GetSFMLRepr();
	drawShape->setOrigin(sf::Vector2f(shape->GetSFMLOriginOffset()(0), shape->GetSFMLOriginOffset()(1)));
	drawShape->setPosition(transform->GetPosition()(0), transform->GetPosition()(1));
	drawShape->setRotation((orientation*180.0f)/PI);
	drawShape->setFillColor(sf::Color::White); //TODO: Colors
	return drawShape;
}

std::vector<Vector2f> RigidBody::SwitchCoordsToWorldSpace(std::vector<Vector2f> i_objectSpaceCoords)
{
	std::vector<Vector2f> worldSpaceCoords;
	Eigen::Rotation2D<float> rotation(orientation);
	Matrix2f rotationMatrix = rotation.toRotationMatrix();
	for (Vector2f objectSpaceCoord : i_objectSpaceCoords) {
		Vector2f worldSpacePoint = transform->GetPosition() + (rotationMatrix * objectSpaceCoord);
		worldSpaceCoords.push_back(worldSpacePoint);
	}
	return worldSpaceCoords;
}

sf::VertexArray RigidBody::CreatOrientationLine() { 
	sf::VertexArray lines = sf::VertexArray(sf::LinesStrip, 2);
	sf::Vector2f position1 = sf::Vector2f(transform->GetPosition()(0), transform->GetPosition()(1)); //WHAT THE HELL 
	Eigen::Rotation2D<float> rotation(orientation);
	Vector2f edgeOffsetFromCenter = Vector2f(shape->GetDistToEdge(), 0.0f);
	Matrix2f rotationMatrix = rotation.toRotationMatrix();
	Vector2f edgeoffSetRotated = rotationMatrix * edgeOffsetFromCenter;
	Vector2f edgePoint = transform->GetPosition() + edgeoffSetRotated;
	sf::Vector2f edgePointSFML = sf::Vector2f(edgePoint(0), edgePoint(1));
	//sf::Vector2f position2 = sf::Vector2f(edgePoint(0), edgePoint(1));
	lines[0] = sf::Vertex(position1, sf::Color::Red);
	lines[1] = sf::Vertex(edgePointSFML, sf::Color::Red);
	return lines;
}

std::vector<sf::CircleShape> RigidBody::CreateStructurePoints()
{
	std::vector<Vector2f> worldPoints = GetVertexCoords();
	std::vector<sf::CircleShape> drawables;
	for (Vector2f point : worldPoints) {
		sf::CircleShape pointRepr(3.0f);
		//Circles are drawn starting at the top left corner of their bounding box,
		//so you need to set the center to +0.5 radius in the X and Y
		pointRepr.setOrigin(1.5f, 1.5f);
		pointRepr.setPosition(point(0), point(1));
		pointRepr.setFillColor(sf::Color::Red);
		drawables.push_back(pointRepr);
	}
	return drawables;
}

std::vector<Vector2f> RigidBody::GetVertexCoords() {
	std::vector<Vector2f> objectPoints = shape->GetPoints();
	std::vector<Vector2f> worldPoints = SwitchCoordsToWorldSpace(objectPoints);
	return worldPoints;
}

std::vector<Vector2f> RigidBody::GetFaceNormals()
{ //ONLY WORKS ON RECTANGLES, gets all normals facing out
	std::vector<Vector2f> objectPoints = shape->GetPoints();
	std::vector<Vector2f> worldPoints = SwitchCoordsToWorldSpace(objectPoints);
	std::vector<Vector2f> faceNormals;
	if (shape->GetShapeType() == Shape::ShapeType::Rectangle) {
		faceNormals.push_back(Vector2f(worldPoints[2] - worldPoints[0]).unitOrthogonal()); 	//top face normal
		faceNormals.push_back(Vector2f(worldPoints[0] - worldPoints[1]).unitOrthogonal()); 	//left face normal
		faceNormals.push_back(Vector2f(worldPoints[3] - worldPoints[2]).unitOrthogonal()); 	//right face normal
		faceNormals.push_back(Vector2f(worldPoints[1] - worldPoints[3]).unitOrthogonal()); 	//bottom face normal
	}
	else if (shape->GetShapeType() == Shape::ShapeType::Circle) {
		//Do nothing this should return an empty list representing the infinite/nonexistent face normals of a circle
	}
	return faceNormals;
}
