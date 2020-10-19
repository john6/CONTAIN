#pragma once
#include "Entity.h"

class EndObject :
	public Entity
{
public:
	bool active;

	EndObject(Vector2f i_startPosition, RigidBody i_rb =
		RigidBody(std::make_shared<Rectangle>(150.0f * GLBVRS::SIZE_RAT, 150.0f * GLBVRS::SIZE_RAT), STATIC));
	~EndObject();


	void Update(float i_stepSize) override;

	void Lock(); 

	void Unlock();
};
