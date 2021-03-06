#pragma once
#include "Entity.h"

class Wall :
	public Entity
{
public:
	bool breakable;
	bool exploding;
	hiRes_time_point lastExplosion;
	float explosionRate;
	float maxHealth;
	float currHealth;

	sf::Color origColorFill;
	sf::Color origColorOutLine;
	sf::Color deathColorFill;
	sf::Color deathColorOutLine;

	void CollideWithProjectile(CollisionData i_coll) override;

	void TakeDamage(float i_dmg);

	void ChangeColorHealth();

	void GenerateDeathEffects();

	void Update(float i_stepSize) override;

	Wall(Vector2f i_startPosition, RigidBody i_rb,
		sf::Color i_fillCol = sf::Color::Black, sf::Color i_outCol = sf::Color::White,
		bool i_break = false);
	~Wall();
};
