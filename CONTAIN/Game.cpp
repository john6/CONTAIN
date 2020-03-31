#include "Game.h"

Game::Game(RESOURCES* i_resources, DIFFICULTY i_difficulty)
	: resources {i_resources} , HUD {HeadsUpDisplay(i_resources)}
{
	beginTime = std::chrono::high_resolution_clock::now();
	font = resources->GetFont();
	numLvls = 3;
	currLvl = 0;
}

 
Game::~Game()
{
}

GAME_STATE Game::Update(float microSeconds) {
	timeElapsed = std::chrono::duration_cast<std::chrono::seconds>(hiResTime::now() - beginTime);
	float millisecLag = abs(microSeconds / MICROSECONDS_TO_MILLISECONDS);
	PollKeys(millisecLag);
	if (timeElapsed.count() >= timeToComplete) {
		return LOSE;
	}
	switch (playState) {
		case (GENERAL_GAMEPLAY): {
			return UpdateGeneral(millisecLag);
			break;
		}
		default: {
			return MENU;
			break;
		}
	}
	UpdateHUD();
}

GAME_STATE Game::UpdateGeneral(float stepSize) {
	//player movement
	//other object movement
	//collisions
	//BreakObject Bounce
	//delete objects
	return IN_GAME;
}

void Game::UpdateHUD() {

}

void Game::PollKeys(float stepSize) {
	float paddleStep = gameSpeedPerMill * stepSize;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
	}
}

void Game::Render(sf::RenderWindow* window) {
	//SHOULD TRANFER ALL ENTITES TO "Render.cpp"
}

void Game::GenerateLevels(DIFFICULTY i_diff) {
	DeleteLevels();
	Level* lvlOne = new Level(0, i_diff);
	Level* lvlTwo = new Level(0, i_diff);
	Level* lvlThree = new Level(0, i_diff);
	levels.insert(levels.begin(), lvlOne);
	levels.insert(levels.begin() + 1, lvlTwo);
	levels.insert(levels.begin() + 2, lvlThree);
}

void Game::DeleteLevels() {
	for (int i = 0; i < numLvls; i++) {
		delete &levels[i];
	}
	levels.clear();
}

const int Game::GetCurrLvl() { return currLvl;  }
const int Game::GetNumLvls() { return numLvls;  }