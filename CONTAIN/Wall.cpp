#include "Wall.h"
#include "Entity.h"

Wall::Wall(Vector2f i_startPosition, RigidBody i_rb,
	sf::Color i_fillCol, sf::Color i_outCol, bool i_break) :
	Entity(i_startPosition, i_rb, WALL_BASIC), breakable{ i_break }
{
	health = 20;
	fixedPosition = true;
	fillColor = i_fillCol;
	outlineColor = i_outCol;
}

void Wall::CollideWithProjectile(Projectile * i_projPtr)
{
	if (breakable) {
		TakeDamage(1.0f);
	}
}

void Wall::TakeDamage(float i_dmg)
{
	health -= i_dmg;
	if (health <= 0.0f) {
		rb.mat = DENSE_METAL;
		rb.SetMassData();
	}
}


Wall::~Wall()
{
}

