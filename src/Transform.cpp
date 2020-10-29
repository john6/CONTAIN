#include <CONTAIN/Transform.h>


Transform::Transform(Vector2f i_pos, Vector2f i_scale, float i_orient) :
	pos{ i_pos }, prevPos{ i_pos }, scale{ i_scale }, orient{ i_orient }
{
}

Transform::Transform() :
	pos { Vector2f(0.0f, 0.0f) }, prevPos{ Vector2f(0.0f, 0.0f) }, scale { Vector2f(1.0f, 1.0f) },
	orient { 0.0f }
{
}

Transform::~Transform()
{
}
