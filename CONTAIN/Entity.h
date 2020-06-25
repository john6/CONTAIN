#pragma once
#include "RigidBody.h"

class Entity
{
//enum EntityGroup { PlayerChar, Enemy, Projectile, Wall, Door };

private:

protected:
	sf::Color fillColor;
	sf::Color outlineColor;
	bool killMeNextLoop;

public:
	Entity();
	Entity(RigidBody i_rb);
	~Entity();

	RigidBody rb;

	const bool MarkedForDeath();

	virtual void CollideWith(Entity& i_other) = 0;

	virtual void Destroy() = 0;

	std::unique_ptr<sf::Shape> CreateDrawable(float i_lerp_fraction);
};

class PlayerChar :
	public Entity
{
private:
	int health;


public:
	PlayerChar(RigidBody i_rb);
	~PlayerChar();

	void CollideWith(Entity& i_other) override;

	void Destroy() override;
};

class Projectile :
	public Entity
{
public:
	Projectile(RigidBody i_rb);
	~Projectile();

	void CollideWith(Entity& i_ent) override;

	void Destroy() override;
};

class Enemy :
	public Entity
{
public:
	Enemy(RigidBody i_rb);
	~Enemy();

	void CollideWith(Entity& i_other) override;

	void Destroy() override;
};

class Wall :
	public Entity
{
public:
	Wall(RigidBody i_rb);
	~Wall();

	void CollideWith(Entity& i_other) override;

	void Destroy() override;
};

class Door :
	public Entity
{
public:
	Door(RigidBody i_rb);
	~Door();

	void CollideWith(Entity& i_other) override ;

	void Destroy() override;
};