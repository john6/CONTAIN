#include "BossBurst.h"
#include "Projectile.h"

BossBurst::BossBurst(DIFFICULTY i_diff, Vector2f i_startPosition, bool i_isMiniBoss, RigidBody i_rb) :
	Enemy(i_diff, i_startPosition, i_rb, ENEMY_BOSS), isMiniBoss{ i_isMiniBoss }
{
	if (!i_isMiniBoss) {
		rb = RigidBody(Physics::CreateRegularPolygon(5, 100.0f), METAL);
	}
	origColorFill = PARAKEET;
	origColorOutLine = EMERALD;
	fillColor = origColorFill;
	outlineColor = origColorOutLine;
	Stun(0.3f);
	sameDirTime = 0.3;
	timeTillDirSwitch = 0.0f;
	lastShotFired = hiResTime::now();
	shipRateOfFire = 1.0f;
	currDir = CreateRandomDir();
	SetDiffVars(i_diff);
}

void BossBurst::Update(float i_stepSize)
{
	UpdateTendrilPosition(i_stepSize);
	UpdateHealth(i_stepSize);
	if (stunSecs < 0) {
		//apply impulse towards player
		Vector2f playerDir = charPtr->rb.transform.pos - rb.transform.pos;
		playerDir.normalize();
		float moveDist = (speed / 2.0f) * i_stepSize;
		rb.ApplyImpulse(playerDir * moveDist, NULL_VECTOR);
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
		//attempt to shoot projectiles all over the place
		shootProj();
	}
	else {
		float secsInUpdate = i_stepSize / 1000.0f;
		stunSecs -= secsInUpdate;
	}
}

void BossBurst::TakeDamage(float i_dmg)
{
	GLBVRS::RSRCS->PlaySound(RESOURCES::OUCH3);
	health -= i_dmg;
	ChangeColorHealth();
}

void BossBurst::Destroy()
{
	GLBVRS::RSRCS->PlaySound(RESOURCES::BOSSEXPLODE);
	//sectPtr->GenerateDeathEffects(this, ENEMY_BURST_DEATH);
	killMeNextLoop = true;
	if (isMiniBoss) {
		DropPowerUp();
	}
}

void BossBurst::shootProj()
{
	weaponDelay = (std::chrono::duration_cast<std::chrono::microseconds>(hiResTime::now() - lastShotFired)).count() / 1000000.0f;
	if (weaponDelay >= shipRateOfFire) {
		GLBVRS::RSRCS->PlaySound(RESOURCES::SWORD5);
		lastShotFired = hiResTime::now();
		int i = 0;
		while (i < numShots) {
			Vector2f projDir = CreateRandomDir();
			projDir.normalize();
			std::shared_ptr<Entity> projectile = std::make_shared<Projectile>(
				rb.transform.pos + (projDir * (100.0f + (i * 15.0f))), 1);
			projectile->rb.ApplyImpulse((projDir * projSpeed), NULL_VECTOR);
			spawnVect.push_back(projectile);
			i++;
		}
	}
}

void BossBurst::Stun(float i_stunTime)
{
	stunSecs = i_stunTime / 2.0f;
}

void BossBurst::SetDiffVars(int i_diff)
{
	if (isMiniBoss) {
		rb = RigidBody(std::make_shared<Circle>(60), WOOD);
		switch (i_diff) {
		case EASY: {
			speed = GLBVRS::ENEMYSPEEDEASY;
			projSpeed = 2800.0f;
			numShots = 2;
			maxHealth = 2;
			health = maxHealth;
			break;
		}
		case MEDIUM: {
			speed = GLBVRS::ENEMYSPEEDMED;
			projSpeed = 3000.0f;
			numShots = 2;
			maxHealth = 3;
			health = maxHealth;
			break;
		}
		case HARD: {
			speed = GLBVRS::ENEMYSPEEDHARD;
			projSpeed = 3200.0f;
			numShots = 3;
			maxHealth = 4;
			health = maxHealth;
			break;
		}
		}
		speed *= 1.5;
	}
	else {
		switch (i_diff) {
		case EASY: {
			speed = GLBVRS::ENEMYSPEEDEASY;
			projSpeed = 2800.0f;
			numShots = 7;
			maxHealth = 8;
			health = maxHealth;
			break;
		}
		case MEDIUM: {
			speed = GLBVRS::ENEMYSPEEDMED;
			projSpeed = 3000.0f;
			numShots = 8;
			maxHealth = 9;
			health = maxHealth;
			break;
		}
		case HARD: {
			speed = GLBVRS::ENEMYSPEEDHARD;
			projSpeed = 3200.0f;
			numShots = 9;
			maxHealth = 10;
			health = maxHealth;
			break;
		}
		}
		speed *= 6;
	}
}
