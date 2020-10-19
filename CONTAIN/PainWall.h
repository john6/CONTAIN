#pragma once
#include "Entity.h"

class PainWall :
	public Entity
{
private:
	bool colorState;
	sf::Color colorA;
	sf::Color colorB;
	hiRes_time_point lastColorSwitch;
	float colorSwitchRate;

public:

	void Update(float i_stepSize) override;

	PainWall(Vector2f i_startPosition, RigidBody i_rb);
	~PainWall();
};
