#pragma once
#include "Entity.h"

class PowerUp :
	public Entity
{
public:
	UPGRADE_TYPE powType;

	PowerUp(Vector2f i_startPosition, UPGRADE_TYPE i_powType, RigidBody i_rb =
		RigidBody(std::make_shared<Rectangle>(100.0f * GLBVRS::SIZE_RAT, 100.0f * GLBVRS::SIZE_RAT), STATIC));
	~PowerUp();

	void Update(float i_stepSize) override;
};
