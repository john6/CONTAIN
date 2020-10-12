#include "Game.h"
#include "TuteLib.h"

Game::Game(sf::RenderWindow* i_window, RESOURCES* i_resources, DIFFICULTY i_difficulty)
	: renderWindow{ i_window }, resources {i_resources}, tuteLib{ TuteLib(i_window, i_resources)}, HUD { HeadsUpDisplay(i_resources) }
{
	font = resources->GetFont();
}

Game::~Game()
{
	DeleteLevels();
}

GAME_STATE Game::Update(float i_microSecs, sf::RenderWindow* i_window, sf::Vector2i i_mousePos) {
	if (tutorial) {
		if ((currSector.x == 5) && (currSector.y == 0) && (levels[currLvl]->GetSector(currSector)->sectEnemyNum == 0)) {
			tuteLib.PlayTutorial(TuteLib::TUTORIALS::ESCAPE);
		}
		else if ((currSector.x == 4) && (currSector.y == 0) && (levels[currLvl]->GetSector(currSector)->firstPhase == false)) {
			tuteLib.PlayTutorial(TuteLib::TUTORIALS::PUSH_ENEMIES);
		}
		else {
			tuteLib.PlayTutorial(tuteLib.GetTuteFromSect(currSector.x, currSector.y));
		}

	}
	timeElapsed = std::chrono::duration_cast<std::chrono::seconds>(hiResTime::now() - beginTime);
	float millisecLag = abs(i_microSecs / MICROSECS_TO_MILLISECS);
	float timeLeft = timeToComplete - timeElapsed.count();
	int timeLeftSeconds = timeLeft / 1000000;
	PlayerChar* playerPtr = dynamic_cast<PlayerChar*>(playerChar.get());
	HUD.Update(currLvl, levels[currLvl]->GetTimeLeftInLevel(), levels[currLvl]->timeToComplete, playerPtr);
	switch (playState) {
		case (GENERAL_GAMEPLAY): {
			return UpdateGeneral(millisecLag, i_mousePos);
			break;
		}
		case (WON_LEVEL): {
			UPGRADE_TYPE upgradeResult = currUpgradeMenu->Update(i_microSecs, i_window, i_mousePos);
			if (upgradeResult != NONE) {
				PlayerChar* playerPtr = dynamic_cast<PlayerChar*>(playerChar.get());
				playerPtr->ReceivePowerUp(upgradeResult);
				playState = GENERAL_GAMEPLAY;
				currRunScore += 5000 * (1 + gameDiff);
				currRunScore += playerPtr->GetCurrHealth() * 100 * (1 + gameDiff);
				currRunScore += levels[currLvl]->timeToComplete * 150 * (1 + gameDiff);
				currRunScore += levels[currLvl]->enemiesKilled * 25 * (1 + gameDiff);
				++currLvl;
				currSector = levels[currLvl]->originCoord;
				playerPtr->ResetHealth();
				playerPtr->ResetSpecialAmmo();
				playerPtr->rb.ResetPosition(Vector2f(
					GLBVRS::HR_MRG + GLBVRS::CRT_WDTH / 2.0f, 
					GLBVRS::VRT_MRG + GLBVRS::CRT_HGHT * (3.0f / 4.0f)));
				playerPtr->rb.angVel = 0.0f;
				playerPtr->rb.vel = Vector2f(0.0f, 0.0f);
				PlayRandomSong();
			}
			return IN_GAME;
			break;
		}
		default: {
			return MENU;
			break;
		}
	}
}

GAME_STATE Game::UpdateGeneral(float i_stepSize, sf::Vector2i i_mousePos) {
	levels[currLvl]->UpdateLevel();
	UpdateLvlEntities(levels[currLvl]->GetSector(currSector)->GetSectorEntities(), i_stepSize);
	if (playerWon) {
		return WIN;
	}
	else if (dynamic_cast<PlayerChar*>(playerChar.get())->GetCurrHealth() <= 0) {
		return LOSE;
	}
	return IN_GAME;
}

