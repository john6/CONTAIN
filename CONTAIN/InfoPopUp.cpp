#include "InfoPopUp.h"

InfoPopUp::InfoPopUp(RESOURCES* i_resource, float i_width, float i_height, sf::Color i_colA, sf::Color i_colB, sf::Color i_colC) :
	resources{ i_resource }, font{ i_resource->GetFont() }, colA{ i_colA }, colB{ i_colB }, colC{ i_colC }
{
	infoView.reset(sf::FloatRect(0, 0, GLBVRS::SCREEN_WIDTH, GLBVRS::SCREEN_HEIGHT));
	background = std::make_shared< sf::RectangleShape>(sf::Vector2f(i_width, i_height));
	background->setPosition(sf::Vector2f(GLBVRS::HR_MRG, GLBVRS::VRT_MRG));
	background->setFillColor(i_colA);
	background->setOutlineColor(i_colB);
	background->setOutlineThickness(10.0f);
	played = false;
}

InfoPopUp::InfoPopUp()
{
}


void InfoPopUp::AddTextBox(std::string i_str)
{
	sf::Vector2f topRightCorn(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH * (1.0f / 8.0f)), GLBVRS::VRT_MRG + (GLBVRS::CRT_HGHT * (1.0f / 8.0f)));
	sf::Vector2f size(GLBVRS::CRT_WDTH * (6.0f / 8.0f), GLBVRS::CRT_HGHT * (1.0f / 5.0f));
	int textOffsetx = 10.0f;
	int textOffsety = 10.0f;
	sf::Text text = sf::Text(i_str, font, 20);
	text.setFillColor(sf::Color::White);
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(1.0f);
	text.setPosition(sf::Vector2f(topRightCorn.x + textOffsetx, topRightCorn.y + textOffsety));
	std::shared_ptr<sf::RectangleShape> textBox = std::make_shared< sf::RectangleShape>(size);
	textBox->setOrigin(sf::Vector2f(0, 0));
	textBox->setPosition(topRightCorn);
	textBox->setFillColor(colC);
	textBox->setOutlineColor(colB);
	textBox->setOutlineThickness(10.0f);
	textBoxes.push_back(std::make_tuple(text, textBox));
}

void InfoPopUp::PlaceBottomBox()
{
	sf::Vector2f topRightCorn(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH * (1.0f / 8.0f)), GLBVRS::VRT_MRG + (GLBVRS::CRT_HGHT * (7.0f / 8.0f)));
	sf::Vector2f size(GLBVRS::CRT_WDTH * (6.0f / 8.0f), GLBVRS::CRT_HGHT * (1.0f / 16.0f));
	int textOffsetx = GLBVRS::CRT_WDTH * (2.0f / 8.0f);
	int textOffsety = 10.0f;
	std::string sapceStr = "Press SPACE to continue";
	sf::Text text = sf::Text(sapceStr, font, 20);
	text.setFillColor(sf::Color::White);
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(1.0f);
	text.setPosition(sf::Vector2f(topRightCorn.x + textOffsetx, topRightCorn.y + textOffsety));
	std::shared_ptr<sf::RectangleShape> textBox = std::make_shared< sf::RectangleShape>(size);
	textBox->setOrigin(sf::Vector2f(0, 0));
	textBox->setPosition(topRightCorn);
	textBox->setFillColor(colC);
	textBox->setOutlineColor(colB);
	textBox->setOutlineThickness(10.0f);
	textBoxes.push_back(std::make_tuple(text, textBox));
}

InfoPopUp::~InfoPopUp()
{
}


void InfoPopUp::RenderPopUp(sf::RenderWindow * window)
{
	window->clear();

	window->setView(infoView);

	window->draw(*background);
	
	for (std::tuple<sf::Text, std::shared_ptr<sf::RectangleShape>> tBox : textBoxes) {
		window->draw(*std::get<1>(tBox));
		window->draw(std::get<0>(tBox));
	}

	window->display();
}
