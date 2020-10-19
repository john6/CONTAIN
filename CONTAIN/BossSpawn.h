#pragma once
#include "Enemy.h"

class BossSpawn : public Enemy
{
private:
	DIFFICULTY diff;
	Vector2f currDir;
	float sameDirTime;
	float timeTillDirSwitch;
	float numShots;
	hiRes_time_point lastShotFired;
	float shipRateOfFire;
	float weaponDelay;
	bool isMiniBoss;

public:
	bool invulnerable;

	BossSpawn(DIFFICULTY i_diff, Vector2f i_startPosition, bool i_isMiniBoss = false,
		RigidBody i_rb = RigidBody(std::make_shared<Rectangle>(100.0f * GLBVRS::SIZE_RAT, 100.0f * GLBVRS::SIZE_RAT), STATIC));

	void Update(float i_stepSize) override;

	void Destroy() override;

	void TakeDamage(float i_dmg);

	void shootProj();

	void Stun(float i_stunTime) override;

	void SetDiffVars(int i_diff);
};
