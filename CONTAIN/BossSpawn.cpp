#include "BossSpawn.h"


BossSpawn::BossSpawn(DIFFICULTY i_diff, Vector2f i_startPosition, bool i_isMiniBoss, RigidBody i_rb) :
	Enemy(i_diff, i_startPosition, i_rb, ENEMY_BOSS), diff{ i_diff }, isMiniBoss{ i_isMiniBoss }
{
	origColorFill = EMERALD;
	origColorOutLine = SEAFOAM;
	fillColor = origColorFill;
	outlineColor = origColorOutLine;
	if (isMiniBoss) {
		rb = RigidBody(std::make_shared<Rectangle>(100.0f * GLBVRS::SIZE_RAT, 100.0f * GLBVRS::SIZE_RAT), STATIC);
		rb.transform.pos = i_startPosition;
	}
	invulnerable = true;
	Stun(0.5f);
	lastShotFired = hiResTime::now();
	shipRateOfFire = 1.0f;
	currDir = CreateRandomDir();
	SetDiffVars(i_diff);
}

void BossSpawn::Update(float i_stepSize)
{
	//if (sectPtr->sectEnemyNum < 2) {
	//	invulnerable = false;
	//	//fillColor = EMERALD;
	//	//outlineColor = PISTACHIO;
	//	ChangeColorHealth();
	//}
	//else {
	//	invulnerable = true;
	//	fillColor = SILVER;
	//	outlineColor = DIMGRAY;
	//}
	//UpdateHealth(i_stepSize);
	//if (stunSecs < 0) {
	//	if (timeTillDirSwitch < 0) {
	//		if (sectPtr->sectEnemyNum < 2) {
	//			timeTillDirSwitch = sameDirTime;
	//			sectPtr->PlaySound(RESOURCES::BRAKE);
	//			sectPtr->GenerateEnemies(numShots, ENEMY_RAND, CENTER, 1, diff, 0);
	//			sectPtr->GenerateEnemies(numShots, ENEMY_SEEK, CENTER, 1, diff, 0);
	//		}
	//		else {
	//			timeTillDirSwitch = sameDirTime;
	//			sectPtr->PlaySound(RESOURCES::BRAKE);
	//			sectPtr->GenerateEnemies(std::max((int)numShots - 2, 1), ENEMY_RAND, CENTER, 1, diff, 0);
	//			sectPtr->GenerateEnemies(std::max((int)numShots - 2, 1), ENEMY_SEEK, CENTER, 1, diff, 0);
	//		}
	//	}
	//	else {
	//		float secsInUpdate = i_stepSize / 1000.0f;
	//		timeTillDirSwitch -= secsInUpdate;
	//	}
	//}
	//else {
	//	float secsInUpdate = i_stepSize / 1000.0f;
	//	stunSecs -= secsInUpdate;
	//}
}

void BossSpawn::Destroy()
{
	GLBVRS::RSRCS->PlaySound(RESOURCES::BOSSEXPLODE);
	//sectPtr->GenerateDeathEffects(this, ENEMY_BURST_DEATH);
	killMeNextLoop = true;
	if (isMiniBoss) {
		DropPowerUp();
	}
}

void BossSpawn::TakeDamage(float i_dmg)
{
	if (!invulnerable) {
		int xasd = 5;
		GLBVRS::RSRCS->PlaySound(RESOURCES::OUCH3);
		health -= i_dmg;
	}
	int x = 5;
	int xasd = 5;
	int xa = 5;
	int xq = 5;
	if (invulnerable) {
		int x = 5;
		GLBVRS::RSRCS->PlaySound(RESOURCES::SWORDCLASH);
	}
}

void BossSpawn::shootProj()
{
}

void BossSpawn::Stun(float i_stunTime)
{
	stunSecs = i_stunTime / 2.0f;
}

void BossSpawn::SetDiffVars(int i_diff)
{
	if (isMiniBoss) {
		switch (i_diff) {
		case EASY: {
			numShots = 3;
			maxHealth = 4;
			sameDirTime = 16.0;
			timeTillDirSwitch = 0.0f;
			health = maxHealth;
			break;
		}
		case MEDIUM: {
			numShots = 4;
			maxHealth = 5;
			sameDirTime = 12.0;
			timeTillDirSwitch = 0.0f;
			health = maxHealth;
			break;
		}
		case HARD: {
			numShots = 5;
			maxHealth = 6;
			sameDirTime = 10.0;
			timeTillDirSwitch = 0.0f;
			health = maxHealth;
			break;
		}
		}
	}
	else {
		switch (i_diff) {
		case EASY: {
			numShots = 4;
			maxHealth = 28;
			sameDirTime = 14.0;
			timeTillDirSwitch = 0.0f;
			health = maxHealth;
			break;
		}
		case MEDIUM: {
			numShots = 5;
			maxHealth = 22;
			sameDirTime = 10.0;
			timeTillDirSwitch = 0.0f;
			health = maxHealth;
			break;
		}
		case HARD: {
			numShots = 6;
			maxHealth = 26;
			sameDirTime = 8.0;
			timeTillDirSwitch = 0.0f;
			health = maxHealth;
			break;
		}
		}
	}
}
