#include "Door.h"


Door::Door(MapCoord i_outCoord, Vector2f i_startPos, Vector2f i_outPos, RigidBody i_rb, SCREEN_SIDE i_side) :
	Entity(i_startPos, i_rb, DOOR), outCoord{ i_outCoord }, outPos{ i_outPos }, side{ i_side }
{
	//state
	intangible = true;
	locked = false;
	disabled = false;
	open = false;
	disableTimeStamp = hiResTime::now();
	disabledAmount = 0.0f;
	//color
	openColor = CYAN;
	openColorOutline = DIMGRAY;

	closedColor = DARKSILVER;
	closedColorOutline = DIMGRAY;

	fillColor = closedColor;
	outlineColor = closedColorOutline;

}

Door::~Door()
{
}

const MapCoord Door::GetOutCoord()
{
	return outCoord;
}

const Vector2f Door::GetOutPos()
{
	return outPos;
}

void Door::Update(float i_stepSize)
{
	float timeSinceDisable = (std::chrono::duration_cast<std::chrono::microseconds>(hiResTime::now() - disableTimeStamp)).count() / 1000000.0f;

	if ((locked) || (timeSinceDisable <= disabledAmount)) {
		Close();
	}
	else {
		Open();
	}
}

void Door::Disable(float i_disableTime)
{
	disabled = true;
	disableTimeStamp = hiResTime::now();
	disabledAmount = i_disableTime;
}

void Door::Unlock() {
	locked = false;
}

void Door::Lock()
{
	locked = true;
}

void Door::Open()
{
	if (!open) {
		open = true;
		fillColor = openColor;
		outlineColor = openColorOutline;
	}
}

const bool Door::IsOpen()
{
	return open;
}

void Door::Close()
{
	if (open) {
		open = false;
		fillColor = closedColor;
		outlineColor = closedColorOutline;
	}
}

