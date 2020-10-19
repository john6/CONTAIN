#include "Projectile.h"
#include "Entity.h"
#include "Enemy.h"
#include "BossSpawn.h"
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

void Projectile::CollideWithPlayer(PlayerChar * i_playerPtr)
{
	if (projType == 1) {
		Explode();
	}
	Destroy();
}

void Projectile::CollideWithProjectile(Projectile * i_projPtr)
{
	if (i_projPtr->projType != projType) {
		Explode();
		Destroy();
	}
}

void Projectile::CollideWithBlocker(Blocker * i_blockPtr)
{
	if (projType == 1) {
		Explode();
		Destroy();
	}
}

void Projectile::CollideWithEnemy(Enemy * i_enemyPtr)
{
	if (i_enemyPtr->GetTypeID() == ENEMY_BOSS) {
		if (auto spawnerBoss = dynamic_cast<BossSpawn*>(i_enemyPtr)) {
			if (spawnerBoss->invulnerable) {

			}
			else {
				if (projType == 0) {
					Explode();
				}
				Destroy();
			}
		}
		else {
			if (projType == 0) {
				Explode();
			}
			Destroy();
		}
	}
	Explode();
	Destroy();
}

void Projectile::CollideWithWall(Wall * i_wallPtr)
{
	Destroy();
}

void Projectile::CollideWithPainWall(PainWall * i_painWallPtr)
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

void Projectile::CollideWithDoor(Door * i_doorPtr)
{
	Explode();
	Destroy();
}

