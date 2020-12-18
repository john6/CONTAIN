#pragma once
#include "Entity.h"

class PowerUp :
	public Entity
{
public:
	UPGRADE_TYPE powType;
	sf::Color powColor;

	PowerUp(Vector2f i_startPosition, UPGRADE_TYPE i_powType, RigidBody i_rb =
		RigidBody(std::make_shared<Rectangle>(50.0f, 50.0f), STATIC));
	~PowerUp();

	void Update(float i_stepSize) override;

	virtual void UpdateVisuals(float i_stepSize) override;
};
