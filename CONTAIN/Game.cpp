#include "Game.h"

Game::Game(RESOURCES* i_resources, DIFFICULTY i_difficulty)
	: resources {i_resources} , HUD {HeadsUpDisplay(i_resources)}
{
	beginTime = std::chrono::high_resolution_clock::now();
	font = resources->GetFont();
	numLvls = 9;
	currLvl = 7;
	timeToComplete = 999999999.0f;
	playState = GENERAL_GAMEPLAY;
}

Game::~Game()
{
	DeleteLevels();
}

GAME_STATE Game::Update(float i_microSeconds) {
	timeElapsed = std::chrono::duration_cast<std::chrono::seconds>(hiResTime::now() - beginTime);
	float millisecLag = abs(i_microSeconds / MICROSECS_TO_MILLISECS);
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

GAME_STATE Game::UpdateGeneral(float i_stepSize) {
	//player movement
	//other object movement
	//collisions
	//BreakObject Bounce
	//delete objects
	//levels[currLvl]->GetLvlBoundaries();
	//levels[currLvl]->GetLvlEntites();
	UpdateLvlEntities(levels[currLvl]->GetLvlEntites(), i_stepSize);
	return IN_GAME;
}

GAME_STATE  Game::UpdateLvlEntities(std::vector<RigidBody>* i_lvlEnts, float i_stepSize) {
	std::vector<CollisionData> collisions;
	for (int i = 0; i < i_lvlEnts->size(); ++i) {
		for (int j = i + 1; j < i_lvlEnts->size(); ++j) {
			RigidBody* entPtri = &i_lvlEnts->at(i);
			RigidBody* entPtrj = &i_lvlEnts->at(j);
			CollisionData collisionData = CollisionData();
			collisionData.bodyA = &i_lvlEnts->at(i);
			collisionData.bodyB = &i_lvlEnts->at(j);
			bool collided = Physics::CheckCollision(&collisionData);
			if (collided) { collisions.push_back(collisionData); }
		}
	}

	for (CollisionData collision : collisions) {
		Physics::CreateCollisionImpulse(&collision);
	}

	for (int i = 0; i < i_lvlEnts->size(); ++i) {
		RigidBody* entPtr = &i_lvlEnts->at(i);
		entPtr->IntegrateForces();
		entPtr->IntegrateVelocity(i_stepSize);
	}

	for (CollisionData collision : collisions) {
		Physics::PositionalCorrection(&collision);
	}

	return IN_GAME;
}


void Game::UpdateHUD() {

}

void Game::PollKeys(float i_stepSize) {
	float paddleStep = gameSpeedPerMill * i_stepSize;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
	}
}

void Game::Render(sf::RenderWindow* i_window, float i_elapsedMilliseconds) {
	GameRenderer::Render(i_window, i_elapsedMilliseconds, levels[currLvl]->GetLvlEntites());
}

void Game::GenerateLevels(DIFFICULTY i_diff) {
	DeleteLevels();
	for (int i = 0; i < numLvls; ++i) {
		Level* lvl = new Level(i, i_diff);
		levels.push_back(lvl);
	}
}

void Game::DeleteLevels() {
	int size = levels.size();
	for (int i = 0; i < size; ++i) {
		delete levels[i];
	}
	levels.clear();
}

const int Game::GetCurrLvl() { return currLvl;  }
const int Game::GetNumLvls() { return numLvls;  }