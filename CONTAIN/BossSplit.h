#pragma once
#include "Enemy.h"

class BossSplit : public Enemy
{
private:
	bool crazy;
	int splitsLeft;
	DIFFICULTY diff;
	Vector2f currDir;
	float sameDirTime;
	float timeTillDirSwitch;
	float numShots;
	hiRes_time_point lastShotFired;
	float shipRateOfFire;
	float weaponDelay;

public:
	BossSplit(DIFFICULTY i_diff, int i_splitsLeft, float i_spdFct, bool i_crazy,
		Vector2f i_startPosition, RigidBody i_rb = RigidBody(std::make_shared<Circle>(150.0f * GLBVRS::SIZE_RAT), BOUNCYBALL));

	void Update(float i_stepSize) override;

	void Destroy() override;

	void TakeDamage(float i_dmg);

	void shootProj();

	void Stun(float i_stunTime) override;

	void SetDiffVars(int i_diff);
};
