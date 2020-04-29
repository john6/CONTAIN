#include "Entity.h"



Entity::Entity() :
	rb { RigidBody(std::make_shared<Rectangle>(100.0f, 100.0f)) }
{
	Material defMaterial(0.6f, 0.1f, 0.6f, 0.3f);
	rb.mat = defMaterial;
	rb.SetMassData();
	color = sf::Color::Red;
}

Entity::Entity(RigidBody i_rb) :
	rb { i_rb }
{
	/*Material defMaterial(0.6f, 0.1f, 0.6f, 0.3f);
	rb.mat = defMaterial;
	rb.SetMassData();*/
	color = sf::Color::Red;
}


Entity::~Entity()
{
}

