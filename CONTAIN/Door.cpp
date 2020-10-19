#include "Door.h"


Door::Door(MapCoord i_outCoord, Vector2f i_startPos, Vector2f i_outPos, RigidBody i_rb, SCREEN_SIDE i_side) :
	Entity(i_startPos, i_rb, DOOR), outCoord{ i_outCoord }, outPos{ i_outPos }, side{ i_side }
{
	//state
	intangible = true;
	locked = false;
	disabled = false;
	disableTimeStamp = hiResTime::now();
	disabledAmount = 0.0f;
	//color
	colorOpen = CYAN;
	colorClosed = DARKSILVER;
	colorOpenOutline = CYAN;
	colorClosedOutline = SILVER;
	fillColor = colorClosed;
	outlineColor = colorClosedOutline;

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
	locked = true;
}

void Door::Open()
{
	fillColor = colorOpen;
	outlineColor = colorOpenOutline;
}

void Door::Close()
{
	fillColor = colorClosed;
	outlineColor = colorClosedOutline;
}

