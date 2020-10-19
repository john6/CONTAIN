#include "Blast.h"

Blast::Blast(Vector2f i_startPosition, int i_blastType, float i_strength, float i_stunTime, RigidBody i_rb) :
	Entity(i_startPosition, i_rb, BLAST_STUN),
	blastType{ i_blastType }, strength{ i_strength }, stunTime{ i_stunTime }
{
	charP = GLBVRS::PPTR;
	fillColor = sf::Color(0, 255, 255, 128);
	outlineColor = CYAN;
	intangible = true;
	deathTimer = 0.7f;
}

Blast::~Blast()
{
}

void Blast::Update(float i_stepSize)
{
	rb.transform.pos = charP->rb.transform.pos;
	float secsInUpdate = i_stepSize / 1000.0f;
	deathTimer -= secsInUpdate;
	if (deathTimer <= 0) { Destroy(); }
}
