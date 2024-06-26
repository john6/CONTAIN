#include "BossStream.h"
#include "Projectile.h"

BossStream::BossStream(DIFFICULTY i_diff, Vector2f i_startPosition, bool i_isMiniBoss, int lvl_num, RigidBody i_rb) :
	Enemy(i_diff, i_startPosition, i_rb, ENEMY_BOSS), isMiniBoss{ i_isMiniBoss }
{
	if (!i_isMiniBoss) {
		rb = RigidBody(Physics::CreateRegularPolygon(8, 90.0f), METAL);
	}
	origColorFill = SAGE;
	origColorOutLine = EMERALD;
	fillColor = origColorFill;
	outlineColor = origColorOutLine;
	Stun(0.3f);
	sameDirTime = 0.2;
	timeTillDirSwitch = 0.0f;
	lastShotFired = hiResTime::now();
	currDir = CreateRandomDir();
	SetDiffVars(i_diff, lvl_num);
	if (isMiniBoss) {
		isBoss = false;
	}
	else {
		isBoss = true;
	}
}

void BossStream::Update(float i_stepSize)
{
	UpdateTendrilPosition(i_stepSize);
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
	GenerateDeathEffects(ENEMY_BURST_DEATH);
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
			rb.transform.pos + (projDir * (95)), 1);
		projectile->rb.ApplyImpulse((projDir * projSpeed), NULL_VECTOR);
		spawnVect.push_back(projectile);
	}
}

void BossStream::Stun(float i_stunTime)
{
	stunSecs = i_stunTime * 0.6f;
}

void BossStream::SetDiffVars(int i_diff, int lvl_num)
{
	if (isMiniBoss) {
		rb = RigidBody(std::make_shared<Circle>(60), WOOD);
		switch (i_diff) {
		case EASY: {
			speed = GLBVRS::ENEMYSPEEDEASY;
			projSpeed = 2800.0f;
			numShots = 6;
			maxHealth = 2 + ((lvl_num / 5) * 1);
			shipRateOfFire = 0.40f;
			health = maxHealth;
			break;
		}
		case MEDIUM: {
			speed = GLBVRS::ENEMYSPEEDMED;
			projSpeed = 3000.0f;
			numShots = 7;
			maxHealth = 3 + ((lvl_num / 5) * 2);
			shipRateOfFire = 0.25f;
			health = maxHealth;
			break;
		}
		case HARD: {
			speed = GLBVRS::ENEMYSPEEDHARD;
			projSpeed = 3200.0f;
			numShots = 8;
			maxHealth = 4 + ((lvl_num / 5) * 3);
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
			maxHealth = 25 + ((lvl_num / 5) * 4);
			shipRateOfFire = 0.10f;
			health = maxHealth;
			break;
		}
		case MEDIUM: {
			speed = GLBVRS::ENEMYSPEEDMED;
			projSpeed = 3100.0f;
			numShots = 7;
			maxHealth = 30 + ((lvl_num / 5) * 6);
			shipRateOfFire = 0.05f;
			health = maxHealth;
			break;
		}
		case HARD: {
			speed = GLBVRS::ENEMYSPEEDHARD;
			projSpeed = 3300.0f;
			numShots = 8;
			maxHealth = 36 + ((lvl_num / 5) * 8);
			shipRateOfFire = 0.04f;
			health = maxHealth;
			break;
		}
		}
		speed *= 7;
	}

}
