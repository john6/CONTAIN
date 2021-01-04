#include "Scenery.h"
#include "Entity.h"


Scenery::Scenery(Vector2f i_startPosition, std::shared_ptr <sf::Shape> i_drawable, int i_renderLvl,
	RigidBody i_rb, std::string i_texFileName, float stretchFactWidth, float stretchFactHeight) : Entity(i_startPosition, i_rb, SCENERY), renderLvl { i_renderLvl }
{
	physicalObject = false;
	intangible = true;
	hasVisuals = true;
	drawables = std::make_shared<std::vector<std::shared_ptr<sf::Drawable>>>();

	colorSwitchRate = 3.5f;
	lastColorSwitch = hiResTime::now();

	i_drawable->setPosition(sf::Vector2f(i_startPosition[0], i_startPosition[1]));

	firstDrawb = i_drawable;

	//if (std::shared_ptr<sf::RectangleShape> rectPtr = std::dynamic_pointer_cast<sf::RectangleShape>(i_drawable)) {
	//	drawbWidth = rectPtr->getSize().x;
	//	drawbHeight = rectPtr->getSize().y;
	//}
	//else if (std::shared_ptr<sf::CircleShape> circPtr = std::dynamic_pointer_cast<sf::CircleShape>(i_drawable)) {
	//	drawbWidth = circPtr->getRadius() * 2;
	//	drawbHeight = circPtr->getRadius() * 2;
	//}
	//else { }

	if (i_texFileName != "") {
		drawbWidth = i_drawable->getLocalBounds().width;
		drawbHeight = i_drawable->getLocalBounds().height;

		sf::Texture texture;
		//texture.setRepeated(true);
		std::string filePath = "Textures/" + i_texFileName + ".png";
		if (!texture.loadFromFile(filePath, sf::IntRect(0, 0, 64, 64)))
		{
			std::cerr << "failed to load " << filePath << "\n";
		}
		//if ((i_texFileName == "tile136") && (drawbHeight > drawbWidth)) {
		//	texture.loadFromFile("Textures/tile137.png", sf::IntRect(0, 0, 64, 64));
		//}
		//rect width == number of pixels I want the texutre to attempt to fill
		//number of tiles I want == 64 * rectDimension == 64 * (textRectWidth
		int rectWid = (int)(drawbWidth * stretchFactWidth);
		int rectHeight = (int)(drawbHeight * stretchFactHeight);
		i_drawable->setTextureRect(sf::IntRect(0, 0, rectWid, rectHeight));
		texturePtr = std::make_shared<sf::Texture>(texture);
		texturePtr->setRepeated(true);
		//texturePtr->setSmooth(true);
		i_drawable->setTexture(texturePtr.get());
}

drawables->push_back(i_drawable);
}

Scenery::~Scenery()
{
}

drawablePtrVect Scenery::CreateDrawables(float i_lerp_fraction)
{
	if (drawables->size() == 0) {
		std::cout << "Drawables is empty!\n";
	}
	return drawables;
}

void Scenery::Update(float i_stepSize)
{
	if (renderLvl == 2) {

		float colorSwitchCurr = (std::chrono::duration_cast<std::chrono::microseconds>(hiResTime::now() - lastColorSwitch)).count() / 1000000.0f;
		if (colorSwitchCurr >= colorSwitchRate) {
			lastColorSwitch = hiResTime::now();
		}

		float colorpercent = (colorSwitchCurr / colorSwitchRate) * 2.0f * PI;
		float clrPhse = cos(colorpercent);
		float newA = std::min(255.0f, std::max(30.0f, (100 * clrPhse)));

		std::cout << "newA" << newA << "\n";
		firstDrawb->setFillColor(sf::Color(254, 0, 0, newA));

	}
}
