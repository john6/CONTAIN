#include "Circle.h"

Circle::Circle(float i_radius) :
	radius {i_radius}
{}

Circle::~Circle()
{
}

void Circle::SetRadius(float i_radius) { radius = i_radius; }

float Circle::GetRadius() { return radius; }

Vector2f Circle::GetSFMLOriginOffset()
{
	//not sure if I should replace this, SFML doesnt really use the radius as the center offset, but my math does 
	//IDK for now
	float adjacent = sqrt(pow(radius, 2)/ 2.0f);
	return Vector2f(radius, radius);
}

std::unique_ptr<sf::Shape> Circle::GetSFMLRepr()
{
	std::unique_ptr<sf::CircleShape> circlePtr = std::make_unique<sf::CircleShape>();
	circlePtr->setRadius(radius);
	std::unique_ptr<sf::Shape> shapePtr = move(circlePtr);
	return shapePtr;
}

Shape::ShapeType Circle::GetShapeType()
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

std::vector<Vector2f> GetPoints() {
	std::vector<Vector2f> pointVector = std::vector<Vector2f>();
	pointVector.push_back(Vector2f(0.0f, 0.0f));
	return pointVector;
}
