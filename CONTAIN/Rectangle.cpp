#include "Rectangle.h"


Rectangle::Rectangle(float i_width, float i_height) :
	width{i_width}, height{i_height}
{
}


Rectangle::~Rectangle()
{
}

void Rectangle::SetWidth(float i_width) { width = i_width; }

void Rectangle::SetHeight(float i_height) { height = i_height; }

float Rectangle::GetWidth() { return width; }

float Rectangle::GetHeight() { return height; }

Vector2f Rectangle::GetSFMLOriginOffset() { return Vector2f(width/2.0f, height/2.0f); }

std::unique_ptr<sf::Shape> Rectangle::GetSFMLRepr() {
	std::unique_ptr<sf::RectangleShape> rectPtr = std::make_unique<sf::RectangleShape>();
	rectPtr->setSize(sf::Vector2f(GetWidth(), GetHeight()));
	std::unique_ptr<sf::Shape> shapePtr = move(rectPtr);
	return shapePtr;
}

Shape::ShapeType Rectangle::GetShapeType()
{
	return ShapeType::Rectangle;
}

float Rectangle::GetDistToEdge() {
	return width/2.0f;
}

std::vector<Vector2f> Rectangle::GetPoints()
{ //returns four points representing the four corners of the rectangle, with the origin as the center
	float halfWidth = width / 2.0f;
	float halfHeight = height / 2.0f;
	std::vector<Vector2f> pointVector = std::vector<Vector2f>();
	pointVector.push_back(Vector2f(-halfWidth, halfHeight)); //top left corner
	pointVector.push_back(Vector2f(halfWidth, halfHeight)); //top right corner
	pointVector.push_back(Vector2f(halfWidth, -halfHeight)); //bottom right corner
	pointVector.push_back(Vector2f(-halfWidth, -halfHeight)); //bottom left corner
	return pointVector;
}
