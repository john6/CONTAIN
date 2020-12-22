#include "Anim.h"

Anim::Anim(Vector2f i_startPosition, microSec i_lifetime,
	ANIMTYPE i_aType, Entity* i_entPtr) :
	Entity(i_startPosition, RigidBody(std::make_shared<Circle>(1.0f)), ANIMATION),
	birthTime{ hiResTime::now() }, deathTime{ birthTime + i_lifetime }, lifeTime{ i_lifetime },
	aType{ i_aType }
{
	physicalObject = false;
	intangible = true;
	hasVisuals = true;
	drawables = std::make_shared<std::vector<std::shared_ptr<sf::Drawable>>>();
	rb.vel *= 0.1;

	if (i_entPtr) { 
	rb = i_entPtr->rb; 
	fillColor = i_entPtr->fillColor;
	outlineColor = i_entPtr->outlineColor;
	}
	else {
		fillColor = YELLOWCYBER;
		outlineColor = RED;
	}
	rb.transform.pos = i_startPosition; //rb position was overriding the actual inputted position


	if (aType == CANNED_EXPLOSION) {
		rb.angVel = 0;
		rb.vel = Vector2f(0.0f, 0.0f);
		//origPosition = sf::Vector2f(rb.transform.pos[0], rb.transform.pos[1]);
		origPosition = sf::Vector2f(i_startPosition[0], i_startPosition[1]);
		auto regPoly1 = Physics::CreateRegularPolygon(5, 30.0f);
		auto regPoly2 = Physics::CreateRegularPolygon(5, 15.0f);
		polys.push_back(regPoly1);
		polys.push_back(regPoly2);
	}/*
	else if (i_entPtr != NULL) {
		fillColor = i_entPtr->fillColor;
		outlineColor = i_entPtr->outlineColor;
		rb = i_entPtr->rb;
		}*/
	if (rb.shape->GetType() == Shape::ShapeType::CIRCLE) {
		auto circle = dynamic_cast<Circle*>(rb.shape.get());
		origRadius = circle->radius;
	}
	if (rb.shape->GetType() == Shape::ShapeType::RECTANGLE) {
		auto rectangle = dynamic_cast<Rectangle*>(rb.shape.get());
		origWidth = rectangle->GetWidth();
		origHeight = rectangle->GetHeight();
	}
}



//This constructor is for creating sparks
Anim::Anim(Vector2f direction, std::vector<Vector2f> i_contactPoints, microSec i_lifetime, int i_number, int i_splashOption) :
	Entity(i_contactPoints[0], RigidBody(std::make_shared<Circle>(1.0f)), ANIMATION),
	birthTime{ hiResTime::now() }, deathTime{ birthTime + i_lifetime }, lifeTime{ i_lifetime },
	aType{ SPARKS }
{
	physicalObject = false;
	intangible = true;
	hasVisuals = true;
	drawables = std::make_shared<std::vector<std::shared_ptr<sf::Drawable>>>();
	rb.vel *= 0.1;

	collisionNormal = direction;


	if (i_splashOption == 0) {
		fillColor = YELLOWCYBER;
		outlineColor = YELLOWCYBER;
	}
	else if (i_splashOption == 1) {
		fillColor = BASIL;
		outlineColor = PEAR;
	}
	
	rb.transform.pos = i_contactPoints[0];

	spread = 4.0f;


		rb.angVel = 0;
		rb.vel = Vector2f(0.0f, 0.0f);
		//origPosition = sf::Vector2f(rb.transform.pos[0], rb.transform.pos[1]);
		origPosition = sf::Vector2f(i_contactPoints[0][0], i_contactPoints[0][1]);


		//Physics::CreateRegularPolygon(5, 15.0f);
		////auto regPoly1 = PolyLib
		////auto regPoly2 = Physics::CreateRegularPolygon(5, 15.0f);
		//polys.push_back(regPoly1);
		//polys.push_back(regPoly2);




		float collisionDir = Math::VectToAngle(collisionNormal);


		auto poly1 = std::make_shared<Polygon>(std::vector<Vector2f>({ Vector2f(0.0f, 0.0f), Vector2f(10.0f, 15.0f), Vector2f(20.0f, 0.0f) }));
		auto rb1 = std::make_shared<RigidBody>(poly1);
		rb1->ResetOrientation(((collisionDir*180.0f) / PI) + 90.0f);
		rb1->ResetPosition(i_contactPoints[0]);
		rbs.push_back(rb1);


		auto poly2 = std::make_shared<Polygon>(std::vector<Vector2f>({ Vector2f(0.0f, 0.0f), Vector2f(10.0f, 15.0f), Vector2f(20.0f, 0.0f) }));
		auto rb2 = std::make_shared<RigidBody>(poly2);
		rb2->ResetOrientation(((collisionDir*180.0f) / PI) - 90.0f);
		rb2->ResetPosition(i_contactPoints[0]);
		rbs.push_back(rb2);

		auto poly3 = std::make_shared<Polygon>(std::vector<Vector2f>({ Vector2f(0.0f, 0.0f), Vector2f(10.0f, 15.0f), Vector2f(20.0f, 0.0f) }));
		auto rb3 = std::make_shared<RigidBody>(poly3);
		rb3->ResetOrientation(((collisionDir*180.0f) / PI) + 45.0f);
		rb3->ResetPosition(i_contactPoints[0]);
		rbs.push_back(rb3);

		auto poly4 = std::make_shared<Polygon>(std::vector<Vector2f>({ Vector2f(0.0f, 0.0f), Vector2f(10.0f, 15.0f), Vector2f(20.0f, 0.0f) }));
		auto rb4 = std::make_shared<RigidBody>(poly4);
		rb4->ResetOrientation(((collisionDir*180.0f) / PI) - 45.0f);
		rb4->ResetPosition(i_contactPoints[0]);
		rbs.push_back(rb4);
}




