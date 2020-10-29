#include "BossSpawn.h"
#include "CrazyBoi.h"

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
	CheckChildren();
	if (children.size() < 2) {
		invulnerable = false;
		ChangeColorHealth();
	}
	else {
		invulnerable = true;
		fillColor = SILVER;
		outlineColor = DIMGRAY;
	}
	UpdateHealth(i_stepSize);
	if (stunSecs < 0) {
		if (timeTillDirSwitch < 0) {
			if (children.size() < 2) {
				timeTillDirSwitch = sameDirTime;
				GLBVRS::RSRCS->PlaySound(RESOURCES::BRAKE);
				SpawnEnemies(numShots, ENEMY_RAND, 0);
				SpawnEnemies(numShots, ENEMY_SEEK, 0);
			}
			else {
				timeTillDirSwitch = sameDirTime;
				GLBVRS::RSRCS->PlaySound(RESOURCES::BRAKE);
				SpawnEnemies(std::max((int)numShots - 2, 1), ENEMY_RAND, 0);
				SpawnEnemies(std::max((int)numShots - 2, 1), ENEMY_SEEK, 0);
			}
		}
		else {
			float secsInUpdate = i_stepSize / 1000.0f;
			timeTillDirSwitch -= secsInUpdate;
		}
	}
	else {
		float secsInUpdate = i_stepSize / 1000.0f;
		stunSecs -= secsInUpdate;
	}
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


/// <summary>This method will generate the requested number of enemies, allowing the caller to specify enemy type and a size modification. 
/// The enemy types are limited to basic enemies in order to not overwhelm the player, though it can be expanded if appropriate. </summary>
void BossSpawn::SpawnEnemies(int i_numEnems, TypeID i_enemyType, int i_sizeMod)
{
	for (int i = 0; i < i_numEnems; ++i) {
		std::random_device sizeSeed;
		std::mt19937 genRoomSeed(sizeSeed());
		std::discrete_distribution<> widthModDist({ 10, 15, 50, 15, 10 });
		std::discrete_distribution<> radiusModDist({ 0, 15, 55, 20, 10 });
		std::discrete_distribution<> heightModDist({ 13, 18, 40, 18, 13 });
		int randWidth = widthModDist(genRoomSeed);
		int ranDifwidth = (randWidth - 2 + i_sizeMod) * 10;
		int randHeight = heightModDist(genRoomSeed);
		int randDifHeight = (randHeight - 2 + i_sizeMod) * 10;
		int randRad = radiusModDist(genRoomSeed);
		int randDifRadius = (randRad - 2 + i_sizeMod) * 10;
		int rectBaseDim = 65;
		int circBaseRad = 40;
		std::shared_ptr<Entity> ent;
		switch (i_enemyType) {
		case ENEMY_SEEK: {
			std::shared_ptr<Shape> shape = std::make_shared<Rectangle>(
				(rectBaseDim + ranDifwidth), (rectBaseDim + randDifHeight));
			RigidBody projBody = RigidBody(shape, ROCK);
			ent = std::make_shared<Enemy>(diff, rb.transform.pos, projBody);
			break;
		}
		case ENEMY_RAND: {
			std::shared_ptr<Shape> shape = std::make_shared<Circle>((circBaseRad + randDifRadius));
			RigidBody projBody = RigidBody(shape, ROCK);
			ent = std::make_shared<CrazyBoi>(diff, rb.transform.pos, projBody);
			break;
		}
		case ENEMY_SEEK_PUSH: {
			std::shared_ptr<Shape> shape = std::make_shared<Rectangle>(
				(rectBaseDim + ranDifwidth), (rectBaseDim + randDifHeight));
			RigidBody projBody = RigidBody(shape, ROCK);
			ent = std::make_shared<Enemy>(diff, rb.transform.pos, projBody);
			break;
		}
		case ENEMY_RAND_PUSH:
			std::shared_ptr<Shape> shape = std::make_shared<Circle>((circBaseRad + randDifRadius));
			RigidBody projBody = RigidBody(shape, ROCK);
			ent = std::make_shared<CrazyBoi>(diff, rb.transform.pos, projBody);
			break;
		}
		spawnVect.push_back(ent);
		children.push_back((std::weak_ptr<Entity>)ent);
	}
}

void BossSpawn::CheckChildren()
{
	auto childIter = children.begin();
	while (childIter != children.end()) {
		if (childIter->expired()) {
			childIter = children.erase(childIter);
		}
		else {
			++childIter;
		}
	}
}
