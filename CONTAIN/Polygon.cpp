#include "Polygon.h"

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











//Polygon::Polygon()
//{
//}

//Poinst must always be in clockwise order
Polygon::Polygon(std::vector<Vector2f> i_pointArr) : 
	pointArr { i_pointArr }, numPoints { (int)i_pointArr.size() }
{
}

Polygon::~Polygon()
{
}

Vector2f Polygon::GetSFMLOriginOffset()
{
	//Im gonna say the center is always (0, 0) and see what happens,
	//calculating the center is apparently a bitch and a half and I dont think the physics engine guy did it.
	//Half my coordinates will be negative but Idk if that will cause any problems
	return Vector2f(0, 0);
}

std::unique_ptr<sf::Shape> Polygon::GetSFMLRepr()
{
	std::unique_ptr<sf::ConvexShape> polyPtr = std::make_unique<sf::ConvexShape>(numPoints);
	int size = pointArr.size();
	for (int i = 0; i < size; ++i) {
		sf::Vector2f vect = sf::Vector2f(pointArr[i][0], pointArr[i][1]);
		polyPtr->setPoint(i, vect);
	}
	std::unique_ptr<sf::Shape> shapePtr = move(polyPtr);
	return shapePtr;
}

Shape::ShapeType Polygon::GetType()
{
	return POLYGON;
}

float Polygon::GetDistToEdge()
{
	//used only in "CreateOrientationLine";
	return 50.0f;
}

std::vector<Vector2f> Polygon::GetPoints()
{//used in rb copy, shape resize, 
	return pointArr;
}

float Polygon::GetArea()
{
	Vector2f centroid = Vector2f(0.0f, 0.0f);
	float area = 0.0f;
	int size = pointArr.size();
	float momOfInertia = 0.0f;
	const float k_inv3 = 1.0f / 3.0f;
	for (int i = 0; i < size; ++i) {
			int i2 = i + 1 < size ? i + 1 : 0;
			Vector2f ptA = pointArr[i];
			Vector2f ptB = pointArr[i2];
			Vector2f vectFromCentertoA = Vector2f(0.0f, 0.0f) - ptA;
			float lengthA = vectFromCentertoA.norm();
			Vector2f vectFromCentertoB = Vector2f(0.0f, 0.0f) - ptB;
			float lengthB = vectFromCentertoB.norm();
			Vector2f vectFromCentertoC = ptA - ptB;
			float lengthC = vectFromCentertoC.norm();

			float perim = (lengthA + lengthB + lengthC) / 2.0f;
			float triangleArea = sqrt(perim*((perim - lengthA)*(perim - lengthB)*(perim - lengthC)));

			area += triangleArea;
			centroid += triangleArea * k_inv3 * (ptA + ptB);
			//real intx2 = p1.x * p1.x + p2.x * p1.x + p2.x * p2.x;
			//real inty2 = p1.y * p1.y + p2.y * p1.y + p2.y * p2.y;
			//I += (0.25f * k_inv3 * D) * (intx2 + inty2);
			//Idk what all this shit is but it calculates the moment of inertia
			float intX2 = ptA.x() * ptA.x() + ptB.x() * ptA.x() + ptB.x() * ptB.x();
			float inty2 = ptA.y() * ptA.y() + ptB.y() * ptA.y() + ptB.y() * ptB.y();
			momOfInertia += (0.25f * k_inv3 * Math::CrossProdScalar(ptA, ptB) * (intX2 + inty2));
	}
	return area;
}

float Polygon::GetInertiaCoeff()
{
	Vector2f centroid = Vector2f(0.0f, 0.0f);
	float area = 0.0f;
	int size = pointArr.size();
	float momOfInertia = 0.0f;
	const float k_inv3 = 1.0f / 3.0f;
	for (int i = 0; i < size; ++i) {
		int i2 = i + 1 < size ? i + 1 : 0;
		Vector2f ptA = pointArr[i];
		Vector2f ptB = pointArr[i2];
		Vector2f vectFromCentertoA = Vector2f(0.0f, 0.0f) - ptA;
		float lengthA = vectFromCentertoA.norm();
		Vector2f vectFromCentertoB = Vector2f(0.0f, 0.0f) - ptB;
		float lengthB = vectFromCentertoB.norm();
		Vector2f vectFromCentertoC = ptA - ptB;
		float lengthC = vectFromCentertoC.norm();

		float perim = (lengthA + lengthB + lengthC) / 2.0f;
		float triangleArea = sqrt(perim*((perim - lengthA)*(perim - lengthB)*(perim - lengthC)));

		area += triangleArea;
		centroid += triangleArea * k_inv3 * (ptA + ptB);
		//real intx2 = p1.x * p1.x + p2.x * p1.x + p2.x * p2.x;
		//real inty2 = p1.y * p1.y + p2.y * p1.y + p2.y * p2.y;
		//I += (0.25f * k_inv3 * D) * (intx2 + inty2);
		//Idk what all this shit is but it calculates the moment of inertia
		//I = bh3 / 36
		//float intX2 = ptA.x() * ptA.x() + ptB.x() * ptA.x() + ptB.x() * ptB.x();
		//float inty2 = ptA.y() * ptA.y() + ptB.y() * ptA.y() + ptB.y() * ptB.y();
		momOfInertia += abs(0.25f * k_inv3 * Math::CrossProdScalar(ptA, ptB));
	}
	//return 166;
	return momOfInertia;
}

void Polygon::ChangeSizeOfShape(float i_widthOrRadius, float i_heightOrGarbage)
{
	//not gonna worry about this just yet
}
