#include "Transform.h"



Transform::Transform(Vector2f i_position, Vector2f i_scale, float i_rotationAngle) :
	position {i_position}, rotationAngle { i_rotationAngle }, scale {i_scale}
{
}

Transform::Transform()
{
}

Transform::~Transform()
{
}

void Transform::SetPosition(Vector2f i_position) { position = i_position;  }
void Transform::SetScale(Vector2f i_scale) { scale = i_scale; }
void Transform::SetRotation(float i_rotation) { rotationAngle = i_rotation; }
Vector2f Transform::GetPosition() { return position;  }
Vector2f Transform::GetScale() { return scale;  }
float Transform::GetRotation() { return rotationAngle; }