Anim::~Anim()
{
}

void Anim::Update(float i_stepSize)
{
	//Im gonna make the polygon shrink to !almost! nothing before it dissapears
	float microSecsAlive = (hiResTime::now() - birthTime).count();
	float lifeTimeFloat = lifeTime.count();
	float lifePercent = microSecsAlive / lifeTimeFloat;
	float lowBound = std::max(lifePercent, 0.1f);
	float upBound = std::min(1.0f, lowBound);
	float animPercent = upBound;
	if (animPercent > 0.99f) {
		Destroy();
	}
	else if (aType == CANNED_EXPLOSION) {
		for (int i = 0; i < polys.size(); i++) {
			//std::cout << "I am resizing and Im at " + std::to_string(animPercent) + "percent\n";
			polys[i]->ResetSize(animPercent);
		}
	}
	else if (aType == ENEMY_BURST_DEATH) {
		if ((rb.shape->GetType() == Shape::ShapeType::CIRCLE)) {
			auto circle = dynamic_cast<Circle*>(rb.shape.get());
			float radius = circle->radius;
			float sizeIncrease = animPercent * (origRadius * 0.5f);
			circle->ChangeSizeOfShape(origRadius + sizeIncrease, 0.0f);
		}
		else if (rb.shape->GetType() == Shape::ShapeType::RECTANGLE) {
			auto rect = dynamic_cast<Rectangle*>(rb.shape.get());
			float sizeIncreaseWidth = animPercent * (origWidth * 0.5f);
			float sizeIncreaseHeight = animPercent * (origHeight * 0.5f);
			rect->ChangeSizeOfShape(origWidth + sizeIncreaseWidth, origHeight + sizeIncreaseHeight);
		}
	}
	else if (aType == SPARKS) {
		for (int i = 0; i < rbs.size(); i++) {
			//std::cout << "I am resizing and Im at " + std::to_string(animPercent) + "percent\n";
			auto poly = dynamic_cast<Polygon*>(rbs[i]->shape.get());
			//poly->ResetSize(1 - animPercent);
			rbs[i]->AdjustPosition(Math::AngleToVect(rbs[i]->transform.orient) * animPercent * -spread);

		}
	}
}

void Anim::UpdateCanned(float i_stepSize)
{

}

void Anim::UpdateBurstDeath(float i_stepSize)
{

}

void Anim::UpdateSparks(float i_stepSize)
{

}


drawablePtrVect Anim::CreateDrawables(float i_lerp_fraction)
{
	float microSecsAlive = (hiResTime::now() - birthTime).count();
	float lifeTimeFloat = lifeTime.count();
	float lifePercent = microSecsAlive / lifeTimeFloat;
	float lowBound = std::max(lifePercent, 0.1f);
	float upBound = std::min(1.0f, lowBound);
	float animPercent = upBound;
	drawables->clear();
	if (aType == SPARKS) {

		for (int i = 0; i < rbs.size(); i++) {
			std::shared_ptr<sf::Shape> polyDrawable = rbs[i]->shape->GetSFMLRepr();

			polyDrawable->setOrigin(sf::Vector2f(rbs[i]->shape->GetSFMLOriginOffset()(0), rbs[i]->shape->GetSFMLOriginOffset()(1)));
			polyDrawable->setPosition(rbs[i]->transform.pos(0), rbs[i]->transform.pos(1));
			polyDrawable->setRotation(rbs[i]->transform.orient);
			if (i % 2 == 0) {
				polyDrawable->setFillColor(fillColor);
			}
			else {
				polyDrawable->setFillColor(outlineColor);
			}

			drawables->push_back(polyDrawable);
		}
	}
	if (aType == ENEMY_BURST_DEATH) {
		std::shared_ptr<sf::Shape> drawShape = CreateDrawableRB(i_lerp_fraction);
		drawables->emplace_back(drawShape);
		return drawables;
	}
	if (aType == CANNED_EXPLOSION) {
		for (int i = 0; i < polys.size(); i++) {
			std::shared_ptr<sf::Shape> polyDrawable = polys[i]->GetSFMLRepr();
			if ((i % 2) == 0) {
				polyDrawable->setFillColor(fillColor);
				polyDrawable->setOutlineColor(outlineColor);
			}
			else {
				polyDrawable->setFillColor(outlineColor);
				polyDrawable->setOutlineColor(fillColor);
			}
			polyDrawable->setOutlineThickness(6.0f);
			polyDrawable->setPosition(origPosition);
			polyDrawable->setPosition(origPosition);
			float rotationDir = i == 0 ? 1 : (i * -1);
			float rotationOffset = i * 36;
			polyDrawable->setRotation((animPercent * 270 * rotationDir) + rotationOffset);

			drawables->push_back(polyDrawable);
		}
	}

	return drawables;
}

void Anim::Destroy()
{
	killMeNextLoop = true;
}