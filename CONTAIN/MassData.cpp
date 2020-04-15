#include "MassData.h"


MassData::MassData(float i_mass, float i_inertia) :
	mass {i_mass}, inertia {i_inertia}
{
}

MassData::MassData()
{
}

MassData::~MassData()
{
}

void MassData::SetMass(float i_mass)
{//Infinite mass will be denoted as 0 mass
	mass = i_mass;
	massInv = (i_mass == 0) ? 0 : 1.0f / i_mass;
}
void MassData::SetInertia(float i_inertia)
{//Infinite inertia will be denoted as 0
	inertia = i_inertia;
	inertiaInv = (i_inertia == 0) ? 0 : 1.0f / i_inertia;
}

float MassData::GetMass() { return mass; }

float MassData::GetMassInv() { return massInv; }

float MassData::GetIntertia() { return inertia; }

float MassData::GetInertiaInv() { return inertiaInv; }
