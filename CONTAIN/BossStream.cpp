#include "BossStream.h"
#include "Projectile.h"

BossStream::BossStream(DIFFICULTY i_diff, Vector2f i_startPosition, bool i_isMiniBoss, RigidBody i_rb) :
	Enemy(i_diff, i_startPosition, i_rb, ENEMY_BOSS), isMiniBoss{ i_isMiniBoss }
{
	origColorFill = SAGE;
	origColorOutLine = EMERALD;
	fillColor = origColorFill;
	outlineColor = origColorOutLine;
	Stun(0.3f);
	sameDirTime = 0.2;
	timeTillDirSwitch = 0.0f;
	lastShotFired = hiResTime::now();
	currDir = CreateRandomDir();
	SetDiffVars(i_diff);
}

void BossStream::Update(float i_stepSize)
{
	UpdateHealth(i_stepSize);
	if (stunSecs < 0) {
		rb.angVel = 0.03f;
		if (timeTillDirSwitch < 0) {
			// also apply random impulse, lets see what happens lul
			timeTillDirSwitch = sameDirTime;
			currDir = CreateRandomDir();
			float moveDist = speed * i_stepSize;
			rb.ApplyImpulse(currDir * moveDist, NULL_VECTOR);
		}
		else {
			float secsInUpdate = i_stepSize / 1000.0f;
			timeTillDirSwitch -= secsInUpdate;
			float moveDist = speed * i_stepSize;
			rb.ApplyImpulse(currDir * moveDist, NULL_VECTOR);
		}
		shootProj();
	}
	else {
		float secsInUpdate = i_stepSize / 1000.0f;
		stunSecs -= secsInUpdate;
	}
}

void BossStream::TakeDamage(float i_dmg)
{
	GLBVRS::RSRCS->PlaySound(RESOURCES::OUCH3);
	health -= i_dmg;
	ChangeColorHealth();
}

void BossStream::Destroy()
{
	GLBVRS::RSRCS->PlaySound(RESOURCES::BOSSEXPLODE);
	//sectPtr->GenerateDeathEffects(this, ENEMY_BURST_DEATH);
	killMeNextLoop = true;
	if (isMiniBoss) {
		DropPowerUp();
	}
}

void BossStream::shootProj()
{
	weaponDelay = (std::chrono::duration_cast<std::chrono::microseconds>(hiResTime::now() - lastShotFired)).count() / 1000000.0f;
	if (weaponDelay >= shipRateOfFire) {
		GLBVRS::RSRCS->PlaySound(RESOURCES::PLAYERSHOOT);
		lastShotFired = hiResTime::now();
		Vector2f projDir = Math::AngleToVect(rb.transform.orient);
		projDir.normalize();
		std::shared_ptr<Entity> projectile = std::make_shared<Projectile>(
			rb.transform.pos + (projDir * (100.0f)), 1);
		projectile->rb.ApplyImpulse((projDir * projSpeed), NULL_VECTOR);
		spawnVect.push_back(projectile);
	}
}

void BossStream::Stun(float i_stunTime)
{
	stunSecs = i_stunTime / 2.0f;
}

void BossStream::SetDiffVars(int i_diff)
{
	if (isMiniBoss) {
		rb = RigidBody(std::make_shared<Circle>(60), WOOD);
		switch (i_diff) {
		case EASY: {
			speed = GLBVRS::ENEMYSPEEDEASY;
			projSpeed = 2800.0f;
			numShots = 6;
			maxHealth = 2;
			shipRateOfFire = 0.40f;
			health = maxHealth;
			break;
		}
		case MEDIUM: {
			speed = GLBVRS::ENEMYSPEEDMED;
			projSpeed = 3000.0f;
			numShots = 7;
			maxHealth = 3;
			shipRateOfFire = 0.25f;
			health = maxHealth;
			break;
		}
		case HARD: {
			speed = GLBVRS::ENEMYSPEEDHARD;
			projSpeed = 3200.0f;
			numShots = 8;
			maxHealth = 4;
			shipRateOfFire = 0.15f;
			health = maxHealth;
			break;
		}
		}
		speed *= 2;
	}
	else {
		switch (i_diff) {
		case EASY: {
			speed = GLBVRS::ENEMYSPEEDEASY;
			projSpeed = 2900.0f;
			numShots = 6;
			maxHealth = 10;
			shipRateOfFire = 0.10f;
			health = maxHealth;
			break;
		}
		case MEDIUM: {
			speed = GLBVRS::ENEMYSPEEDMED;
			projSpeed = 3100.0f;
			numShots = 7;
			maxHealth = 11;
			shipRateOfFire = 0.05f;
			health = maxHealth;
			break;
		}
		case HARD: {
			speed = GLBVRS::ENEMYSPEEDHARD;
			projSpeed = 3300.0f;
			numShots = 8;
			maxHealth = 12;
			shipRateOfFire = 0.04f;
			health = maxHealth;
			break;
		}
		}
		speed *= 7;
	}

}
