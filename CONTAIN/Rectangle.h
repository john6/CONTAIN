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

	void SetWidth(float i_width);
	void SetHeight(float i_height);
	float GetWidth();
	float GetHeight();
	Vector2f GetSFMLOriginOffset();
	std::unique_ptr<sf::Shape> GetSFMLRepr();
	ShapeType GetShapeType();
	float GetDistToEdge();
	std::vector<Vector2f> GetPoints();
	/*std::vector<Vector2f> GetCorners();*/
};