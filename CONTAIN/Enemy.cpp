#include "Enemy.h"
#include "PlayerChar.h"
#include "Blast.h"
#include "Projectile.h"
#include "PowerUp.h"

Enemy::Enemy(DIFFICULTY i_diff, Vector2f i_startPosition, RigidBody i_rb, TypeID i_typeID) :
	Entity(i_startPosition, i_rb, i_typeID)
{
	charPtr = GLBVRS::PPTR;
	metal = false;
	origColorFill = MOSS;
	origColorOutLine = PEAR;
	fillColor = origColorFill;
	outlineColor = origColorOutLine;
	SetDiffVars(i_diff);
	maxHealth = 1;
	health = maxHealth;
	deathColorFill = UMBERBROWN;
	deathColorOutLine = PENNYBROWN;
}

Enemy::~Enemy()
{
}

void Enemy::Update(float i_stepSize)
{ //enemies cant all access the player's location at the same time, so updates shouldnt be parallel
	UpdateHealth(i_stepSize);
	if (stunSecs < 0) {
		Vector2f playerDir = charPtr->rb.transform.pos - rb.transform.pos;
		playerDir.normalize();
		float moveDist = speed * i_stepSize;
		rb.ApplyImpulse(playerDir * moveDist, NULL_VECTOR);
	}
	else {
		float secsInUpdate = i_stepSize / 1000.0f;
		stunSecs -= secsInUpdate;
	}
}

Vector2f Enemy::CreateRandomDir()
{
	std::random_device rd1;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen1(rd1()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> distrib(1, 20); //both boundaries are inclusive
	float randX = (distrib(gen1) / 10.0f) - 1.0f;
	float randY = (distrib(gen1) / 10.0f) - 1.0f;
	return Vector2f(randX, randY);
}

void Enemy::Destroy() {
	//GLBVRS::RSRCS->PlaySound(RESOURCES::OUCH1);
	//sectPtr->GenerateDeathEffects(this, ENEMY_BURST_DEATH);
	killMeNextLoop = true;
}

void Enemy::CollideWithPainWall(PainWall * i_painWallPtr)
{
	//GLBVRS::RSRCS->PlaySound(RESOURCES::FIREBALL);
	TakeDamage(1);
}

void Enemy::CollideWithPlayer(PlayerChar * i_playerPtr)
{
	if (!metal) {
		i_playerPtr->TakeDamage(1.0f);
	}
}

void Enemy::CollideWithBlast(Blast * i_blastPtr)
{
	if ((i_blastPtr->blastType == 0) && (stunSecs <= 0.0)) {
		Stun(i_blastPtr->stunTime);
		//Vector2f blastPos = i_blastPtr->rb.transform.pos;
		Vector2f blastDir = rb.transform.pos - i_blastPtr->rb.transform.pos;
		float proximityBonus = std::abs(blastDir.norm()) * (i_blastPtr->strength * (1.0f / 4.0f));
		blastDir.normalize();
		rb.ApplyImpulse((blastDir * (i_blastPtr->strength + proximityBonus)), NULL_VECTOR);
	}
}

void Enemy::CollideWithProjectile(Projectile * i_projPtr)
{
	if (i_projPtr->projType == 0) {
		TakeDamage(1);
	}
}

void Enemy::DropPowerUp()
{
	std::shared_ptr<Entity> smallShipPOW3 = std::make_shared<PowerUp>(
		Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH * (2.0f / 5.0f)), GLBVRS::CRT_HGHT / 2.0)), TEMP_HEALTH);
	smallShipPOW3->rb.transform.pos = rb.transform.pos;
	spawnVect.push_back(smallShipPOW3);
}

void Enemy::UpdateHealth(float i_stepSize)
{
	if (health <= 0.0f) {
		//fillColor = sf::Color::Red;
		//outlineColor = sf::Color::Red;
		Destroy();
	}
}

void Enemy::TakeDamage(float i_dmg)
{
	health -= i_dmg;
	ChangeColorHealth();
}

void Enemy::Stun(float i_stunTime)
{
	stunSecs = i_stunTime;
}

void Enemy::SetDiffVars(int i_diff)
{
	switch (i_diff) {
	case EASY: {
		speed = GLBVRS::ENEMYSPEEDEASY;
		Stun(0.7f);
		break;
	}
	case MEDIUM: {
		speed = GLBVRS::ENEMYSPEEDMED;
		Stun(0.45f);
		break;
	}
	case HARD: {
		Stun(0.3f);
		speed = GLBVRS::ENEMYSPEEDHARD;
		break;
	}
	}
}

void Enemy::ChangeColorHealth()
{
	float lifeRatio = std::max((health / maxHealth), 0.0f);
	float fillColorR = std::max(std::min((int)((lifeRatio * origColorFill.r) + ((1.0f - lifeRatio) * deathColorFill.r)), 255), 0);
	float fillColorG = std::max(std::min((int)((lifeRatio * origColorFill.g) + ((1.0f - lifeRatio) * deathColorFill.g)), 255), 0);
	float fillColorB = std::max(std::min((int)((lifeRatio * origColorFill.b) + ((1.0f - lifeRatio) * deathColorFill.b)), 255), 0);
	float outLineColorR = std::max(std::min((int)((lifeRatio * origColorOutLine.r) + ((1.0f - lifeRatio) * deathColorOutLine.r)), 255), 0);
	float outLineColorG = std::max(std::min((int)((lifeRatio * origColorOutLine.g) + ((1.0f - lifeRatio) * deathColorOutLine.g)), 255), 0);
	float outLineColorB = std::max(std::min((int)((lifeRatio * origColorOutLine.b) + ((1.0f - lifeRatio) * deathColorOutLine.b)), 255), 0);
	fillColor = sf::Color(fillColorR, fillColorG, fillColorB);
	outlineColor = sf::Color(outLineColorR, outLineColorG, outLineColorB);
}

void Enemy::TurnToMetal()
{
	metal = true;
	fillColor = UMBERBROWN;
	outlineColor = PENNYBROWN;
	rb.mat = DENSE_METAL;
	speed *= 3.0f;
	rb.SetMassData();
}
