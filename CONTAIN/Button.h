#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include "GLOBAL_CONSTANTS.h"
#include <algorithm> 

class Button
{
public:
	enum BUTTON_STATE { UP, DOWN, HOVER };

private:
	sf::Font* fontPtr;
	sf::Text m_text;
	sf::RectangleShape m_positionAndSize;
	sf::Color m_colorUp;
	sf::Color m_colorDown;
	sf::Color m_colorHover;
	std::function<void(int)> m_callback;
	Button::BUTTON_STATE m_state;

public:
	Button(std::string string, sf::RectangleShape positionAndSize, sf::Font* i_fontPtr);
	Button() = default;
	~Button();

	sf::RectangleShape GetRect();
	sf::Text GetText();
	void SetText(std::string s);
	void SetPositionAndSize(sf::RectangleShape rect);
	void SetSize(sf::Vector2f size);
	void SetPosition(sf::Vector2f position);
	bool HandleInput(sf::Vector2f mousePosition, bool mouseState);
	void SetColors(sf::Color colorUp, sf::Color colorDown, sf::Color colorHover);
	void ResizeForText();
	BUTTON_STATE GetState();
	void SetState(BUTTON_STATE state);

private:
	void SetColorsFromState();
};

