#pragma once
#include "RigidBody.h"


class Entity
{
enum EntityGroup { PlayerChar, Enemy, Projectile, Wall, Door };

private:
	sf::Color color;
	EntityGroup entType;

public:
	Entity();
	Entity(RigidBody i_rb);
	~Entity();

	RigidBody rb;
	//RigidBody* rb;
};