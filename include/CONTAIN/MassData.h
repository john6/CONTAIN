#pragma once
class MassData
{
private:
	float mass;
	float massInv;
	float inertia;
	float inertiaInv;

public:
	MassData(float i_mass, float i_inertia);
	MassData();
	~MassData();

	void SetMass(float i_mass);
	void SetInertia(float i_inertia);
	float GetMass();
	float GetMassInv();
	float GetIntert();
	float GetInertInv();
};

