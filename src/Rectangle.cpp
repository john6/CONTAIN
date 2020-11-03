#include <CONTAIN/Rectangle.h>


Rectangle::Rectangle(float i_width, float i_height) :
	width{i_width}, height{i_height}
{
}

Rectangle::~Rectangle()
{
}

Vector2f Rectangle::GetSFMLOriginOffset() { 
	return Vector2f(width/2.0f, height/2.0f); 
}

std::shared_ptr<sf::Shape> Rectangle::GetSFMLRepr() {
	std::shared_ptr<sf::RectangleShape> rectPtr = std::make_shared<sf::RectangleShape>();
	rectPtr->setSize(sf::Vector2f(width, height));
	std::shared_ptr<sf::Shape> shapePtr = move(rectPtr);
	return shapePtr;
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

Shape::ShapeType Rectangle::GetType()
{
	return ShapeType::RECTANGLE;
}

float Rectangle::GetDistToEdge() {
	return width / 2.0f;
}

float Rectangle::GetArea()
{
	return width * height;
}

float Rectangle::GetInertiaCoeff()
{
	return (pow(width, 2) + pow(height, 2)) / 12.0f;
}

void Rectangle::ChangeSizeOfShape(float i_widthOrRadius, float i_heightOrGarbage)
{
	width = i_widthOrRadius;
	height = i_heightOrGarbage;
}

float Rectangle::GetWidth()
{
	return width;
}

float Rectangle::GetHeight()
{
	return height;
}

void Rectangle::ResetOrigin()
{
}
