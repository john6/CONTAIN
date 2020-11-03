#include <CONTAIN/Polygon.h>


void Polygon::CalculateValues()
{//recalculates shapes area, inertia coefficient, and recenters it around (0,0)
	Vector2f centroid = Vector2f(0.0f, 0.0f);
	float newArea = 0.0f;
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

		newArea += triangleArea;
		centroid += triangleArea * k_inv3 * (ptA + ptB);
		momOfInertia += abs(0.25f * k_inv3 * Math::CrossProdScalar(ptA, ptB));
	}
	centroid *= (1.0f / newArea);
	for (int i = 0; i < numPoints; i++) {
		pointArr[i] -= centroid;
	}
	area = newArea;
	inertiaCoeff = momOfInertia;
}

//Poinst must always be in clockwise order
Polygon::Polygon(std::vector<Vector2f> i_pointArr) : 
	pointArr { i_pointArr }, numPoints { (int)i_pointArr.size() }
{
	currSizeRat = 1;
	CalculateValues();
}

Polygon::~Polygon()
{
}

Vector2f Polygon::GetSFMLOriginOffset()
{
	return Vector2f(0, 0);
}

std::shared_ptr<sf::Shape> Polygon::GetSFMLRepr()
{
	std::shared_ptr<sf::ConvexShape> polyPtr = std::make_shared<sf::ConvexShape>(numPoints);
	int size = pointArr.size();
	for (int i = 0; i < size; ++i) {
		sf::Vector2f vect = sf::Vector2f(pointArr[i][0], pointArr[i][1]);
		polyPtr->setPoint(i, vect);
	}
	std::shared_ptr<sf::Shape> shapePtr = move(polyPtr);
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
			float intX2 = ptA.x() * ptA.x() + ptB.x() * ptA.x() + ptB.x() * ptB.x();
			float inty2 = ptA.y() * ptA.y() + ptB.y() * ptA.y() + ptB.y() * ptB.y();
			momOfInertia += (0.25f * k_inv3 * Math::CrossProdScalar(ptA, ptB) * (intX2 + inty2));
	}
	return area;
}

float Polygon::GetInertiaCoeff()
{
	return inertiaCoeff;
}

void Polygon::ChangeSizeOfShape(float i_widthOrRadius, float i_heightOrGarbage)
{//This will result in the shape becoming a regular polygon if its not already
//Gonna turn this off unless I need to use it I think it'll do more harm than good
 //for (int i = 0; i < numPoints; i++) {
	//	pointArr[i] *= i_widthOrRadius;
	//}
	//currSizeRat = i_widthOrRadius;
	//CalculateValues();
}

void Polygon::ResizeMutliple(float i_sizeFactor)
{//So long as the polygon is centered at zero, this should maintain the shape while altering the magnitude of each points vector
	for (int i = 0; i < numPoints; i++) {
		pointArr[i] *= i_sizeFactor;
	}
	currSizeRat *= i_sizeFactor;
	CalculateValues();
}

void Polygon::ResetSize(float i_sizeFactor)
{
	float ratioChange = (i_sizeFactor / currSizeRat);
	for (int i = 0; i < numPoints; i++) {
		pointArr[i] *= ratioChange;
	}
	currSizeRat = i_sizeFactor;
	CalculateValues();
}

float Polygon::GetDistToCorner()
{
	return pointArr[0].norm();
}
