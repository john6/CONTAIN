#pragma once
#include "GLOBAL_CONSTANTS.h"

class Transform
{
private:
	Vector2f position;
	Vector2f scale;
	float rotationAngle; //not gonna use any more DEPRECIATED LUL

public:
	Transform(Vector2f i_position, Vector2f i_scale, float i_rotationAngle);
	Transform();
	~Transform();

	void SetPosition(Vector2f i_position);
	void SetRotation(float i_rotation);
	void SetScale(Vector2f i_scale);
	Vector2f GetPosition();
	Vector2f GetScale();
	float GetRotation();
};