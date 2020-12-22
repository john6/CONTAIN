#include "Wall.h"
#include "Entity.h"
#include "Anim.h"

Wall::Wall(Vector2f i_startPosition, RigidBody i_rb,
	sf::Color i_fillCol, sf::Color i_outCol, bool i_break) :
	Entity(i_startPosition, i_rb, WALL_BASIC), breakable{ i_break }
{
	maxHealth = 10;
	currHealth = maxHealth;
	fixedPosition = true;
	origColorFill = i_fillCol;
	origColorOutLine = i_outCol;
	deathColorFill = DIMGRAY;
	deathColorOutLine = PENNYBROWN;

	fillColor = origColorFill;
	outlineColor = origColorOutLine;
}

void Wall::CollideWithProjectile(CollisionData i_coll)
{
	if (breakable) {
		TakeDamage(1.0f);
	}
}

void Wall::TakeDamage(float i_dmg)
{
	currHealth -= i_dmg;
	ChangeColorHealth();
	if (currHealth == 0.0f) {
		GenerateDeathEffects();
	}
	if (currHealth <= 0.0f) {
		rb.mat = DENSE_METAL;
		rb.SetMassData();
	}
}

void Wall::ChangeColorHealth()
{
	float lifeRatio = std::max((currHealth / maxHealth), 0.0f);
	float fillColorR = std::max(std::min((int)((lifeRatio * origColorFill.r) + ((1.0f - lifeRatio) * deathColorFill.r)), 255), 0);
	float fillColorG = std::max(std::min((int)((lifeRatio * origColorFill.g) + ((1.0f - lifeRatio) * deathColorFill.g)), 255), 0);
	float fillColorB = std::max(std::min((int)((lifeRatio * origColorFill.b) + ((1.0f - lifeRatio) * deathColorFill.b)), 255), 0);
	float outLineColorR = std::max(std::min((int)((lifeRatio * origColorOutLine.r) + ((1.0f - lifeRatio) * deathColorOutLine.r)), 255), 0);
	float outLineColorG = std::max(std::min((int)((lifeRatio * origColorOutLine.g) + ((1.0f - lifeRatio) * deathColorOutLine.g)), 255), 0);
	float outLineColorB = std::max(std::min((int)((lifeRatio * origColorOutLine.b) + ((1.0f - lifeRatio) * deathColorOutLine.b)), 255), 0);
	fillColor = sf::Color(fillColorR, fillColorG, fillColorB);
	outlineColor = sf::Color(outLineColorR, outLineColorG, outLineColorB);
}

void Wall::GenerateDeathEffects()
{
	microSec microSec(250000000);
	std::vector<Vector2f> shapePoints = rb.GetVertCords();
	float minX = INT_MAX;
	float minY = INT_MAX;
	float maxX = INT_MIN;
	float maxY= INT_MIN;
	for (int i = 0; i < shapePoints.size(); i++) {
		if (shapePoints[i][0] < minX) { minX = shapePoints[i][0]; }
		if (shapePoints[i][0] > maxX) { maxX = shapePoints[i][0]; }

		if (shapePoints[i][1] < minY) { minY = shapePoints[i][1]; }
		if (shapePoints[i][1] > maxY) { maxY = shapePoints[i][1]; }
	}

	Vector2f topLeft(minX, minY);
	Vector2f bottomRight(maxX, maxY);

	for (int i = 0; i < 4; i++) {
		Vector2f spawnPos = Math::GetRandomCoordInRect(topLeft, bottomRight);
		std::shared_ptr<Entity> anim = std::make_shared<Anim>(spawnPos, microSec);
		spawnVect.push_back(anim);
	}
}

Wall::~Wall()
{
}

