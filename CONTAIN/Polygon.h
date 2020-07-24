#pragma once
#include "Shape.h"
#include <math.h>
#include "Math.h"

class Polygon :
	public Shape
{
private:
	int numPoints;
	std::vector<Vector2f> pointArr;

public:
	Polygon(std::vector<Vector2f> i_pointArr);
	~Polygon();

	Vector2f GetSFMLOriginOffset() override;
	std::unique_ptr<sf::Shape> GetSFMLRepr() override;
	ShapeType GetType() override;
	float GetDistToEdge() override;
	std::vector<Vector2f> GetPoints() override;
	float GetArea() override;
	float GetInertiaCoeff() override;
	void ChangeSizeOfShape(float i_widthOrRadius, float i_heightOrGarbage) override;

	void ReCenter();
};