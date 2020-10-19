#pragma once
#include "Entity.h"

class Door :
	public Entity
{
private:
	Vector2f outPos;
	MapCoord outCoord;



	sf::Color colorOpen;
	sf::Color colorClosed;
	hiRes_time_point disableTimeStamp;
	float disabledAmount;

	sf::Color colorOpenOutline;
	sf::Color colorClosedOutline;

public:
	bool locked;
	bool disabled;
	SCREEN_SIDE side;

	Door(MapCoord i_outCoord, Vector2f i_startPos, Vector2f i_outPos, RigidBody i_rb, SCREEN_SIDE i_side);
	~Door();

	const MapCoord GetOutCoord();

	const Vector2f GetOutPos();

	void Update(float i_stepSize) override;

	void Disable(float i_disableTime);

	void Unlock();

	void Open();

	void Close();
};
