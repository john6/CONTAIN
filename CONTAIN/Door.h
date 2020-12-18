#pragma once
#include "Entity.h"

class Door :
	public Entity
{
private:
	Vector2f outPos;
	MapCoord outCoord;

	sf::Color openColor;
	sf::Color openColorOutline;

	sf::Color closedColor;
	sf::Color closedColorOutline;





	hiRes_time_point disableTimeStamp;
	float disabledAmount;

	bool open;
	bool locked;
	bool disabled;

public:
	SCREEN_SIDE side;

	Door(MapCoord i_outCoord, Vector2f i_startPos, Vector2f i_outPos, RigidBody i_rb, SCREEN_SIDE i_side);
	~Door();

	const MapCoord GetOutCoord();

	const Vector2f GetOutPos();

	void Update(float i_stepSize) override;

	void Disable(float i_disableTime);

	void Unlock();

	void Lock();

	void Open();

	const bool IsOpen();

	void Close();
};