GAME_STATE  Game::UpdateLvlEntities(std::list<std::shared_ptr<Entity>>* i_lvlEnts, float i_stepSize) {
	playerChar->Update(i_stepSize);
	auto iter = i_lvlEnts->begin();
	while (iter != i_lvlEnts->end()) {
		iter._Ptr->_Myval->Update(i_stepSize);
		iter++;
	}
	concurrency::concurrent_vector<CollisionData> collisions;

	std::vector<int> parallelVect;
	for (int i = 0; i < i_lvlEnts->size(); i++) {
		parallelVect.push_back(i);
	}

	std::vector<std::shared_ptr<Entity>> entPVect;
	for (auto iter = i_lvlEnts->begin(); iter != i_lvlEnts->end(); ++iter) {
		std::shared_ptr<Entity> entPtr = iter._Ptr->_Myval;
		entPVect.push_back(entPtr);
	}
	std::for_each(std::execution::par, parallelVect.begin(), parallelVect.end(), [&](int index) {
		//This keeps going one index over what its supposed to, I must be deleting shit when Im not supposed to delete it
		//this next if statement doesnt solve root problem but its prob gonna stop crashed because of the index out of bounds
		//TODO
		if (index < entPVect.size()) {
			std::shared_ptr<Entity> entPtri = entPVect[index];
			CollisionData collisionData = CollisionData();
			collisionData.entA = playerChar;
			collisionData.entB = entPtri;
			bool collided = Physics::CheckCollision(&collisionData);
			if (collided) {
				collisions.push_back(collisionData);
			}
		}
	});

	std::for_each(std::execution::par, parallelVect.begin(), parallelVect.end(), [&](int index) {
		for (int j = index + 1; j < parallelVect.size(); ++j) {
			if (j < entPVect.size()) {
				std::shared_ptr<Entity> entPtri = entPVect[index];
				std::shared_ptr<Entity> entPtrj = entPVect[j];
				CollisionData collisionData = CollisionData();
				collisionData.entA = entPtri;
				collisionData.entB = entPtrj;
				bool collided = Physics::CheckCollision(&collisionData);
				if (collided) {
					collisions.push_back(collisionData);
				}
			}
		}
	});

	std::vector<int> parCollisions;
	for (int i = 0; i < collisions.size(); i++) {
		parCollisions.push_back(i);
	}
	std::for_each(std::execution::par, parCollisions.begin(), parCollisions.end(), [&](int index) {
		if (index < collisions.size()) {
			collisions[index].entA->CollideWith(*collisions[index].entB);
			collisions[index].entB->CollideWith(*collisions[index].entA);
			Physics::CreateCollisionImpulse(&collisions[index]);
		}
	});

	playerChar->rb.IntegrateForces();
	playerChar->rb.IntegrateVelocity(i_stepSize);
	std::cout << "i_stepSize: " << i_stepSize;

	std::for_each(std::execution::par, parallelVect.begin(), parallelVect.end(), [&](int index) {
		if (index < entPVect.size()) {
			entPVect[index]->rb.IntegrateForces();
			entPVect[index]->rb.IntegrateVelocity(i_stepSize);
		}
	});
	std::for_each(std::execution::par, parCollisions.begin(), parCollisions.end(), [&](int index) {
		if (index < parCollisions.size()) {
			Physics::PositionalCorrection(&collisions[index]);
		}
	});
	levels[currLvl]->GetSector(currSector)->RemoveDestroyedEntities();
	return IN_GAME;
}

void Game::Render(float i_elapsedMilliseconds) {
	if (playState == GENERAL_GAMEPLAY) {
		GameRenderer::Render(renderWindow, i_elapsedMilliseconds, levels[currLvl]->GetSector(currSector)->GetSectorEntities(),
			playerChar.get(), HUD.GetDrawables());
	}
	else if (playState == WON_LEVEL) {
		currUpgradeMenu->Render(renderWindow);
	}
}

void Game::TestCollision(std::shared_ptr<Entity> entA, std::shared_ptr<Entity> entB, std::vector<CollisionData>* collisionList)
{
	CollisionData collisionData = CollisionData();
	collisionData.entA = entA;
	collisionData.entB = entB;
	bool collided = Physics::CheckCollision(&collisionData);
	if (collided) { collisionList->push_back(collisionData); }
}

