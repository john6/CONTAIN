#include "BossRush.h"
#include "PlayerChar.h"

BossRush::BossRush(DIFFICULTY i_diff, Vector2f i_startPosition, bool i_isMiniBoss, RigidBody i_rb) :
	Enemy(i_diff, i_startPosition, i_rb, ENEMY_BOSS), isMiniBoss{ i_isMiniBoss }
{
	origColorFill = MIDNIGHTMOSS;
	origColorOutLine = METALLICSILVER;
	fillColor = origColorFill;
	outlineColor = origColorOutLine;
	Stun(1.0f);
	rushTime = 3.0f;
	waitTime = 1.0f;
	rushing = true;
	timeTillSwitch = rushTime;
	lastShotFired = hiResTime::now();
	shipRateOfFire = 1.0f;
	currDir = CreateRandomDir();
	SetDiffVars(i_diff);
}

void BossRush::Update(float i_stepSize)
{
	UpdateHealth(i_stepSize);
	if (stunSecs < 0) {
		float secsInUpdate = i_stepSize / 1000.0f;
		timeTillSwitch -= secsInUpdate;
		if (rushing) {
			Vector2f playerDir = charPtr->rb.transform.pos - rb.transform.pos;
			playerDir.normalize();
			float moveDist = (speed / 2.0f) * i_stepSize;
			rb.ApplyImpulse(playerDir * moveDist, NULL_VECTOR);
			if (timeTillSwitch < 0) {
				// also apply random impulse, lets see what happens lul
				rushing = false;
				timeTillSwitch = waitTime;
			}
		}
		else {
			rb.vel -= (rb.vel * secsInUpdate * (1.0f / waitTime));
			if (timeTillSwitch < 0) {
				// also apply random impulse, lets see what happens lul
				rushing = true;
				timeTillSwitch = rushTime;
			}
		}
	}
	else {
		float secsInUpdate = i_stepSize / 1000.0f;
		stunSecs -= secsInUpdate;
	}
}

void BossRush::Destroy()
{
	GLBVRS::RSRCS->PlaySound(RESOURCES::BOSSEXPLODE);
	//sectPtr->GenerateDeathEffects(this, ENEMY_BURST_DEATH);
	killMeNextLoop = true;
	if (isMiniBoss) {
		DropPowerUp();
	}
}

void BossRush::TakeDamage(float i_dmg)
{
	GLBVRS::RSRCS->PlaySound(RESOURCES::OUCH3);
	health -= i_dmg;
	ChangeColorHealth();
}

void BossRush::shootProj()
{
}

void BossRush::Stun(float i_stunTime)
{
	stunSecs = i_stunTime / 2.0f;
}

void BossRush::SetDiffVars(int i_diff)
{
	if (isMiniBoss) {
		rb = RigidBody(std::make_shared<Circle>(60 * GLBVRS::SIZE_RAT), HEAVYBOUNCE);
		switch (i_diff) {
		case EASY: {
			speed = GLBVRS::ENEMYSPEEDEASY;
			numShots = 6;
			maxHealth = 2;
			health = maxHealth;
			break;
		}
		case MEDIUM: {
			speed = GLBVRS::ENEMYSPEEDMED;
			numShots = 7;
			maxHealth = 3;
			health = maxHealth;
			break;
		}
		case HARD: {
			speed = GLBVRS::ENEMYSPEEDHARD;
			numShots = 8;
			maxHealth = 4;
			health = maxHealth;
			break;
		}
		}
		speed *= 13;
	}
	else {
		switch (i_diff) {
		case EASY: {
			speed = GLBVRS::ENEMYSPEEDEASY;
			numShots = 6;
			maxHealth = 9;
			health = maxHealth;
			break;
		}
		case MEDIUM: {
			speed = GLBVRS::ENEMYSPEEDMED;
			numShots = 7;
			maxHealth = 10;
			health = maxHealth;
			break;
		}
		case HARD: {
			speed = GLBVRS::ENEMYSPEEDHARD;
			numShots = 8;
			maxHealth = 11;
			health = maxHealth;
			break;
		}
		}
		speed *= 28;
	}
}

void BossRush::CollideWithPainWall(PainWall * i_painWallPtr)
{
	//sectPtr->PlaySound(RESOURCES::FIRE5);
	//TakeDamage(1);
}

void BossRush::CollideWithPlayer(PlayerChar * i_playerPtr)
{
	i_playerPtr->TakeDamage(1.0f);
	//rushing = false;
	//timeTillDirSwitch = sameDirTime;
}
