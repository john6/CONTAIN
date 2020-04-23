#pragma once
#include "Shape.h"

class Circle :
	public Shape
{
public:
	Circle(float i_radius);
	~Circle();

	float radius;

	Vector2f GetSFMLOriginOffset();
	std::unique_ptr<sf::Shape> GetSFMLRepr();
	ShapeType GetType();
	float GetDistToEdge();
	std::vector<Vector2f> GetPoints();
	float GetArea();
	float GetInertiaCoeff();
};