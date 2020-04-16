#pragma once
#include "GLOBAL_CONSTANTS.h"

class Transform
{
private:


public:
	Transform(Vector2f i_position, Vector2f i_scale, float i_orientation);
	Transform();
	~Transform();

	Vector2f position;
	Vector2f scale;
	float orientation; //radians


	//void SetPosition(Vector2f i_position);
	//void SetRotation(float i_rotation);
	//void SetScale(Vector2f i_scale);
	//Vector2f GetPosition();
	//Vector2f GetScale();
	//float GetRotation();
};