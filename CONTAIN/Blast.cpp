#include "Blast.h"
#include "Enemy.h"
#include "Wall.h"

Blast::Blast(Vector2f i_startPosition, int i_blastType, float i_strength, float i_stunTime, float i_blastRadius) :
	Entity(i_startPosition, RigidBody(std::make_shared<Circle>(i_blastRadius)), BLAST_STUN),
	blastType{ i_blastType }, strength{ i_strength }, stunTime{ i_stunTime }, blastRadius{ i_blastRadius }
{
	charP = GLBVRS::PPTR;
	fillColor = sf::Color(0, 255, 255, 128);
	outlineColor = CYAN;
	intangible = true;
	deathTimerMax = 0.7f;
	deathTimer = deathTimerMax;
	hasVisuals = true;
}

void Blast::CollideWithEnemy(Enemy * i_enemyPtr, CollisionData i_collision)
{
	if ((blastType == 0) && (i_enemyPtr->stunSecs <= 0.0)) {
		i_enemyPtr->Stun(stunTime);
		//Vector2f blastPos = i_blastPtr->rb.transform.pos;
		Vector2f blastDir = i_enemyPtr->rb.transform.pos - rb.transform.pos;
		float proximityBonus = std::abs(blastDir.norm()) * (strength * (1.0f / 2.0f));
		blastDir.normalize();
		i_enemyPtr->rb.ApplyImpulse((blastDir * (strength + proximityBonus)), NULL_VECTOR);
	}
}

void Blast::CollideWithWall(Wall * i_wallPtr)
{
	if ((blastType == 0)) {
		//Vector2f blastPos = i_blastPtr->rb.transform.pos;
		Vector2f blastDir = i_wallPtr->rb.transform.pos - rb.transform.pos;
		float proximityBonus = std::abs(blastDir.norm()) * (strength * (1.0f / 2.0f));
		blastDir.normalize();
		i_wallPtr->rb.ApplyImpulse((blastDir * (strength + proximityBonus)), NULL_VECTOR);
	}
}

void Blast::UpdateVisuals(float i_stepSize)
{
	float fractionTimer = deathTimerMax / 3.0f;
	float pulseTimer = deathTimer;
	while (pulseTimer > fractionTimer) {
		pulseTimer -= fractionTimer;
	}
	float radius = (1 - (pulseTimer / fractionTimer)) * blastRadius;
	Circle circle(radius);
	std::shared_ptr<sf::Shape> drawShapeBase = circle.GetSFMLRepr();
	drawShapeBase->setOrigin(sf::Vector2f(radius, radius));
	drawShapeBase->setFillColor(sf::Color(0, 0, 0, 0));
	drawShapeBase->setOutlineColor(outlineColor);
	drawShapeBase->setPosition(sf::Vector2f(rb.transform.pos[0], rb.transform.pos[1]));
	drawShapeBase->setOutlineThickness(8.0f);
	visuals->emplace_back(drawShapeBase);
}

void Blast::Update(float i_stepSize)
{
	rb.transform.pos = charP->rb.transform.pos;
	float secsInUpdate = i_stepSize / 1000.0f;
	deathTimer -= secsInUpdate;
	if (deathTimer <= 0) { Destroy(); }
}



Blast::~Blast()
{
}
