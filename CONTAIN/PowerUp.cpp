#include "PowerUp.h"
#include "Entity.h"


PowerUp::PowerUp(Vector2f i_startPosition, UPGRADE_TYPE i_powType, RigidBody i_rb) :
	Entity(i_startPosition, i_rb, UPGRADE), powType{ i_powType }
{
	intangible = true;

	switch (i_powType) {
	case (RATE_OF_FIRE): {
		fillColor = sf::Color(255, 128, 128);
		outlineColor = METALLICSILVER;
		break;
	}
	case (SCATTER): {
		fillColor = sf::Color(51, 153, 102);
		outlineColor = METALLICSILVER;
		break;
	}
	case (TEMP_HEALTH): {
		fillColor = RED;
		outlineColor = METALLICSILVER;
		break;
	}
						//TODO
	}
}

PowerUp::~PowerUp()
{
	fillColor = sf::Color::Magenta;
	outlineColor = sf::Color::Magenta;
}

void PowerUp::Update(float i_stepSize)
{
	rb.angVel = 0.03f;
}
