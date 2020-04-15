#pragma once
#include "GLOBAL_CONSTANTS.h"


class Shape
{
private:

public:
	enum ShapeType {Circle, Rectangle};

	Shape();
	~Shape();

	virtual Vector2f GetSFMLOriginOffset() = 0;
	virtual std::unique_ptr<sf::Shape> GetSFMLRepr() = 0;
	virtual ShapeType GetShapeType() = 0;
	virtual float GetDistToEdge() = 0;
	virtual std::vector<Vector2f> GetPoints() = 0;
};