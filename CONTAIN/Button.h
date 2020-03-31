#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

class Button
{
public:
	enum BUTTON_STATE { UP, DOWN, HOVER };

private:
	sf::Text m_text;
	sf::RectangleShape m_positionAndSize;
	sf::Color m_colorUp;
	sf::Color m_colorDown;
	sf::Color m_colorHover;
	std::function<void(int)> m_callback;
	Button::BUTTON_STATE m_state;

public:
	Button(std::string string, sf::RectangleShape positionAndSize);
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
	BUTTON_STATE GetState();
	void SetState(BUTTON_STATE state);

private:
	void SetColorsFromState();
};

