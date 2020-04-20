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