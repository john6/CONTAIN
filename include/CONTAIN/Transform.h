#pragma once
#include "GLOBAL_CONSTANTS.h"

class Transform
{
private:


public:
	Transform(Vector2f i_position, Vector2f i_scale, float i_orientation);
	Transform();
	~Transform();

	Vector2f pos;     //current position
	Vector2f prevPos; //previous position
	Vector2f scale;   //scale of body
	float orient;     // orientation in radians
	float prevOrient; // previouos orientation
};