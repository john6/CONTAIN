#include "Enemy.h"
#include "PlayerChar.h"
#include "Projectile.h"
#include "PowerUp.h"
#include "Anim.h"
#include "Scenery.h"

Enemy::Enemy(DIFFICULTY i_diff, Vector2f i_startPosition, RigidBody i_rb, TypeID i_typeID) :
	Entity(i_startPosition, i_rb, i_typeID), tendrilSize{ i_rb.shape->GetArea() / 4000.0f },
trianglePoly{ Polygon({ Vector2f(0.0f, 0.0f), Vector2f(4.5f * tendrilSize, 12.0f * tendrilSize), Vector2f(18.0f * tendrilSize, 0.0f) }) }
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

	//Visuals
	deathColorFill = UMBERBROWN;
	deathColorOutLine = PENNYBROWN;
	hasVisuals = true;
	tendrilRotation = 0;
	//Give a circle enemy a bigger triangle
	/*if (rb.GetVertCords().size() == 1) {
		trianglePoly = Polygon({ Vector2f(0.0f, 0.0f), Vector2f(18.f * tendrilSize, 46.0f * tendrilSize), Vector2f(52.0f * tendrilSize, 0.0f) });
	}*/
}

Enemy::~Enemy()
{
}

void Enemy::Update(float i_stepSize)
{ //enemies cant all access the player's location at the same time, so updates shouldnt be parallel
	UpdateHealth(i_stepSize);
	float secsInUpdate = i_stepSize / 1000.0f;
	if (stunSecs < 0) {
		Vector2f playerDir = charPtr->rb.transform.pos - rb.transform.pos;
		playerDir.normalize();
		float moveDist = speed * i_stepSize;
		rb.ApplyImpulse(playerDir * moveDist, NULL_VECTOR);
		tendrilRotation += .2 / secsInUpdate; 
		//tendrilRotation += std::abs(rb.vel.norm()) /secsInUpdate;
		std::string speed = std::to_string(std::abs(rb.vel.norm()) / secsInUpdate);
		std::cout << "speed" << speed << "\n";
	}
	else {
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
	GenerateDeathEffects(ENEMY_BURST_DEATH);
	killMeNextLoop = true;
}

void Enemy::GenerateDeathEffects(ANIMTYPE animType)
{
	int splatNum = 2;
	int circleSplatNum = 2;

	std::random_device rd1;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen1(rd1()); //Standard mersenne_twister_engine seeded with rd()


	if (animType == ENEMY_BURST_DEATH) {
		for (int i = 0; i < splatNum; i++) {
			microSec ms(100000000);
			std::shared_ptr<Entity> anim = std::make_shared<Anim>(rb.transform.pos, ms, ENEMY_BURST_DEATH, this);
			spawnVect.push_back(anim);
			if (rb.shape->GetType() == Shape::ShapeType::CIRCLE) {
				auto circle = dynamic_cast<Circle*>(rb.shape.get());
				std::shared_ptr<sf::Shape> corpse = Physics::CreateIrregularPolygon(5, circle->radius * 3)->GetSFMLRepr();
				int corpseR = (fillColor.r + 0) / 2;
				int corpseG = (fillColor.g + 0) / 2;
				int corpseB = (fillColor.b + 0) / 2;
				sf::Color corpseCol(corpseR, corpseG, corpseB, 178);
				corpse->setFillColor(corpseCol);
				corpse->setOutlineColor(corpseCol);
				corpse->setPosition(sf::Vector2f(rb.transform.pos.x(), rb.transform.pos.y()));
				std::shared_ptr<Entity> corpseEnt = std::make_shared<Scenery>(rb.transform.pos, corpse);
				spawnVect.push_back(corpseEnt);
			}
			else if (rb.shape->GetType() == Shape::ShapeType::RECTANGLE) {
				auto rectangle = dynamic_cast<Rectangle *> (rb.shape.get());
				std::shared_ptr<sf::Shape> corpse = Physics::CreateIrregularPolygon(5, (rectangle->GetWidth() + rectangle->GetHeight())*1.0f)->GetSFMLRepr();
				int corpseR = (fillColor.r + 0) / 2;
				int corpseG = (fillColor.g + 0) / 2;
				int corpseB = (fillColor.b + 0) / 2;
				sf::Color corpseCol(corpseR, corpseG, corpseB, 178);
				corpse->setFillColor(corpseCol);
				corpse->setOutlineColor(corpseCol);
				corpse->setPosition(sf::Vector2f(rb.transform.pos.x(), rb.transform.pos.y()));
				std::shared_ptr<Entity> corpseEnt = std::make_shared<Scenery>(rb.transform.pos, corpse);
				spawnVect.push_back(corpseEnt);
			}
			else if (rb.shape->GetType() == Shape::ShapeType::POLYGON) {
				auto poly = dynamic_cast<Polygon *> (rb.shape.get());
				std::shared_ptr<sf::Shape> corpse = Physics::CreateIrregularPolygon(poly->numPoints + 1, poly->GetDistToCorner() * 2)->GetSFMLRepr();
				int corpseR = (fillColor.r + 0) / 2;
				int corpseG = (fillColor.g + 0) / 2;
				int corpseB = (fillColor.b + 0) / 2;
				sf::Color corpseCol(corpseR, corpseG, corpseB, 170);
				corpse->setFillColor(corpseCol);
				corpse->setOutlineColor(corpseCol);
				corpse->setPosition(sf::Vector2f(rb.transform.pos.x(), rb.transform.pos.y()));
				std::shared_ptr<Entity> corpseEnt = std::make_shared<Scenery>(rb.transform.pos, corpse);
				spawnVect.push_back(corpseEnt);
			}
		}
		for (int i = 0; i < circleSplatNum; i++) {
			float circRadius;
			if (rb.shape->GetType() == Shape::ShapeType::CIRCLE) {
				auto circle = dynamic_cast<Circle*>(rb.shape.get());
				circRadius = circle->radius * 0.5;
			}
			else if (rb.shape->GetType() == Shape::ShapeType::RECTANGLE) {
				auto rectangle = dynamic_cast<Rectangle *> (rb.shape.get());
				circRadius = (rectangle->GetWidth() + rectangle->GetHeight())*0.25f;
			}
			else if (rb.shape->GetType() == Shape::ShapeType::POLYGON) {
				auto poly = dynamic_cast<Polygon *> (rb.shape.get());
				circRadius = poly->GetDistToCorner() * 0.75f;
			}
			std::uniform_int_distribution<> transformDistrib(-(int)circRadius, (int)circRadius); //both boundaries are inclusive

			int randPosDiffX = transformDistrib(gen1);
			int randPosDiffY = transformDistrib(gen1);
			int corpseR = ((fillColor.r + 0) / 2); // +(randPosDiffX / 4.0);
			int corpseG = ((fillColor.g + 0) / 2); // +(randPosDiffX / 4.0);
			int corpseB = ((fillColor.b + 0) / 2); // +(randPosDiffX / 4.0);
			sf::Color corpseCol(corpseR, corpseG, corpseB, 170);
			std::shared_ptr<sf::Shape> corpse;
			float radius;
			if (i == 0) {
				radius = (circRadius * 1.5);
				corpse = std::make_shared<sf::CircleShape>(radius);
				corpse->setPosition(sf::Vector2f(rb.transform.pos.x(), rb.transform.pos.y()));

			}
			else {
				radius = circRadius * 0.50f + (randPosDiffY * 0.40f);
				corpse = std::make_shared<sf::CircleShape>(radius);
				corpse->setPosition(sf::Vector2f(rb.transform.pos.x() + (randPosDiffX * 2.0f), rb.transform.pos.y() + (randPosDiffY * 2.0f)));
			}
			corpse->setOrigin(sf::Vector2f(radius * 0.75f, radius * 0.75f));
			corpse->setFillColor(corpseCol);
			corpse->setOutlineColor(corpseCol);

			std::shared_ptr<Entity> corpseEnt = std::make_shared<Scenery>(rb.transform.pos, corpse);
			spawnVect.push_back(corpseEnt);
		}
	}
	if (animType == CANNED_EXPLOSION) {
		microSec ms(5000000000);
		std::shared_ptr<Entity> anim = std::make_shared<Anim>(rb.transform.pos, ms, CANNED_EXPLOSION, this);
		spawnVect.push_back(anim);
	}
}

void Enemy::CollideWithPainWall(PainWall * i_painWallPtr)
{
	//GLBVRS::RSRCS->PlaySound(RESOURCES::FIREBALL);
	TakeDamage(1);
}

void Enemy::CollideWithProjectile(Projectile * i_projPtr)
{
	if (i_projPtr->projType == 0) {
		TakeDamage(1);
	}
}

void Enemy::UpdateVisuals(float i_stepSize)
{
	if (visuals->size() == 0) {
		std::vector<Vector2f> verts = rb.GetVertCords();
		int numVerts = verts.size();
		for (int i = 0; i < numVerts; i++) {
			std::shared_ptr<sf::Shape> drawShapeBase = trianglePoly.GetSFMLRepr();
			drawShapeBase->setFillColor(outlineColor);
			drawShapeBase->setOutlineColor(fillColor);
			int nextVert = (i + 1) % numVerts;
			//sf::Vector2f midPoint(verts[i][0], verts[i][1]);
			sf::Vector2f midPoint((verts[i][0] + verts[nextVert][0]) / 2, (verts[i][1] + verts[nextVert][1]) / 2);
			drawShapeBase->setPosition(midPoint);
			//float oppositeRotation = 360 - (((rb.transform.orient - PI) * 180) / PI);
			drawShapeBase->setRotation(tendrilRotation + (i * 0.3f));
			drawShapeBase->setOutlineThickness(2.0f);
			visuals->emplace_back(drawShapeBase);
		}
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

drawablePtrVect Enemy::CreateDrawables(float i_lerp_fraction)
{
	if (drawables->size() > 0) { drawables->clear(); }
	if (visuals->size() > 0) { visuals->clear(); }
	//for linear interpolation this will now use the previous position and orientation

	if (hasVisuals) {
		UpdateVisuals(i_lerp_fraction);
		auto visualIt = visuals->begin();
		while (visualIt != visuals->end()) {
			drawables->emplace_back(*visualIt);
			visualIt++;
		}
	}
	if (physicalObject) {
		std::shared_ptr<sf::Shape> drawShape = CreateDrawableRB(i_lerp_fraction);
		drawables->emplace_back(drawShape);
	}
	return drawables;
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
