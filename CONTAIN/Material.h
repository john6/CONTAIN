#pragma once

class Material
{
public:
	float dens;       //density
	float rest;       //restitution (bounciness)
	float statFrict;  //static friction
	float dynaFrict;  //dynamic friction


	Material();
	Material(float i_dens, float i_rest, float i_statFrict, float i_dynaFrict);
	~Material();
};

