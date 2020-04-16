#include "Transform.h"


Transform::Transform(Vector2f i_position, Vector2f i_scale, float i_orientation) :
	position {i_position}, scale{ i_scale }, orientation{ i_orientation }
{
}

Transform::Transform() :
	position { Vector2f(0.0f, 0.0f) }, scale { Vector2f(1.0f, 1.0f) },
	orientation{ 0.0f }
{
}

Transform::~Transform()
{
}

//void Transform::SetPosition(Vector2f i_position) { position = i_position;  }
//void Transform::SetScale(Vector2f i_scale) { scale = i_scale; }
//void Transform::SetRotation(float i_rotation) { rotationAngle = i_rotation; }
//Vector2f Transform::GetPosition() { return position;  }
//Vector2f Transform::GetScale() { return scale;  }
//float Transform::GetRotation() { return rotationAngle; }
