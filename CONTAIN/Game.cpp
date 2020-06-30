#include "Game.h"
#include "QuadTree.h"
#include <execution>

Game::Game(sf::RenderWindow* i_window, RESOURCES* i_resources, DIFFICULTY i_difficulty)
	: renderWindow{ i_window }, resources {i_resources}, HUD{ HeadsUpDisplay(i_resources) }
{
	std::shared_ptr<Shape> shape = std::make_shared<Rectangle>(100.0f, 100.0f);
	Material Wood = Material(0.3f, 0.2f, 0.5f, 0.25f);
	RigidBody rb(shape, Wood);
	//Entity ent(rb);
	//I can not use "make_shared" with the entity type or else memory will have an issue with it
	playerChar = std::make_shared<PlayerChar>(PlayerChar(rb, Vector2f(500, 500.0f), this));
	playerChar->rb.transform.orient = 1.0f;
	beginTime = std::chrono::high_resolution_clock::now();
	font = resources->GetFont();
	numLvls = 1;
	this->currLvl = 0;
	timeToComplete = 999999999.0f;
	playState = GENERAL_GAMEPLAY;
	const microSec UPDATE_INTERVAL(16666);
}

Game::~Game()
{
	DeleteLevels();
}

GAME_STATE Game::Update(float i_microSecs, sf::RenderWindow* i_window, sf::Vector2i i_mousePos) {
	timeElapsed = std::chrono::duration_cast<std::chrono::seconds>(hiResTime::now() - beginTime);
	float millisecLag = abs(i_microSecs / MICROSECS_TO_MILLISECS);
	//PollKeys(millisecLag);
	if (timeElapsed.count() >= timeToComplete) {
		return LOSE;
	}
	switch (playState) {
		case (GENERAL_GAMEPLAY): {
			return UpdateGeneral(millisecLag, i_mousePos);
			break;
		}
		default: {
			return MENU;
			break;
		}
	}
	UpdateHUD();
}

GAME_STATE Game::UpdateGeneral(float i_stepSize, sf::Vector2i i_mousePos) {
	//PollKeys(i_stepSize, i_mousePos);

	//other object movement
	//collisions
	//BreakObject Bounce
	//delete objects
	//levels[currLvl]->GetLvlBoundaries();
	//levels[currLvl]->GetLvlEntites();

	UpdateLvlEntities(levels[currLvl]->GetSector(currSector)->GetSectorEntities(), i_stepSize);
	return IN_GAME;
}

