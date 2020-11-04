#pragma once
#include "Entity.h"

class EndObject :
	public Entity
{
public:
	bool active;

	EndObject(Vector2f i_startPosition, RigidBody i_rb =
		RigidBody(std::make_shared<Rectangle>(150.0f, 150.0f), STATIC));
	~EndObject();


	void Update(float i_stepSize) override;

	void Lock(); 

	void Unlock();
};
