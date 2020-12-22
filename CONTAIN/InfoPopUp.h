#pragma once
#include <SFML/Graphics.hpp>
#include "TextBox.h"
#include "GLOBAL_CONSTANTS.h"
#include "RESOURCES.h"
#include "GameRenderer.h"

class InfoPopUp
{
private:


	std::shared_ptr<sf::Shape> background;
	std::vector<std::tuple<sf::Text, std::shared_ptr<sf::RectangleShape>>> textBoxes;
	sf::Color colA;
	sf::Color colB;
	sf::Color colC;
	sf::Font font;
	sf::View infoView;

public:	
	bool played;

	RESOURCES* resources;
	InfoPopUp(RESOURCES* i_resource, float i_width, float i_height, sf::Color i_colA = DARKVIOLET, sf::Color i_colB = INDIGO, sf::Color i_colC = DIMGRAY);
	InfoPopUp();
	~InfoPopUp();

	//void SetSize(float i_width, float i_height);

	//void SetColorScheme(sf::Color i_colA, sf::Color i_colB, sf::Color i_colC);

	void RenderPopUp(sf::RenderWindow* window);

	void AddTextBox(std::string i_str);

	void PlaceBottomBox();
};

