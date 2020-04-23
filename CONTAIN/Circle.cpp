#include "Circle.h"

Circle::Circle(float i_radius) :
	radius {i_radius}
{}

Circle::~Circle()
{
}

Vector2f Circle::GetSFMLOriginOffset()
{ //the origin offset is the distance from the corner of the circles bounding box to the center
	return Vector2f(radius, radius);
}

std::unique_ptr<sf::Shape> Circle::GetSFMLRepr()
{
	std::unique_ptr<sf::CircleShape> circlePtr = std::make_unique<sf::CircleShape>();
	circlePtr->setRadius(radius);
	std::unique_ptr<sf::Shape> shapePtr = move(circlePtr);
	return shapePtr;
}

Shape::ShapeType Circle::GetType()
{
	return ShapeType::Circle;
}

float Circle::GetDistToEdge() {
	return radius;
}

std::vector<Vector2f> Circle::GetPoints()
{
	std::vector<Vector2f> points;
	points.push_back(Vector2f(0.0f, 0.0f));
	return points;
}

float Circle::GetArea()
{
	return PI * radius * radius;
}

float Circle::GetInertiaCoeff()
{
	return  0.5f * radius * radius;
}

std::vector<Vector2f> GetPoints() {
	std::vector<Vector2f> pointVector = std::vector<Vector2f>();
	pointVector.push_back(Vector2f(0.0f, 0.0f));
	return pointVector;
}
