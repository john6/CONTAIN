#pragma once
#include "Shape.h"

class Circle :
	public Shape
{
private:
	float radius;

public:
	Circle(float i_radius);
	~Circle();

	void SetRadius(float i_radius);
	float GetRadius();
	Vector2f GetSFMLOriginOffset();
	std::unique_ptr<sf::Shape> GetSFMLRepr();
	ShapeType GetShapeType();
	float GetDistToEdge();
	std::vector<Vector2f> GetPoints();
};