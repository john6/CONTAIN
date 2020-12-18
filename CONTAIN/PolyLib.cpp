#include "PolyLib.h"


PolyLib::PolyLib()
{
	vertArray = std::vector<std::vector<Vector2f>>();
	vertArrayTriangles = std::vector<std::vector<Vector2f>>();

//irreg poly 1
std::vector<Vector2f> irregPoly1 = { Vector2f(0.0f, 0.0f), Vector2f(0.0f, 68.0f), Vector2f(68.0f, 45.0f), Vector2f(97.0f, 0.0f), Vector2f(68.0f, -45.0f) };
vertArray.emplace_back(irregPoly1);
// irreg poly 2 square 
std::vector<Vector2f> polySquare = {Vector2f(0.0f, 0.0f), Vector2f(105.0f, 30.0f), Vector2f(75.0f, -45.0f), Vector2f(15.0f, -45.0f)};
vertArray.emplace_back(polySquare);
//irreg poly 3, six sides
std::vector<Vector2f> polysix = { Vector2f(0.0f, 0.0f), Vector2f(60.0f, 90.0f), Vector2f(90.0f, 90.0f), Vector2f(120.0f, 30.0f), Vector2f(105.0f, 0.0f), Vector2f(60.0f, -15.0f) };
vertArray.emplace_back(polySquare);


//triangle, seems kinda odd I think Ill stick to polys
std::vector<Vector2f> polyTriangle = { Vector2f(0.0f, 0.0f), Vector2f(30.0f, 90.0f), Vector2f(120.0f, 0.0f) }; //triangle
vertArrayTriangles.emplace_back(polyTriangle);


std::vector<Vector2f> polyDiamond = { Vector2f(0.0f, 0.0f), Vector2f(-20.0f, 20.0f), Vector2f(-20.0f, 60.0f), Vector2f(0.0f, 80.0f), Vector2f(20.0f, 60.0f), Vector2f(20.0f, 20.0f) }; //poly diamond
}


PolyLib::~PolyLib()
{
}

std::vector<Vector2f> PolyLib::GetRandomVertArray()
{
	std::random_device rd1;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen1(rd1()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> distrib(0, vertArray.size() -1); //both boundaries are inclusive
	int randExtra = distrib(gen1);
	return vertArray[randExtra];
}

std::vector<Vector2f> PolyLib::GetRandomTriangleVertArray()
{
	std::random_device rd1;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen1(rd1()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> distrib(0, vertArrayTriangles.size() - 1); //both boundaries are inclusive
	int randExtra = distrib(gen1);
	return vertArrayTriangles[randExtra];
}

std::vector<Vector2f> PolyLib::GetRegularTriangleVertArray()
{
	return { Vector2f(0.0f, 0.0f), Vector2f(20.0f, 30.0f), Vector2f(40.0f, 0.0f) };
}

std::vector<Vector2f> PolyLib::GetIsosoleseTriangleVertArray()
{
	return { Vector2f(0.0f, 0.0f), Vector2f(30.0f, 15.0f), Vector2f(30.0f, -15.0f) };
}
