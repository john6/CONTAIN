#include "Blocker.h"

Blocker::Blocker(Vector2f i_startPosition, RigidBody i_rb) :
	Entity(i_startPosition, i_rb, PROJ_WALL)
{
	fillColor = INDIGO;
	outlineColor = METALLICSILVER;
}

Blocker::~Blocker()
{
}