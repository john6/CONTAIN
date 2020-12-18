#include "Math.h"



Math::Math()
{
}


Math::~Math()
{
}


Vector2f Math::FloatVectCross(float f, Vector2f vect)
{
	return Vector2f(-f * vect(1), f * vect(0));
}

Vector2f Math::VectFloatCross(Vector2f vect, float f)
{
	return Vector2f(f * vect(1), -f * vect(0));
}

float Math::CrossProdScalar(Vector2f vectA, Vector2f vectB)
{
	return ((vectA(0) * vectB(1)) - (vectA(1) * vectB(0)));
}

float Math::VectToAngle(Vector2f i_dirVect)
{
	return 	atan2(i_dirVect[1], i_dirVect[0]);;
}

Vector2f Math::AngleToVect(float i_angle)
{
	return Vector2f(cos(i_angle), sin(i_angle));
}

//Expecting second coord to be greater than the first on both the x and y axis
Vector2f Math::GetRandomCoordInRect(Vector2f topLeft, Vector2f bottomRight)
{
	std::random_device rd;
	std::mt19937 gen1(rd());
	std::mt19937 gen2(rd());
	int leftX = (int)topLeft[0];
	int rightX = (int)bottomRight[0];
	int topY = (int)topLeft[1];
	int bottomY = (int)bottomRight[1];

	int randXcordInBounds;
	int randYcordInBounds; 

	if (leftX < rightX) {
		std::uniform_int_distribution<> xCordDist(leftX, rightX);
		randXcordInBounds = xCordDist(gen1);
	}
	else {
		randXcordInBounds = leftX;
	}

	if (topY < bottomY) {
		std::uniform_int_distribution<> yCordDist(topY, bottomY);
		randYcordInBounds = yCordDist(gen2);
	}
	else {
		randYcordInBounds = bottomY;
	}

	return Vector2f(randXcordInBounds, randYcordInBounds);
}
