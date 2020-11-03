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

	Vector2f GetSFMLOriginOffset() override;
	std::shared_ptr<sf::Shape> GetSFMLRepr() override;
	ShapeType GetType() override;
	float GetDistToEdge() override;
	std::vector<Vector2f> GetPoints() override;
	float GetArea() override;
	float GetInertiaCoeff() override;
	void ChangeSizeOfShape(float i_widthOrRadius, float i_heightOrGarbage) override;
	float GetWidth();
	float GetHeight();
	void ResetOrigin();
};