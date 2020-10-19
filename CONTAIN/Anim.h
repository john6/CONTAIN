#pragma once
#include "Entity.h"

class Anim :
	public Entity
{
private:
	hiRes_time_point birthTime;
	microSec lifeTime;
	hiRes_time_point deathTime;

	std::vector<std::shared_ptr<Polygon>> polys;
	ANIMTYPE aType;

	//DUMB ASS SHIT need to generalize
	float origRadius;
	float origWidth;
	float origHeight;
	sf::Vector2f origPosition;

public:
	Anim(Vector2f i_startPosition, microSec i_lifetime,
		ANIMTYPE i_aType = CANNED_EXPLOSION, Entity* i_entPtr = NULL);
	~Anim();

	void Update(float i_stepSize) override;

	void Destroy() override;

	drawablePtrVect CreateDrawables(float i_lerp_fraction) override;
};
