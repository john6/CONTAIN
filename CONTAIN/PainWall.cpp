#include "PainWall.h"


PainWall::PainWall(Vector2f i_startPosition, RigidBody i_rb) :
	Entity(i_startPosition, i_rb, WALL_FIRE)
{
	intangible = true;
	fixedPosition = true;
	colorSwitchRate = 3;
	lastColorSwitch = hiResTime::now();
	colorState = true;
	fillColor = VIVIDYELLOW;
	outlineColor = sf::Color(0.0f, 0.0f, 0.0f, 0.0f);
	//colorA = MYSTICRED;
	colorA = RED;
	colorB = MYSTICRED;

	sf::Texture texture;
	//if (!texture.loadFromFile("Textures/tile089.png", sf::IntRect(0, 0, 64, 64)))
	//{
	//	std::cerr << "failed to load Textures/tile091.png";
	//}
	//else {
	//	//texturePtr = std::make_shared<sf::Texture>(texture);
	//}


	if (textRectWidth < textRectHeight) {
		if (!texture.loadFromFile("Textures/tile139.png", sf::IntRect(0, 0, 64, 64)))
		{
			std::cerr << "failed to load Textures/tile091.png";
		}
		else {
			texturePtr = std::make_shared<sf::Texture>(texture);
		}
		textStretchWidth = 64.0f / textRectWidth;
		textStretchHeight = 0.5f;
		//textRectWidth = 64.0f;
		//textRectHeight = 64.0f;
	}
	else {
		if (!texture.loadFromFile("Textures/tile138.png", sf::IntRect(0, 0, 64, 64)))
		{
			std::cerr << "failed to load Textures/tile091.png";
		}
		else {
			texturePtr = std::make_shared<sf::Texture>(texture);
		}
		textStretchWidth = 0.5f;
		textStretchHeight = 64.0f / textRectHeight;
	}

}

PainWall::~PainWall()
{
}


void PainWall::Update(float i_stepSize)
{
	float colorSwitchCurr = (std::chrono::duration_cast<std::chrono::microseconds>(hiResTime::now() - lastColorSwitch)).count() / 1000000.0f;
	if (colorSwitchCurr >= colorSwitchRate) {
		lastColorSwitch = hiResTime::now();
	}
	float colorpercent = (colorSwitchCurr / colorSwitchRate) * 2.0f * PI;
	float clrPhse = cos(colorpercent);
	float clrPhseInv = 1.0f - clrPhse;
	float r = std::min(255.0f, std::max(0.0f, (colorA.r * clrPhse) + (colorB.r * clrPhseInv)));
	float g = std::min(255.0f, std::max(0.0f, (colorA.g * clrPhse) + (colorB.g * clrPhseInv)));
	float b = std::min(255.0f, std::max(0.0f, (colorA.b * clrPhse) + (colorB.b * clrPhseInv)));
	fillColor = sf::Color(r, g, b);
	float secsInUpdate = i_stepSize / 1000.0f;
}
