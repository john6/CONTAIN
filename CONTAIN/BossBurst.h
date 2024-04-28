#pragma once
#include "Enemy.h"

class BossBurst : public Enemy
{
private:
	Vector2f currDir;
	float sameDirTime;
	float timeTillDirSwitch;
	float numShots;
	hiRes_time_point lastShotFired;
	float shipRateOfFire;
	float weaponDelay;
	float projSpeed;
	bool isMiniBoss;

public:


	BossBurst(DIFFICULTY i_diff, Vector2f i_startPosition, bool i_isMiniBoss = false, int lvl_num = 0,
		RigidBody i_rb = RigidBody(std::make_shared<Circle>(70), METAL));

	void Update(float i_stepSize) override;

	void Destroy() override;

	void TakeDamage(float i_dmg);

	void shootProj();

	void Stun(float i_stunTime) override;

	void SetDiffVars(int i_diff, int lvl_num);
};


