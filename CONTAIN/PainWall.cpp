#include "PainWall.h"


PainWall::PainWall(Vector2f i_startPosition, RigidBody i_rb) :
	Entity(i_startPosition, i_rb, WALL_FIRE)
{
	intangible = true;
	fixedPosition = true;
	colorSwitchRate = 3;
	lastColorSwitch = hiResTime::now();
	colorState = true;
	fillColor = VIVIDYELLOW;
	outlineColor = sf::Color::Black;
	colorA = MYSTICRED;
	colorB = VIVIDYELLOW;
}

PainWall::~PainWall()
{
}


void PainWall::Update(float i_stepSize)
{
	float colorSwitchCurr = (std::chrono::duration_cast<std::chrono::microseconds>(hiResTime::now() - lastColorSwitch)).count() / 1000000.0f;
	if (colorSwitchCurr >= colorSwitchRate) {
		lastColorSwitch = hiResTime::now();
	}
	float colorpercent = (colorSwitchCurr / colorSwitchRate) * 2.0f * PI;
	float clrPhse = cos(colorpercent);
	float clrPhseInv = 1.0f - clrPhse;
	float r = std::min(255.0f, std::max(0.0f, (colorA.r * clrPhse) + (colorB.r * clrPhseInv)));
	float g = std::min(255.0f, std::max(0.0f, (colorA.g * clrPhse) + (colorB.g * clrPhseInv)));
	float b = std::min(255.0f, std::max(0.0f, (colorA.b * clrPhse) + (colorB.b * clrPhseInv)));
	fillColor = sf::Color(r, g, b);
	float secsInUpdate = i_stepSize / 1000.0f;
}
