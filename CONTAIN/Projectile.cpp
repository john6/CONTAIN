#include "Projectile.h"
#include "Entity.h"
#include "Enemy.h"
#include "BossBurst.h"
#include "BossStream.h"
#include "BossSpawn.h"
#include "BossRush.h"
#include "Anim.h"

Projectile::Projectile(Vector2f i_startPosition, int i_projType, RigidBody i_rb) :
	Entity(i_startPosition, i_rb, PROJ_BASIC), projType{ i_projType }
{
	borderThickness = 6.0f;
	if (projType == 0) {
		fillColor = VIVIDGAMBOGE;
		outlineColor = DARKRED;
	}
	else if (projType == 1) {
		fillColor = BASIL;
		outlineColor = EMERALD;
	}
}

Projectile::~Projectile()
{
}

void Projectile::Update(float i_stepSize)
{
}

void Projectile::Destroy() {
	killMeNextLoop = true;
}

void Projectile::CollideWithPlayer(CollisionData i_coll)
{
	if (projType == 1) {
		Explode();
	}
	Destroy();
}

void Projectile::CollideWithProjectile(CollisionData i_coll)
{
	auto projPtr = dynamic_cast<Projectile*>(i_coll.entB.get());
	if ((projPtr) && (projPtr->projType != projType)) {
		Explode();
		Destroy();
	}
}

void Projectile::CollideWithBlocker(CollisionData i_coll)
{
	if (projType == 1) {
		Explode();
		Destroy();
	}
}

void Projectile::CollideWithEnemy(CollisionData i_coll)
{
	auto enemyPtr = dynamic_cast<Enemy*>(i_coll.entB.get());
	if ((enemyPtr) && (enemyPtr->GetTypeID() == ENEMY_BOSS)) {
		if (auto spawnerBoss = dynamic_cast<BossSpawn*>(enemyPtr)) {
			if (spawnerBoss->invulnerable) {

			}
			else {
				if (projType == 0) {
					Explode();
				}
				Destroy();
			}
		}
		else if (auto burstBoss = dynamic_cast<BossBurst*>(enemyPtr)) {
				if (projType == 0) {
					Explode();
					Destroy();
				}
		}
		else if (auto spawnerBoss = dynamic_cast<BossStream*>(enemyPtr)) {
			if (projType == 0) {
				Explode();
				Destroy();
			}
		}
		else if (auto spawnerBoss = dynamic_cast<BossRush*>(enemyPtr)) {
			if (projType == 0) {
				Explode();
				Destroy();
			}
		}

	}
	else {
		if (projType == 0) {
			Explode();
		}
		Destroy();
	}
}

void Projectile::CollideWithWall(CollisionData i_coll)
{
	Destroy();
}

void Projectile::CollideWithPainWall(CollisionData i_coll)
{
	Explode();
	Destroy();
}

void Projectile::Explode()
{
	//lvlPtr->GenerateDeathEffects(this, CANNED_EXPLOSION);
	microSec ms(120000000);
	//microSec ms(1200000000);
	std::shared_ptr<Entity> anim = std::make_shared<Anim>(rb.transform.pos, ms, CANNED_EXPLOSION, this);
	spawnVect.push_back(anim);
}

void Projectile::CollideWithDoor(CollisionData i_coll)
{
	Explode();
	Destroy();
}

