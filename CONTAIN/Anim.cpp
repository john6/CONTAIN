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

	rb = i_entPtr->rb;
	fillColor = i_entPtr->fillColor;
	outlineColor = i_entPtr->outlineColor;

	if (aType == CANNED_EXPLOSION) {
		rb.angVel = 0;
		rb.vel = Vector2f(0.0f, 0.0f);
		origPosition = sf::Vector2f(rb.transform.pos[0], rb.transform.pos[1]);
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
			std::cout << "I am resizing and Im at " + std::to_string(animPercent) + "percent\n";
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
}

void Anim::Destroy()
{
	killMeNextLoop = true;
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
	if (aType != CANNED_EXPLOSION) {
		std::shared_ptr<sf::Shape> drawShape = CreateDrawableRB(i_lerp_fraction);
		drawables->emplace_back(drawShape);
		return drawables;
	}
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
	return drawables;
}

