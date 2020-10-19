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

	std::list<std::weak_ptr<Entity>> children;
public:
	bool invulnerable;

	BossSpawn(DIFFICULTY i_diff, Vector2f i_startPosition, bool i_isMiniBoss = false,
		RigidBody i_rb = RigidBody(std::make_shared<Rectangle>(100.0f * GLBVRS::SIZE_RAT, 100.0f * GLBVRS::SIZE_RAT), STATIC));

	void Update(float i_stepSize) override;

	void Destroy() override;

	//TODO vulnerable can be made into private too I think I would just put the code into take damage where it just doesnt if its not vulnerable
	void TakeDamage(float i_dmg);

	void shootProj();

	void Stun(float i_stunTime) override;

	void SetDiffVars(int i_diff);

	void SpawnEnemies(int i_numEnems, TypeID enemyType, int i_sizeMod);

	void CheckChildren();

};
