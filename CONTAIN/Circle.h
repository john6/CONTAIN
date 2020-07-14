#pragma once
#include "Shape.h"

class Circle :
	public Shape
{
public:
	Circle(float i_radius);
	~Circle();

	float radius;

	Vector2f GetSFMLOriginOffset() override;
	std::unique_ptr<sf::Shape> GetSFMLRepr() override;
	ShapeType GetType() override;
	float GetDistToEdge() override;
	std::vector<Vector2f> GetPoints() override;
	float GetArea() override;
	float GetInertiaCoeff() override;
	void ChangeSizeOfShape(float i_widthOrRadius, float i_heightOrGarbage) override;
};