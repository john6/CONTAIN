#include "Game.h"
#include "TuteLib.h"

Game::Game(sf::RenderWindow* i_window, RESOURCES* i_resources, DIFFICULTY i_difficulty)
	: renderWindow{ i_window }, resources {i_resources}, HUD{ HeadsUpDisplay(i_resources) }, tuteLib{ TuteLib(i_window, i_resources)}
{
	font = resources->GetFont();
	//InitGame(i_difficulty);
	//loadTestLevel();
}

Game::~Game()
{
	DeleteLevels();
}

GAME_STATE Game::Update(float i_microSecs, sf::RenderWindow* i_window, sf::Vector2i i_mousePos) {

	//InfoPopUp begin = InfoPopUp(resources, GLBVRS::CRT_WDTH, GLBVRS::CRT_HGHT);
	//begin.AddTextBox("Get started bucko");
	//begin.RenderAndWait(i_window);
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
	//renderWindow->clear();
	//renderWindow->display();


	timeElapsed = std::chrono::duration_cast<std::chrono::seconds>(hiResTime::now() - beginTime);
	float millisecLag = abs(i_microSecs / MICROSECS_TO_MILLISECS);
	float timeLeft = timeToComplete - timeElapsed.count();
	int timeLeftSeconds = timeLeft / 1000000;
	HUD.Update(currLvl, levels[currLvl]->GetTimeLeftInLevel(), levels[currLvl]->timeToComplete, playerChar);
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
				++currLvl;
				currSector = levels[currLvl]->originCoord;
				currRunScore += LEVEL_SCORE_INCREASE;
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
		std::shared_ptr<Entity> entPtri = entPVect[index];
		CollisionData collisionData = CollisionData();
		collisionData.entA = playerChar;
		collisionData.entB = entPtri;
		bool collided = Physics::CheckCollision(&collisionData);
		if (collided) {
			collisions.push_back(collisionData); }
	});
	std::for_each(std::execution::par, parallelVect.begin(), parallelVect.end(), [&](int index) {
		for (int j = index+1; j<parallelVect.size(); ++j) {
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
	});

	std::vector<int> parCollisions;
	for (int i = 0; i < collisions.size(); i++) {
		parCollisions.push_back(i);
	}

	std::for_each(std::execution::par, parCollisions.begin(), parCollisions.end(), [&](int index) {
		collisions[index].entA->CollideWith(*collisions[index].entB);
		collisions[index].entB->CollideWith(*collisions[index].entA);
		Physics::CreateCollisionImpulse(&collisions[index]);
	});

	playerChar->rb.IntegrateForces();
	playerChar->rb.IntegrateVelocity(i_stepSize);
	std::cout << "i_stepSize: " << i_stepSize;

	std::for_each(std::execution::par, parallelVect.begin(), parallelVect.end(), [&](int index) {
		entPVect[index]->rb.IntegrateForces();
		entPVect[index]->rb.IntegrateVelocity(i_stepSize);
	});

	std::for_each(std::execution::par, parCollisions.begin(), parCollisions.end(), [&](int index) {
		Physics::PositionalCorrection(&collisions[index]);
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
		Level* lvl = new Level(i, i_diff, playerChar, resources);
		levels.push_back(lvl);
	}
	currSector = levels[currLvl]->originCoord;
}

void Game::GenerateTutorialLevels()
{
	DeleteLevels();
	Level* lvl = new Level(playerChar, resources);
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

const int Game::GetCurrLvl() { return currLvl;  }
const int Game::GetNumLvls() { return numLvls;  }

void Game::RequestTravelToSector(MapCoord i_destSect)
{
	currSector = i_destSect;
}

void Game::CreatePlayerChar()
{
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
	tutorial = false;
	gameDiff = i_diff;
	int startingHealth = 7 * (3 - i_diff);

	switch (i_diff) {
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
	}
	playerChar = std::make_shared<PlayerChar>(this, startingHealth, Vector2f(400, 400.0));
	playerChar->rb.transform.orient = 1.0f;
	beginTime = std::chrono::high_resolution_clock::now();
	numLvls = 5;
	currLvl = 0;
	currRunScore = 5;
	timeToComplete = 999999999.0f;
	playState = GENERAL_GAMEPLAY;
	const microSec UPDATE_INTERVAL(16666);
	playerWon = false;
	GenerateLevels(i_diff);
	PlayRandomSong();
}

void Game::InitTutorial()
{
	tutorial = true;
	gameDiff = EASY;
	int startingHealth = 999999; //6 * (3 - i_diff);
	playerChar = std::make_shared<PlayerChar>(this, startingHealth, Vector2f(400, 400.0));
	PlayerChar* playerPtr = dynamic_cast<PlayerChar*>(playerChar.get());
	playerPtr->maxSpecialAmmo = 999;
	playerPtr->ResetSpecialAmmo();
	playerChar->rb.transform.orient = 1.0f;
	beginTime = std::chrono::high_resolution_clock::now();
	numLvls = 1;
	currLvl = 0;
	currRunScore = 2;
	timeToComplete = 999999999.0f;
	playState = GENERAL_GAMEPLAY;
	const microSec UPDATE_INTERVAL(16666);
	playerWon = false;
	GenerateTutorialLevels();
	PlayRandomSong();
}

void Game::loadTestLevel()
{
	//custom player
	std::shared_ptr<Shape> shape = std::make_shared<Circle>(50.0f);
	Material Metal = Material(1.2f, 0.05f, 0.4f, 0.2f);
	RigidBody rb(shape, Metal);
	int startingHealth = 999999;
	playerChar = std::make_shared<PlayerChar>(PlayerChar(this, startingHealth, Vector2f(GLBVRS::HR_MRG * 10, GLBVRS::VRT_MRG * 2 + 70)));
	//Default player
	//playerChar = std::make_shared<PlayerChar>(PlayerChar(this, startingHealth, Vector2f(200, 200.0)));
	playerChar->rb.transform.orient = 1.0f;
	//playerChar->rb.ApplyImpulse(Vector2f(0.0f, 160000.0f), Vector2f(0.0f, 0.0f));
	beginTime = std::chrono::high_resolution_clock::now();
	numLvls = 5;
	currLvl = 0;
	timeToComplete = 999999999.0f;
	playState = GENERAL_GAMEPLAY;
	const microSec UPDATE_INTERVAL(16666);
	playerWon = false;
	//generate levels test version
	DeleteLevels();
	for (int i = 0; i < numLvls; ++i) {
		std::string str = "Test";
		Level* lvl = new Level(str, playerChar ,resources);
		levels.push_back(lvl);
	}
	currSector = levels[currLvl]->originCoord;
	//levels[currLvl]->GetSector(currSector)->AddTerrain(0);
	//levels[currLvl]->GetSector(currSector)->AddTerrain(1);
	//levels[currLvl]->GetSector(currSector)->AddTerrain(2);
	//levels[currLvl]->GetSector(currSector)->AddTerrain(3);
	//levels[currLvl]->GetSector(currSector)->AddTerrain(4);
	//levels[currLvl]->GetSector(currSector)->AddTerrain(5);
	//levels[currLvl]->GetSector(currSector)->AddTerrain(6);
	//levels[currLvl]->GetSector(currSector)->AddTerrain(7);


	std::vector<Vector2f> vector;
	//counterclockwise
	vector.push_back(Vector2f(-50.0, -50.0));
	vector.push_back(Vector2f(-350.0, 30.0));
	vector.push_back(Vector2f(-200.0, 50.0));
	vector.push_back(Vector2f(0.0, 50.0));
	vector.push_back(Vector2f(50.0, -50.0));
	std::shared_ptr<Shape> shape1 = std::make_shared<Polygon>(vector);
	RigidBody rb1 = RigidBody(shape1, METAL);
	std::shared_ptr<Entity> asd5 = std::make_shared<Wall>(
		Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH / 2.0f), GLBVRS::CRT_HGHT / 2.0)),
		levels[currLvl]->GetSector(currSector).get(), rb1);
	levels[currLvl]->GetSector(currSector)->AddEntPtrToSector(asd5);





	std::vector<Vector2f> vector2;
	vector2.push_back(Vector2f(-50.0, -50.0));
	vector2.push_back(Vector2f(-50.0, 50.0));
	vector2.push_back(Vector2f(50.0, 50.0));
	vector2.push_back(Vector2f(50.0, -50.0));

	std::shared_ptr<Shape> shape2 = std::make_shared<Polygon>(vector2);
	RigidBody rb2 = RigidBody(shape2, METAL);
	std::shared_ptr<Entity> asd6 = std::make_shared<Wall>(
		Vector2f(GLBVRS::HR_MRG * 2, GLBVRS::VRT_MRG * 2), levels[currLvl]->GetSector(currSector).get(), rb2);
	levels[currLvl]->GetSector(currSector)->AddEntPtrToSector(asd6);



	/*
	std::shared_ptr<Entity> asd67 = std::make_shared<Wall>(
		Vector2f(GLBVRS::HR_MRG * 2, GLBVRS::VRT_MRG * 2 + 70), levels[currLvl]->GetSector(currSector).get(), RigidBody(std::make_shared<Rectangle>(100.0f, 100.0f), METAL));
	asd67->rb.transform.orient = 1.0f;
	levels[currLvl]->GetSector(currSector)->AddEntPtrToSector(asd67);


*/
	//playerChar = std::make_shared<Blocker>(PlayerChar(this, startingHealth, Vector2f(GLBVRS::HR_MRG * 2, GLBVRS::VRT_MRG * 2 + 70)));
	////Default player
	////playerChar = std::make_shared<PlayerChar>(PlayerChar(this, startingHealth, Vector2f(200, 200.0)));
	//playerChar->rb.transform.orient = 1.0f;



	std::vector<Vector2f> vector3;
	//counterclockwise
	vector3.push_back(Vector2f(-50.0, -50.0));
	vector3.push_back(Vector2f(-50.0, 50.0));
	vector3.push_back(Vector2f(200.0, 50.0));
	vector3.push_back(Vector2f(200.0, -50.0));



	std::shared_ptr<Shape> shape23 = std::make_shared<Polygon>(vector3);
	RigidBody rb3 = RigidBody(shape23, METAL);
	std::shared_ptr<Entity> asd61 = std::make_shared<Wall>(
		Vector2f(Vector2f(GLBVRS::HR_MRG + (GLBVRS::CRT_WDTH / 2.0f), GLBVRS::CRT_HGHT / 2.0)), levels[currLvl]->GetSector(currSector).get(), rb3);
	levels[currLvl]->GetSector(currSector)->AddEntPtrToSector(asd61);
}

void Game::PlayRandomSong()
{
	std::random_device rd1;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen1(rd1()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> distrib(1, 6); //both boundaries are inclusive
	int randExtra = distrib(gen1);
	resources->PlayMusicFromFile(randExtra);
	//resources->PlayMusicFromFile(1);
}