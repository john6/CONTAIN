#pragma once
#include "Shape.h"
#include <math.h>
#include "Math.h"


//
//
//// create an empty shape
//sf::ConvexShape convex;
//
//// resize it to 5 points
//convex.setPointCount(5);
//
//// define the points
//convex.setPoint(0, sf::Vector2f(0, 0));
//convex.setPoint(1, sf::Vector2f(150, 10));
//convex.setPoint(2, sf::Vector2f(120, 90));
//convex.setPoint(3, sf::Vector2f(30, 100));
//convex.setPoint(4, sf::Vector2f(0, 50));
//
//
//










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
};