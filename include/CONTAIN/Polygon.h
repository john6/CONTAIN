#pragma once
#include "Shape.h"
#include <math.h>
#include "Math.h"

class Polygon :
	public Shape
{
private:

	std::vector<Vector2f> pointArr;
	float currSizeRat;
	float area;
	float inertiaCoeff;

	void CalculateValues();

public:
	int numPoints;

	Polygon(std::vector<Vector2f> i_pointArr);
	~Polygon();

	Vector2f GetSFMLOriginOffset() override;
	std::shared_ptr<sf::Shape> GetSFMLRepr() override;
	ShapeType GetType() override;
	float GetDistToEdge() override;
	std::vector<Vector2f> GetPoints() override;
	float GetArea() override;
	float GetInertiaCoeff() override;
	void ChangeSizeOfShape(float i_widthOrRadius, float i_heightOrGarbage) override;
	void ResizeMutliple(float i_sizeFactor);
	void ResetSize(float i_sizeFactor);
	float GetDistToCorner();
};