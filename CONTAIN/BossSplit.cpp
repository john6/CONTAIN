#include "BossSplit.h"


BossSplit::BossSplit(DIFFICULTY i_diff, int i_splitsLeft, float i_spdFct, bool i_crazy, Vector2f i_startPosition, RigidBody i_rb) :
	Enemy(i_diff, i_startPosition, i_rb, ENEMY_BOSS), splitsLeft{ i_splitsLeft }, diff{ i_diff }, crazy{ i_crazy }
{
	origColorFill = CHARTREUSE;
	origColorOutLine = MOSS;
	fillColor = origColorFill;
	outlineColor = origColorOutLine;
	speed = i_spdFct;
	Stun(0.05f);
	sameDirTime = 0.6;
	timeTillDirSwitch = 0.0f;
	lastShotFired = hiResTime::now();
	shipRateOfFire = 1.0f;
	currDir = CreateRandomDir();
	SetDiffVars(i_diff);
	hasVisuals = false;
}

void BossSplit::Update(float i_stepSize)
{
	UpdateHealth(i_stepSize);
	if (stunSecs < 0) {
		if (crazy) {
			if (timeTillDirSwitch < 0) {
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
		}
		else {
			Vector2f playerDir = charPtr->rb.transform.pos - rb.transform.pos;
			playerDir.normalize();
			float moveDist = speed * i_stepSize;
			rb.ApplyImpulse(playerDir * moveDist, NULL_VECTOR);
		}
	}
	else {
		float secsInUpdate = i_stepSize / 1000.0f;
		stunSecs -= secsInUpdate;
	}
}

void BossSplit::Destroy()
{
	switch (splitsLeft) {
	case 4: {
		GLBVRS::RSRCS->PlaySound(RESOURCES::PLOPBANANA);
		std::shared_ptr<Shape> shape1 = std::make_shared<Circle>(100);
		std::shared_ptr<Shape> shape2 = std::make_shared<Circle>(100);
		RigidBody projBody1 = RigidBody(shape1, LESSBOUNCYBALL);
		RigidBody projBody2 = RigidBody(shape2, LESSBOUNCYBALL);
		std::shared_ptr<Entity> split1 = std::make_shared<BossSplit>(diff, splitsLeft - 1, 4, true, rb.transform.pos, projBody1);
		std::shared_ptr<Entity> split2 = std::make_shared<BossSplit>(diff, splitsLeft - 1, 4, false, rb.transform.pos, projBody2);
		spawnVect.push_back(split1);
		spawnVect.push_back(split2);
		killMeNextLoop = true;
		break;
	}
	case 3: {
		GLBVRS::RSRCS->PlaySound(RESOURCES::PLOPBANANA);
		std::shared_ptr<Shape> shape1 = std::make_shared<Circle>(75);
		std::shared_ptr<Shape> shape2 = std::make_shared<Circle>(75);
		RigidBody projBody1 = RigidBody(shape1, LESSBOUNCYBALL);
		RigidBody projBody2 = RigidBody(shape2, LESSBOUNCYBALL);
		std::shared_ptr<Entity> split1 = std::make_shared<BossSplit>(diff, splitsLeft - 1, 2.5f, true, rb.transform.pos, projBody1);
		std::shared_ptr<Entity> split2 = std::make_shared<BossSplit>(diff, splitsLeft - 1, 2.5f, false, rb.transform.pos, projBody2);
		spawnVect.push_back(split1);
		spawnVect.push_back(split2);
		killMeNextLoop = true;
		break;
	}
	case 2: {
		GLBVRS::RSRCS->PlaySound(RESOURCES::PLOPBANANA);
		std::shared_ptr<Shape> shape1 = std::make_shared<Circle>(50);
		std::shared_ptr<Shape> shape2 = std::make_shared<Circle>(50);
		RigidBody projBody1 = RigidBody(shape1, LESSBOUNCYBALL);
		RigidBody projBody2 = RigidBody(shape2, LESSBOUNCYBALL);
		std::shared_ptr<Entity> split1 = std::make_shared<BossSplit>(diff, splitsLeft - 1, 1.4f, true, rb.transform.pos, projBody1);
		std::shared_ptr<Entity> split2 = std::make_shared<BossSplit>(diff, splitsLeft - 1, 1.4f, false, rb.transform.pos, projBody2);
		spawnVect.push_back(split1);
		spawnVect.push_back(split2);
		killMeNextLoop = true;
		break;
	}
	case 1: {
		GLBVRS::RSRCS->PlaySound(RESOURCES::PLOPBANANA);
		std::shared_ptr<Shape> shape1 = std::make_shared<Circle>(30);
		std::shared_ptr<Shape> shape2 = std::make_shared<Circle>(30);
		RigidBody projBody1 = RigidBody(shape1, BOUNCYBALL);
		RigidBody projBody2 = RigidBody(shape2, BOUNCYBALL);
		std::shared_ptr<Entity> split1 = std::make_shared<BossSplit>(diff, splitsLeft - 1, true, 0.75f, rb.transform.pos, projBody1);
		std::shared_ptr<Entity> split2 = std::make_shared<BossSplit>(diff, splitsLeft - 1, false, 0.60f, rb.transform.pos, projBody2);
		spawnVect.push_back(split1);
		spawnVect.push_back(split2);
		killMeNextLoop = true;
		break;
	}
	case 0: {
		GLBVRS::RSRCS->PlaySound(RESOURCES::PLOP);
		killMeNextLoop = true;
	}
	}
	//sectPtr->GenerateDeathEffects(this, ENEMY_BURST_DEATH);
}

void BossSplit::TakeDamage(float i_dmg)
{
	health -= i_dmg;
	ChangeColorHealth();
}

void BossSplit::shootProj()
{
}

void BossSplit::Stun(float i_stunTime)
{
	stunSecs = i_stunTime * 0.6f;
}

void BossSplit::SetDiffVars(int i_diff)
{
	if (splitsLeft == 4) {
		switch (i_diff) {
		case EASY: {
			speed *= GLBVRS::ENEMYSPEEDEASY;
			numShots = 6;
			maxHealth = 3;
			health = maxHealth;
			break;
		}
		case MEDIUM: {
			speed *= GLBVRS::ENEMYSPEEDMED;
			numShots = 7;
			maxHealth = 4;
			health = maxHealth;
			break;
		}
		case HARD: {
			speed *= GLBVRS::ENEMYSPEEDHARD;
			numShots = 8;
			maxHealth = 5;
			health = maxHealth;
			break;

		}
		}
	}
	else if (splitsLeft == 3) {
		switch (i_diff) {
		case EASY: {
			speed *= GLBVRS::ENEMYSPEEDEASY;
			numShots = 6;
			maxHealth = 3;
			health = maxHealth;
			break;
		}
		case MEDIUM: {
			speed *= GLBVRS::ENEMYSPEEDMED;
			numShots = 7;
			maxHealth = 4;
			health = maxHealth;
			break;
		}
		case HARD: {
			speed *= GLBVRS::ENEMYSPEEDHARD;
			numShots = 8;
			maxHealth = 5;
			health = maxHealth;
			break;
		}
		}
	}
	if (splitsLeft == 2) {
		switch (i_diff) {
		case EASY: {
			speed *= GLBVRS::ENEMYSPEEDEASY;
			numShots = 6;
			maxHealth = 2;
			health = maxHealth;
			break;
		}
		case MEDIUM: {
			speed *= GLBVRS::ENEMYSPEEDMED;
			numShots = 7;
			maxHealth = 3;
			health = maxHealth;
			break;
		}
		case HARD: {
			speed *= GLBVRS::ENEMYSPEEDHARD;
			numShots = 8;
			maxHealth = 4;
			health = maxHealth;
			break;
		}

		}
	}
	if (splitsLeft == 1) {
		switch (i_diff) {
		case EASY: {
			speed *= GLBVRS::ENEMYSPEEDEASY;
			numShots = 6;
			maxHealth = 1;
			health = maxHealth;
			break;
		}
		case MEDIUM: {
			speed *= GLBVRS::ENEMYSPEEDMED;
			numShots = 7;
			maxHealth = 1;
			health = maxHealth;
			break;
		}
		case HARD: {
			speed *= GLBVRS::ENEMYSPEEDHARD;
			numShots = 8;
			maxHealth = 1;
			health = maxHealth;
			break;
		}
		}
	}
	if (splitsLeft == 0) {
		switch (i_diff) {
		case EASY: {
			speed *= GLBVRS::ENEMYSPEEDEASY;
			numShots = 6;
			maxHealth = 1;
			health = maxHealth;
			break;
		}
		case MEDIUM: {
			speed *= GLBVRS::ENEMYSPEEDMED;
			numShots = 7;
			maxHealth = 1;
			health = maxHealth;
			break;
		}
		case HARD: {
			speed *= GLBVRS::ENEMYSPEEDHARD;
			numShots = 8;
			maxHealth = 1;
			health = maxHealth;
			break;
		}
		}
	}
}