void Game::GenerateLevels(DIFFICULTY i_diff) {
	DeleteLevels();
	for (int i = 0; i < numLvls; ++i) {
		Level* lvl = new Level(i, i_diff);
		levels.push_back(lvl);
	}
	currSector = levels[currLvl]->originCoord;
}

void Game::GenerateTutorialLevels()
{
	DeleteLevels();
	Level* lvl = new Level();
	levels.push_back(lvl);
	currSector = levels[currLvl]->originCoord;
}

void Game::DeleteLevels() {
	int size = levels.size();
	for (int i = 0; i < size; ++i) {
		delete levels[i];
	}
	levels.clear();
}

const int Game::GetCurrLvl() 
{ return currLvl;  }

const int Game::GetNumLvls() 
{ return numLvls;  }

void Game::RequestTravelToSector(MapCoord i_destSect)
{
	currSector = i_destSect;
	levels[currLvl]->GetSector(currSector)->Awaken();
}

void Game::CreatePlayerChar()
{
	int startingHealth = 7 * (3 - gameDiff);
	switch (gameDiff) {
	case EASY: {
		startingHealth = 20;
		break;
	}
	case MEDIUM: {
		startingHealth = 16;
		break;
	}
	case HARD: {
		startingHealth = 12;
		break;
	}
	case TUTORIAL: {
		startingHealth = 999;
		break;
	}
	}
	RigidBody rb(Physics::CreateRegularPolygon(6, 75.0f * GLBVRS::SIZE_RAT), METAL);
	playerChar = std::make_shared<PlayerChar>(startingHealth, Vector2f(400.0f * GLBVRS::SIZE_RAT, 400.0f * GLBVRS::SIZE_RAT), rb);
	playerChar->rb.transform.orient = 1.0f;
	GLBVRS::SetGlobalConstants(renderWindow->getSize().x, renderWindow->getSize().y, resources, this, playerChar, resources->soundLvl);
}

void Game::SpawnProjectile()
{
}

void Game::RequestGoToNextLvl()
{
	if (currLvl < numLvls - 1) {
		resources->PlaySound(RESOURCES::SOUNDS::MENUACCEPT3);
		playState = WON_LEVEL;
		currUpgradeMenu = std::make_shared<UpgradeMenu>(resources, gameDiff, dynamic_cast<PlayerChar*>(playerChar.get()));
	}
	else {
		if (tutorial) {
			tuteLib.PlayTutorial(TuteLib::TUTORIALS::TUTORIAL_END);
		}
		playerWon = true;
	}
}

void Game::InitGame(DIFFICULTY i_diff)
{
	gameDiff = i_diff;
	tutorial = false;
	beginTime = std::chrono::high_resolution_clock::now();
	numLvls = 5;
	currLvl = 0;
	currRunScore = 5;
	timeToComplete = 999999999.0f;
	playState = GENERAL_GAMEPLAY;
	const microSec UPDATE_INTERVAL(16666);
	playerWon = false;
	CreatePlayerChar();
	GenerateLevels(i_diff);
	PlayRandomSong();
}

void Game::InitTutorial()
{
	tutorial = true;
	gameDiff = TUTORIAL;
	beginTime = std::chrono::high_resolution_clock::now();
	numLvls = 1;
	currLvl = 0;
	currRunScore = 2;
	timeToComplete = 999999999.0f;
	playState = GENERAL_GAMEPLAY;
	const microSec UPDATE_INTERVAL(16666);
	playerWon = false;
	CreatePlayerChar();
	GenerateTutorialLevels();
	PlayRandomSong();
}

void Game::loadTestLevel()
{
	beginTime = std::chrono::high_resolution_clock::now();
	numLvls = 1;
	currLvl = 0;
	timeToComplete = 999999999.0f;
	playState = GENERAL_GAMEPLAY;
	const microSec UPDATE_INTERVAL(16666);
	playerWon = false;
	//generate levels test version
	std::string str = "Test";
	CreatePlayerChar();
	Level* lvl = new Level(str);
	levels.push_back(lvl);
	currSector = levels[currLvl]->originCoord;
}

void Game::PlayRandomSong()
{
	std::random_device rd1;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen1(rd1()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> distrib(0, 8); //both boundaries are inclusive
	int randExtra = distrib(gen1);
	resources->PlayMusicFromFile(randExtra);
}