#include "MassData.h"


MassData::MassData(float i_mass, float i_inertia) 
{
	SetMass(i_mass);
	SetInertia(i_inertia);
}

MassData::MassData() :
mass{ 0.0f }, massInv{ 0.0f }, inertia{ 0.0f }, inertiaInv{ 0.0f }
{
}

MassData::~MassData()
{
}

void MassData::SetMass(float i_mass)
{//Infinite mass will be denoted as 0 mass
	mass = i_mass;
	massInv = (i_mass == 0.0f) ? 0.0f : 1.0f / i_mass;
}

void MassData::SetInertia(float i_inertia)
{//Infinite inertia will be denoted as 0
	inertia = i_inertia;
	inertiaInv = (i_inertia == 0.0f) ? 0.0f : 1.0f / i_inertia;
}

float MassData::GetMass() { return mass; }

float MassData::GetMassInv() { return massInv; }

float MassData::GetIntertia() { return inertia; }

float MassData::GetInertiaInv() { return inertiaInv; }
