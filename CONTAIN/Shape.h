#pragma once
#include "GLOBAL_CONSTANTS.h"


class Shape
{
private:

public:
	enum ShapeType {CIRCLE, RECTANGLE, POLYGON};

	Shape();
	~Shape();

	virtual Vector2f GetSFMLOriginOffset() = 0;
	virtual std::shared_ptr<sf::Shape> GetSFMLRepr() = 0;
	virtual ShapeType GetType() = 0;
	virtual float GetDistToEdge() = 0;
	virtual std::vector<Vector2f> GetPoints() = 0;
	virtual float GetArea() = 0;
	virtual float GetInertiaCoeff() = 0;
	virtual void ChangeSizeOfShape(float i_widthOrRadius, float i_heightOrGarbage) = 0;
};