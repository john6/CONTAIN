#include "PowerUp.h"
#include "Entity.h"


PowerUp::PowerUp(Vector2f i_startPosition, UPGRADE_TYPE i_powType, RigidBody i_rb) :
	Entity(i_startPosition, i_rb, UPGRADE), powType{ i_powType }
{
	intangible = true;
	hasVisuals = true;


	switch (i_powType) {
	case (RATE_OF_FIRE): {
		powColor = sf::Color(255, 128, 128);
		fillColor = BLACK;
		outlineColor = FORDSILVER;
		break;
	}
	case (SCATTER): {
		powColor = sf::Color(51, 153, 102);
		fillColor = BLACK;
		outlineColor = FORDSILVER;
		break;
	}
	case (TEMP_HEALTH): {
		powColor = RED;
		fillColor = BLACK;
		outlineColor = FORDSILVER;
		break;
	}
						//TODO
	}
}

PowerUp::~PowerUp()
{
}

void PowerUp::Update(float i_stepSize)
{
	rb.angVel = 0.02f;
}

void PowerUp::UpdateVisuals(float i_stepSize)
{
	float dimSize = 50.0f;
	Rectangle rect(dimSize, dimSize);
	std::shared_ptr<sf::Shape> rectDrawShape = rect.GetSFMLRepr();
	rectDrawShape->setOrigin(sf::Vector2f(dimSize / 2, dimSize / 2));
	rectDrawShape->setFillColor(fillColor);
	rectDrawShape->setOutlineColor(outlineColor);
	rectDrawShape->setPosition(sf::Vector2f(rb.transform.pos[0], rb.transform.pos[1]));
	float oppositeRotation = 360 - (((rb.transform.orient - PI) * 180) / PI);
	rectDrawShape->setRotation(oppositeRotation);
	rectDrawShape->setOutlineThickness(4.0f);
	visuals->emplace_back(rectDrawShape);

	std::vector<Vector2f> pointArray({ Vector2f(0.0f, 0.0f), Vector2f(-20.0f, 20.0f), Vector2f(-20.0f, 60.0f), Vector2f(0.0f, 80.0f), Vector2f(20.0f, 60.0f), Vector2f(20.0f, 20.0f) });
	Polygon poly = Polygon(pointArray);
	std::shared_ptr<sf::Shape> drawShapeBase = poly.GetSFMLRepr();
	drawShapeBase->setFillColor(powColor);
	drawShapeBase->setOutlineColor(WHITE);
	drawShapeBase->setPosition(sf::Vector2f(rb.transform.pos[0], rb.transform.pos[1]));
	//float oppositeRotation = 360 - (((rb.transform.orient - PI) * 180) / PI);
	//drawShapeBase->setRotation(oppositeRotation);
	drawShapeBase->setOutlineThickness(2.0f);
	visuals->emplace_back(drawShapeBase);
}
