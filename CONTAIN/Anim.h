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

	Vector2f collisionNormal;
	std::vector<float> rotations;
	float spread;
	std::vector<std::shared_ptr<RigidBody>> rbs;

public:
	Anim(Vector2f direction, std::vector<Vector2f> i_contactPoints, microSec i_lifetime, int i_number, int i_splashOption);

	Anim(Vector2f i_startPosition, microSec i_lifetime, ANIMTYPE i_aType = CANNED_EXPLOSION, Entity* i_entPtr = NULL);


	~Anim();

	void Update(float i_stepSize) override;

	void UpdateCanned(float i_stepSize);

	void UpdateBurstDeath(float i_stepSize);

	void UpdateSparks(float i_stepSize);

	void Destroy() override;

	drawablePtrVect CreateDrawables(float i_lerp_fraction) override;
};
