#include "Wall.h"
#include "Entity.h"
#include "Anim.h"

Wall::Wall(Vector2f i_startPosition, RigidBody i_rb,
	sf::Color i_fillCol, sf::Color i_outCol, bool i_break) :
	Entity(i_startPosition, i_rb, WALL_BASIC), breakable{ i_break }
{
	lastExplosion = std::chrono::high_resolution_clock::now() - std::chrono::minutes(1);
	explosionRate = 4.5f;
	exploding = false;
	maxHealth = 10;
	currHealth = maxHealth;
	fixedPosition = true;

	if (breakable) {
		sf::Texture texture;
		//texture.setRepeated(true);

		if (textRectWidth < textRectHeight) {
			if (!texture.loadFromFile("Textures/tile146.png", sf::IntRect(0, 0, 32, 64)))
			{
				std::cerr << "failed to load Textures/tile111.png";
			}
			else {
				texturePtr = std::make_shared<sf::Texture>(texture);
			}
			textStretchWidth = (32.0f / textRectWidth);
			textStretchHeight = 0.5;
			//textRectWidth = 64.0f;
			//textRectHeight = 64.0f;
		}
		else {
			if (!texture.loadFromFile("Textures/tile147.png", sf::IntRect(0, 0, 32, 64)))
			{
				std::cerr << "failed to load Textures/tile111.png";
			}
			else {
				texturePtr = std::make_shared<sf::Texture>(texture);
			}
			textStretchWidth = 0.5;
			textStretchHeight = (32.0f / textRectHeight);
		}
		origColorFill = sf::Color::White;
		origColorOutLine = METALLICSILVER;
		deathColorFill = PENNYBROWN;
		deathColorOutLine = PENNYBROWN;

		fillColor = origColorFill;
		outlineColor = origColorOutLine;
	}
	else {
		origColorFill = i_fillCol;
		origColorOutLine = i_outCol;
		deathColorFill = DIMGRAY;
		deathColorOutLine = PENNYBROWN;

		fillColor = origColorFill;
		outlineColor = origColorOutLine;

		//sf::Texture texture;
		////texture.setRepeated(true);
		//if (!texture.loadFromFile("Textures/tile116.png", sf::IntRect(0, 0, 64, 64)))
		//{
		//	std::cerr << "failed to load Textures/tile116.png";
		//}
		//else {
		//	texturePtr = std::make_shared<sf::Texture>(texture);
		//}
	}

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

void Wall::Update(float i_stepSize)
{

	if ((!breakable) && (exploding)) {
		float weaponDelay = (std::chrono::duration_cast<std::chrono::microseconds>(hiResTime::now() - lastExplosion)).count() / 1000000.0f;
		if (weaponDelay >= explosionRate) {
			lastExplosion = std::chrono::high_resolution_clock::now();
			microSec microSec(6000000000);
			std::vector<Vector2f> shapePoints = rb.GetVertCords();
			float minX = INT_MAX;
			float minY = INT_MAX;
			float maxX = INT_MIN;
			float maxY = INT_MIN;
			for (int i = 0; i < shapePoints.size(); i++) {
				if (shapePoints[i][0] < minX) { minX = shapePoints[i][0]; }
				if (shapePoints[i][0] > maxX) { maxX = shapePoints[i][0]; }

				if (shapePoints[i][1] < minY) { minY = shapePoints[i][1]; }
				if (shapePoints[i][1] > maxY) { maxY = shapePoints[i][1]; }
			}

			Vector2f topLeft(minX, minY);
			Vector2f bottomRight(maxX, maxY);

			for (int i = 0; i < 3; i++) {
				Vector2f spawnPos = Math::GetRandomCoordInRect(topLeft, bottomRight);
				std::shared_ptr<Entity> anim = std::make_shared<Anim>(spawnPos, microSec, 3.5f);
				spawnVect.push_back(anim);
			}
		}


	}
}

Wall::~Wall()
{
}

