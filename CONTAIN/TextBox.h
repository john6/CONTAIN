#pragma once
#include <SFML/Graphics.hpp>

class TextBox
{
private:

	//std::shared_ptr <sf::RectangleShape> textBox;

	sf::Color textColor;
	sf::Color backgroundColor;

public:
	sf::Text text;
	std::shared_ptr<sf::Shape> textBox;

	void ResetText(std::string i_str);

	void SetColors(sf::Color i_colA, sf::Color i_colB);

	//std::vector<sf::Drawable*> GetDrawables();

	TextBox(std::string i_textString, sf::Font i_font, int i_charSize, int i_width, int i_height);
	~TextBox();
};

