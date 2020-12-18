#pragma once
//#include "Polygon.h"
#include "GLOBAL_CONSTANTS.h"
#include <random>

class PolyLib
{
private:
	std::vector<std::vector<Vector2f>> vertArray;
	std::vector<std::vector<Vector2f>> vertArrayTriangles;

public:
	PolyLib();
	~PolyLib();

	std::vector<Vector2f> GetRandomVertArray();

	std::vector<Vector2f> GetRandomTriangleVertArray();

	std::vector<Vector2f> GetRegularTriangleVertArray();


	std::vector<Vector2f> GetIsosoleseTriangleVertArray();
};

