#pragma once
#include "Enemy.h"

class BossRush : public Enemy
{
private:
	Vector2f currDir;
	bool rushing;
	float timeTillSwitch;
	float rushTime;
	float waitTime;
	float numShots;
	hiRes_time_point lastShotFired;
	float shipRateOfFire;
	float weaponDelay;
	bool isMiniBoss;

public:
	BossRush(DIFFICULTY i_diff, Vector2f i_startPosition, bool i_isMiniBoss = false,
		RigidBody i_rb = RigidBody(std::make_shared<Circle>(75), HEAVYBOUNCE));

	void Update(float i_stepSize) override;

	void Destroy() override;

	void TakeDamage(float i_dmg);

	void shootProj();

	void Stun(float i_stunTime) override;

	void SetDiffVars(int i_diff);

	void CollideWithPainWall(PainWall * i_painWallPtr) override;

	void CollideWithPlayer(PlayerChar* i_playerPtr) override;
};
