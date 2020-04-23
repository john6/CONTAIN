#pragma once
#include "Shape.h"

class Rectangle :
	public Shape
{
private:
	float width;
	float height;

public:
	Rectangle(float i_width, float i_height);
	~Rectangle();

	Vector2f GetSFMLOriginOffset();
	std::unique_ptr<sf::Shape> GetSFMLRepr();
	ShapeType GetType();
	float GetDistToEdge();
	std::vector<Vector2f> GetPoints();
	float GetArea();
	float GetInertiaCoeff();
};