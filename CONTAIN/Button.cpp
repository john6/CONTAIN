#include "Button.h"


Button::Button(std::string string, sf::RectangleShape positionAndSize, sf::Font* i_fontPtr) :
	fontPtr{ i_fontPtr }
 {
	SetPositionAndSize(positionAndSize);
	SetText(string);
	m_state = UP;
	SetColorsFromState();
}

Button::~Button() {
}

sf::RectangleShape Button::GetRect() { return m_positionAndSize; }

sf::Text Button::GetText() { return m_text; }

Button::BUTTON_STATE Button::GetState() {
	return m_state;
}

void Button::SetText(std::string s) {
	m_text.setString(s);
	int charSize = std::min(30, static_cast<int>(m_positionAndSize.getSize().x / 14.0f));
	m_text.setCharacterSize(charSize);
	m_text.setOutlineThickness(1.0f);
	m_text.setFont(*fontPtr);
	GLBVRS::SetTextOriginCenter(&m_text);
	m_text.setPosition(sf::Vector2f(m_positionAndSize.getPosition().x + (m_positionAndSize.getSize().x / 2.0f),
		m_positionAndSize.getPosition().y + (m_positionAndSize.getSize().y / 2.0f)));
}

void Button::SetColorsFromState() {
	switch (m_state) {
	case Button::UP: {
		m_positionAndSize.setFillColor(m_colorUp);
		m_positionAndSize.setOutlineColor(m_colorDown);
		m_text.setFillColor(m_colorDown);
		m_text.setOutlineColor(sf::Color::Black);
		break;
	}
	case Button::HOVER: {
		m_positionAndSize.setFillColor(m_colorHover);
		m_positionAndSize.setOutlineColor(m_colorDown);
		m_text.setFillColor(m_colorDown);
		m_text.setOutlineColor(sf::Color::Black);
		break;
	}
	case Button::DOWN: {
		m_positionAndSize.setFillColor(m_colorDown);
		m_positionAndSize.setOutlineColor(m_colorUp);
		m_text.setFillColor(m_colorUp);
		m_text.setOutlineColor(sf::Color::Black);
		break;
	}
	default: {
		m_positionAndSize.setFillColor(m_colorUp);
		m_positionAndSize.setOutlineColor(m_colorDown);
		m_text.setFillColor(m_colorDown);
		m_text.setOutlineColor(sf::Color::Black);
		break;
	}
	}
}

void Button::SetPositionAndSize(sf::RectangleShape rect) {
	m_positionAndSize = rect;
	int outLineThickness = std::min(10, static_cast<int>(rect.getSize().x / 50));
	m_positionAndSize.setOutlineThickness(outLineThickness);
}

void Button::SetSize(sf::Vector2f size) { m_positionAndSize.setSize(size); }

void Button::SetPosition(sf::Vector2f position) { m_positionAndSize.setPosition(position); }

void Button::SetColors(sf::Color colorUp, sf::Color colorDown, sf::Color colorHover) {
	m_colorUp = colorUp;
	m_colorDown = colorDown;
	m_colorHover = colorHover;
	SetColorsFromState();
}

void Button::ResizeForText()
{
	m_positionAndSize.setSize(sf::Vector2f(GLBVRS::BTTN_WDTH + (GLBVRS::BTTN_WDTH * (m_text.getString().getSize() * (1.0f/ 50.0f) )), GLBVRS::BTTN_HGHT));
}

void Button::SetState(BUTTON_STATE state) {
	m_state = state;
	SetColorsFromState();
}

bool Button::HandleInput(sf::Vector2f mousePosition, bool mouseState) {
	if (true) {
		return true;
	}
	return false;
}
