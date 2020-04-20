#pragma once
#include "GLOBAL_CONSTANTS.h"

class Math
{
public:
	Math();
	~Math();


	static Vector2f FloatVectCross(float f, Vector2f vect);

	static Vector2f VectFloatCross(Vector2f vect, float f);

	static float CrossProdScalar(Vector2f vectA, Vector2f vectB);

};

