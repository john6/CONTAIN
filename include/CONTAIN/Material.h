//Material Name = Material(density, restitution, static friction, dynamic friction);
//Material Rock = Material(0.6f, 0.1f, 0.6f, 0.3f);
//Material Wood = Material(0.3f, 0.2f, 0.5f, 0.25f);
//Material Metal = Material(1.2f, 0.05f, 0.4f, 0.2f);
//Material BouncyBall = Material(0.3f, 0.8f, 0.7f, 0.2f);
//Material SuperBall = Material(0.3f, 0.95f, 0.5f, 0.25f);
//Material Pillow = Material(0.1f, 0.2f, 0.4f, 0.2f);
//Material Static = Material(0.0f, 0.4f, 0.4f, 0.2f);
//Material Rubber = Material(0.3f, 0.8f, 0.9f, 0.6f);
//Material Ice = Material(0.3f, 0.1f, 0.7f, 0.05f);
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

