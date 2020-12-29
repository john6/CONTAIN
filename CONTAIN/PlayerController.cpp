#include "PlayerController.h"

PlayerController::PlayerController(sf::RenderWindow* i_renderWindow) :
	rWindow{ i_renderWindow }
{

}


PlayerController::~PlayerController()
{
}

Eigen::Vector2f PlayerController::PollWASDKeys()
{
		Vector2f wasdDir(0.0f, 0.0f);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
			wasdDir += Vector2f(0.0f, -1.0f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
			wasdDir += Vector2f(-1.0f, 0.0f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
			wasdDir += Vector2f(0.0f, 1.0f);
		}
if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
	wasdDir += Vector2f(1.0f, 0.0f);
}
wasdDir.normalize();
return wasdDir;
}

Vector2f PlayerController::LeftClick()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		sf::Vector2i mScreenPos = sf::Mouse::getPosition(*rWindow);
		sf::Vector2f mWorldPos = rWindow->mapPixelToCoords(mScreenPos, rWindow->getView());
		return Eigen::Vector2f(mWorldPos.x, mWorldPos.y);
	}
	else {
		return Eigen::Vector2f(0.0f, 0.0f);
	}
}

Vector2f PlayerController::RightClick()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		sf::Vector2i mScreenPos = sf::Mouse::getPosition(*rWindow);
		sf::Vector2f mWorldPos = rWindow->mapPixelToCoords(mScreenPos, rWindow->getView());
		return Eigen::Vector2f(mWorldPos.x, mWorldPos.y);
	}
	else {
		return Eigen::Vector2f(0.0f, 0.0f);
	}
}

Eigen::Vector2f PlayerController::GetMousePos()
{
	sf::Vector2i mScreenPos = sf::Mouse::getPosition(*rWindow);
	sf::Vector2f mWorldPos = rWindow->mapPixelToCoords(mScreenPos, rWindow->getView());
	return Eigen::Vector2f(mWorldPos.x, mWorldPos.y);
}

Eigen::Vector2f PlayerController::GetLeftStickVect()
{
	//XBOX360 controller
	//"X axis" is the non inverted X axis of the left stick
	//"Y axis" is the inverted Y axis of the left stick
	//"Z axis" goes roughly to +100 when the left trigger is pulled, and to roughly -100 when the right trigger is pulled
	//"R axis" doesnt seem to register on this controller
	//"U axis" is the non inverted X axis of the right stick
	//"V axis"  is the inverted Y axis of the right stick
	//std::cout << "position: " << sf::Joystick::isButtonPressed(0, sf::Joystick::) << "\n";
	Vector2f wasdDir(0.0f, 0.0f);
	//Get values from joystick, account for shitty controller drift
	float xDir = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
	xDir = std::abs(xDir) > 50.0f ? xDir : 0.0f;
	float yDir = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
	yDir = std::abs(yDir) > 50.0f ? yDir : 0.0f;
	wasdDir = Vector2f(xDir, yDir);
	wasdDir.normalize();
	return wasdDir;
}

Eigen::Vector2f PlayerController::GetRightStickVect()
{
	//XBOX360 controller
	//"X axis" is the non inverted X axis of the left stick
	//"Y axis" is the inverted Y axis of the left stick
	//"Z axis" goes roughly to +100 when the left trigger is pulled, and to roughly -100 when the right trigger is pulled
	//"R axis" doesnt seem to register on this controller
	//"U axis" is the non inverted X axis of the right stick
	//"V axis"  is the inverted Y axis of the right stick
	Vector2f wasdDir(0.0f, 0.0f);
	//Get values from joystick, account for shitty controller drift, had to turn drift thing down to basically nothing, otherwise it really hurts accuracy
	float xDir = sf::Joystick::getAxisPosition(0, sf::Joystick::U);
	xDir = std::abs(xDir) > 1.0f ? xDir : 0.0f;
	float yDir = sf::Joystick::getAxisPosition(0, sf::Joystick::V);
	yDir = std::abs(yDir) > 1.0f ? yDir : 0.0f;
	wasdDir = Vector2f(xDir, yDir);
	wasdDir.normalize();
	return wasdDir;
}

Eigen::Vector2f PlayerController::GetMovementDir()
{
	Vector2f currThrustDirVect(0.0f, 0.0f);
	bool connected = sf::Joystick::isConnected(0);
	if (connected) {
		currThrustDirVect = GetLeftStickVect();
	}
	else {
		currThrustDirVect = PollWASDKeys();
	}
	return currThrustDirVect;
}

Eigen::Vector2f PlayerController::GetAimDir()
{
	return Eigen::Vector2f();
}

bool PlayerController::IsRightTriggerPressed()
{
	//"Z axis" goes roughly to +100 when the left trigger is pulled, and to roughly -100 when the right trigger is pulled
	if (sf::Joystick::getAxisPosition(0, sf::Joystick::Z) < -75.0f) {
		return true;
	}
	else {
		return false;
	}
}

bool PlayerController::IsLeftTriggerPressed()
{
	//"Z axis" goes roughly to +100 when the left trigger is pulled, and to roughly -100 when the right trigger is pulled
	if (sf::Joystick::getAxisPosition(0, sf::Joystick::Z) > 75.0f) {
		return true;
	}
	else {
		return false;
	}
}

bool PlayerController::IsControllerConnected()
{
	return sf::Joystick::isConnected(0);
}

bool PlayerController::IsRightStickPressed()
{
	//std::cout << "button 9: " << sf::Joystick::isButtonPressed(0, 9) << "\n";
	return sf::Joystick::isButtonPressed(0, 9);
}

bool PlayerController::IsStartPressed()
{
	//std::cout << "button 7: " << sf::Joystick::isButtonPressed(0, 7) << "\n";
	bool connected = sf::Joystick::isConnected(0);
	if (connected) {
		return sf::Joystick::isButtonPressed(0, 7);
	}
	else {
		return false;
	}
}

bool PlayerController::ScrollClick()
{
	if ((sf::Mouse::isButtonPressed(sf::Mouse::Middle)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))) {
		return true;
	}
	else {
		return false;
	}
}