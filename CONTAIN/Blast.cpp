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

void Blast::CollideWithEnemy(CollisionData i_coll)
{
	auto enemyPtr = dynamic_cast<Enemy*>(i_coll.entB.get());
	if ((enemyPtr) && (blastType == 0) && (enemyPtr->stunSecs <= 0.0)) {
		enemyPtr->Stun(stunTime);
		//Vector2f blastPos = i_blastPtr->rb.transform.pos;
		Vector2f blastDir = enemyPtr->rb.transform.pos - rb.transform.pos;
		float inverseDistance = std::max((blastRadius - std::abs(blastDir.norm())), 0.0f) / blastRadius;
		float proximityBonus = inverseDistance * strength;
		//std::cout << "blastRadius" << blastRadius << "\n";
		//std::cout << "std::abs(blastDir.norm())" << std::abs(blastDir.norm()) << "\n";
		//std::cout << "inverseDistance" << inverseDistance << "\n";
		blastDir.normalize();
		enemyPtr->rb.ApplyImpulse((blastDir * (strength + proximityBonus)), NULL_VECTOR);
	}
}

void Blast::CollideWithWall(CollisionData i_coll)
{
	auto wallPtr = dynamic_cast<Wall*>(i_coll.entB.get());
	if (wallPtr) {
		//Vector2f blastPos = i_blastPtr->rb.transform.pos;
		Vector2f blastDir = wallPtr->rb.transform.pos - rb.transform.pos;
		float inverseDistance = std::max((blastRadius - std::abs(blastDir.norm())), 0.0f) / blastRadius;
		float proximityBonus = inverseDistance * strength;
		blastDir.normalize();
		wallPtr->rb.ApplyImpulse((blastDir * (strength + proximityBonus)), NULL_VECTOR);
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
	if (blastType == 0) {
		rb.transform.pos = charP->rb.transform.pos;
	}
	float secsInUpdate = i_stepSize / 1000.0f;
	deathTimer -= secsInUpdate;
	if (deathTimer <= 0) { Destroy(); }
}



Blast::~Blast()
{
}
