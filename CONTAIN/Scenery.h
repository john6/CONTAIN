#pragma once
#include "Entity.h"
#include <string>

class Scenery : public Entity {
private:
	drawablePtrVect drawables;

public:
	int renderLvl;
	float drawbWidth;
	float drawbHeight;
	std::shared_ptr <sf::Shape> firstDrawb;
	hiRes_time_point lastColorSwitch;
	float colorSwitchRate;

	Scenery(Vector2f i_startPosition, std::shared_ptr <sf::Shape> i_drawable, int i_renderLvl = 0,
		RigidBody i_rb = RigidBody(std::make_shared<Circle>(1.0f)), std::string i_texFileName = "",
		float stretchFactWidth = 1, float stretchFactHeight = 1);
	~Scenery();

	drawablePtrVect CreateDrawables(float i_lerp_fraction) override;

	virtual void Update(float i_stepSize) override;
};