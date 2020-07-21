#include "TextBox.h"

TextBox::TextBox(std::string i_textString, sf::Font i_font, int i_charSize, int i_width, int i_height)
{
	text = sf::Text(i_textString, i_font, i_charSize);
	//textColor = sf::Color::White;
	//backgroundColor = sf::Color::Green;
	text.setFillColor(sf::Color::White);
	text.setOutlineColor(sf::Color::Black);
	textBox = std::make_shared< sf::RectangleShape>(sf::Vector2f(i_width, i_height));
	textBox->setOrigin(sf::Vector2f(0, 0));
	//textBox->setPosition(sf::Vector2f(0, 0));
	//textBox->setRotation(0);
	textBox->setFillColor(sf::Color::Green);
	textBox->setOutlineColor(sf::Color::Black);
	textBox->setOutlineThickness(5.0f);
	
}

TextBox::~TextBox()
{

}

void TextBox::ResetText(std::string i_str)
{
	text.setString(i_str);
}

void TextBox::SetColors(sf::Color i_colA, sf::Color i_colB)
{
}

//std::vector<sf::Drawable*> TextBox::GetDrawables()
//{
//	return std::vector<sf::Drawable*>();
//}