GAME_STATE  Game::UpdateLvlEntities(std::list<std::shared_ptr<Entity>>* i_lvlEnts, float i_stepSize) {
	/*
	CURRENT STATE OF PHYSICS UPDATE EFFICIENCY
	regular: 47-40 ms avg
	parallel: 14-18 ms avg
	quadtree only: 49-52
	parallel & quadtree: 30-25 ms avg

	I think parallel is best is because my quadtree's insert function is not paralellizable
	*/


	playerChar->Update(i_stepSize);

	auto iter = i_lvlEnts->begin();
	while (iter != i_lvlEnts->end()) {
		iter._Ptr->_Myval->Update(i_stepSize);
		iter++;
	}



	//std::vector<CollisionData> collisions;
	concurrency::concurrent_vector<CollisionData> collisions;

	//PARALLEL WITHOUT QUAD

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

	/// WITHOUT QUADTREE NOT PARALLEL
/*	for (auto iter = i_lvlEnts->begin(); iter != i_lvlEnts->end(); ++iter) {
		RigidBody* entPtri = &iter->rb;
		CollisionData collisionData = CollisionData();
		collisionData.bodyA = &playerChar.rb;
		collisionData.bodyB = entPtri;
		bool collided = Physics::CheckCollision(&collisionData);
		if (collided) { collisions.push_back(collisionData); }
	}

	for (auto iter1 = i_lvlEnts->begin(); iter1 != i_lvlEnts->end(); ++iter1) {
		for (auto iter2 = std::next(iter1); iter2 != i_lvlEnts->end(); ++iter2) {
			RigidBody* entPtri = &iter1->rb;
			RigidBody* entPtrj = &iter2->rb;
			CollisionData collisionData = CollisionData();
			collisionData.bodyA = entPtri;
			collisionData.bodyB = entPtrj;
			bool collided = Physics::CheckCollision(&collisionData);
			if (collided) { collisions.push_back(collisionData); }
		}
	}
	*/

	/// WITH QUADTREE NOT PARALLEL 

/*	QuadTree qTree = QuadTree(0, Vector2f(0.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);

	for (auto iter1 = i_lvlEnts->begin(); iter1 != i_lvlEnts->end(); ++iter1) {
		qTree.Insert(&(*iter1));
	}



	std::vector<Entity*> playerCollisions = qTree.GetSectorEntities(&playerChar);

	for (Entity* collidedWith : playerCollisions) {
		RigidBody* entPtrA = &playerChar.rb;
		RigidBody* entPtrB = &collidedWith->rb;
		CollisionData collisionData = CollisionData();
		collisionData.bodyA = entPtrA;
		collisionData.bodyB = entPtrB;
		bool collided = Physics::CheckCollision(&collisionData);
		if (collided) { collisions.push_back(collisionData); }
	}

	for (auto iter1 = i_lvlEnts->begin(); iter1 != i_lvlEnts->end(); ++iter1) {
		std::vector<Entity*> collisionsWithEnt = qTree.GetSectorEntities(&(*iter1));
		for (Entity* collidedWith : collisionsWithEnt) {
			RigidBody* entPtrA = &(*iter1).rb;
			RigidBody* entPtrB = &collidedWith->rb;
			CollisionData collisionData = CollisionData();
			collisionData.bodyA = entPtrA;
			collisionData.bodyB = entPtrB;
			bool collided = Physics::CheckCollision(&collisionData);
			if (collided) { collisions.push_back(collisionData); }
		}
	}
	*/

	/// PARALLEL WITH QUADTREE

/*	std::vector<int> idk;
	for (int i = 0; i < i_lvlEnts->size(); i++) {
		idk.push_back(i);
	}

	std::vector<Entity*> entPVect;

	for (auto iter = i_lvlEnts->begin(); iter != i_lvlEnts->end(); ++iter) {
		Entity* rbPtri = &*iter;
		entPVect.push_back(rbPtri);
	}

	QuadTree qTree = QuadTree(0, Vector2f(0.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);

	//std::for_each(std::execution::par, idk.begin(), idk.end(), [&](int index) {
	//	qTree.Insert(entPVect[index]);
	//});



	for (auto iter1 = i_lvlEnts->begin(); iter1 != i_lvlEnts->end(); ++iter1) {
		qTree.Insert(&(*iter1));
	}

	std::vector<Entity*> playerCollisions = qTree.GetSectorEntities(&playerChar);

	std::for_each(std::execution::par, idk.begin(), idk.end(), [&](int index) {
		RigidBody* entPtrA = &playerChar.rb;
		RigidBody* entPtrB = &entPVect[index]->rb;
		CollisionData collisionData = CollisionData();
		collisionData.bodyA = entPtrA;
		collisionData.bodyB = entPtrB;
		bool collided = Physics::CheckCollision(&collisionData);
		if (collided) { collisions.push_back(collisionData); }
	});

	std::for_each(std::execution::par, idk.begin(), idk.end(), [&](int index) {
		std::vector<Entity*> collisionsWithEnt = qTree.GetSectorEntities(entPVect[index]);
		for (Entity* collidedWith : collisionsWithEnt) {
			RigidBody* entPtrA = &entPVect[index]->rb;
			RigidBody* entPtrB = &collidedWith->rb;
			CollisionData collisionData = CollisionData();
			collisionData.bodyA = entPtrA;
			collisionData.bodyB = entPtrB;
			bool collided = Physics::CheckCollision(&collisionData);
			if (collided) { collisions.push_back(collisionData); }
		}
	});
	*/

	///END OF QUADSTUFF

	for (CollisionData collision : collisions) {
		collision.entA->CollideWith(*collision.entB);
		collision.entB->CollideWith(*collision.entA);
		Physics::CreateCollisionImpulse(&collision);
	}

	playerChar->rb.IntegrateForces();
	playerChar->rb.IntegrateVelocity(i_stepSize);

	for (auto iter = i_lvlEnts->begin(); iter != i_lvlEnts->end(); ++iter) {
		std::shared_ptr<Entity> entPtr = iter._Ptr->_Myval;
		entPtr->rb.IntegrateForces();
		entPtr->rb.IntegrateVelocity(i_stepSize);
	}

	for (CollisionData collision : collisions) {
		Physics::PositionalCorrection(&collision);
	}

	levels[currLvl]->GetSector(currSector)->RemoveDestroyedEntities();

	return IN_GAME;
}

void Game::UpdateHUD() {

}

void Game::Render(float i_elapsedMilliseconds) {
	QuadTree qTree = QuadTree(0, Vector2f(0.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);/*
	auto i_lvlEnts = levels[currLvl]->GetLvlEntites();
	for (auto iter1 = i_lvlEnts->begin(); iter1 != i_lvlEnts->end(); ++iter1) {
		qTree.Insert(&(*iter1));
	}*/
	GameRenderer::Render(renderWindow, i_elapsedMilliseconds, levels[currLvl]->GetSector(currSector)->GetSectorEntities(),
						 playerChar.get(), qTree.GetDrawableSectionLines());
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
		//std::unique_ptr<Level> lvl = std::make_unique<Level>(i, i_diff);
		Level* lvl = new Level(i, i_diff, playerChar);
		levels.push_back(lvl);
	}

	currSector = levels[0]->origin;
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


