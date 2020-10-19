#pragma once
#include "Entity.h"

class Wall :
	public Entity
{
public:
	bool breakable;
	float health;

	void CollideWithProjectile(Projectile* i_projPtr) override;

	void TakeDamage(float i_dmg);

	Wall(Vector2f i_startPosition, RigidBody i_rb,
		sf::Color i_fillCol = sf::Color::Black, sf::Color i_outCol = sf::Color::White,
		bool i_break = false);
	~Wall();
};
