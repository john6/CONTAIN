#include "PlayerController.h"

PlayerController::PlayerController(sf::RenderWindow* i_renderWindow) :
	rWindow{ i_renderWindow }
{

}


PlayerController::~PlayerController()
{
}

std::vector<PlayerController::Input> PlayerController::PollKeys()
{
	std::vector<PlayerController::Input> in;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
		in.push_back(PlayerController::Input::UP);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
		in.push_back(PlayerController::Input::LEFT);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
		in.push_back(PlayerController::Input::DOWN);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		in.push_back(PlayerController::Input::RIGHT);
	}
	return in;
}

Vector2f PlayerController::LeftClick()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		sf::Vector2i mPos = sf::Mouse::getPosition(*rWindow);
		return Eigen::Vector2f(mPos.x, mPos.y);
	}
	else {
		return Eigen::Vector2f(0.0f, 0.0f);
	}
}

Vector2f PlayerController::RightClick()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		sf::Vector2i mPos = sf::Mouse::getPosition(*rWindow);
		return Eigen::Vector2f(mPos.x, mPos.y);
	}
	else {
		return Eigen::Vector2f(0.0f, 0.0f);
	}
}

bool PlayerController::ScrollClick()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
		return true;
	}
	else {
		return false;
	}
}