#include "Game.h"
#include "QuadTree.h"
#include <execution>

Game::Game(RESOURCES* i_resources, DIFFICULTY i_difficulty)
	: resources {i_resources} , HUD {HeadsUpDisplay(i_resources)}
	
{
	//std::shared_ptr<Shape> shape = std::make_shared<Rectangle>(100.0f, 100.0f);
	//RigidBody rb(shape);
	//Entity ent(rb);
	playerChar = Entity();
	playerChar.rb.ResetPosition(Vector2f(100, 500.0f));
	playerChar.rb.transform.orient = 1.0f;
	beginTime = std::chrono::high_resolution_clock::now();
	font = resources->GetFont();
	numLvls = 9;
	currLvl = 0;
	timeToComplete = 999999999.0f;
	playState = GENERAL_GAMEPLAY;
	const microSec UPDATE_INTERVAL(16666);


	hiRes_time_point lastShotFired = std::chrono::high_resolution_clock::now();
	shipRateOfFire = 1.0f;
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
	PollKeys(i_stepSize, i_mousePos);
	//other object movement
	//collisions
	//BreakObject Bounce
	//delete objects
	//levels[currLvl]->GetLvlBoundaries();
	//levels[currLvl]->GetLvlEntites();
	UpdateLvlEntities(levels[currLvl]->GetLvlEntites(), i_stepSize);
	return IN_GAME;
}

GAME_STATE  Game::UpdateLvlEntities(std::list<Entity>* i_lvlEnts, float i_stepSize) {
	/*
	CURRENT STATE OF PHYSICS UPDATE EFFICIENCY
	regular: 47-40 ms avg
	parallel: 14-18 ms avg
	quadtree only: 49-52
	parallel & quadtree: 30-25 ms avg

	I think parallel is best is because my quadtree's insert function is not paralellizable
	*/

	//std::vector<CollisionData> collisions;
	concurrency::concurrent_vector<CollisionData> collisions;

	//PARALLEL WITHOUT QUAD

	std::vector<int> idk;
	for (int i = 0; i < i_lvlEnts->size(); i++) {
		idk.push_back(i);
	}

	std::vector<RigidBody*> rbPVect;

	for (auto iter = i_lvlEnts->begin(); iter != i_lvlEnts->end(); ++iter) {
		RigidBody* rbPtri = &iter->rb;
		rbPVect.push_back(rbPtri);
	}

	std::for_each(std::execution::par, idk.begin(), idk.end(), [&](int index) {
		RigidBody* entPtri = rbPVect[index];
		CollisionData collisionData = CollisionData();
		collisionData.bodyA = &playerChar.rb;
		collisionData.bodyB = entPtri;
		bool collided = Physics::CheckCollision(&collisionData);
		if (collided) { collisions.push_back(collisionData); }
	});

	std::for_each(std::execution::par, idk.begin(), idk.end(), [&](int index) {
		for (int j = index+1; j<idk.size(); ++j) {
			RigidBody* entPtri = rbPVect[index];
			RigidBody* entPtrj = rbPVect[j];
			CollisionData collisionData = CollisionData();
			collisionData.bodyA = entPtri;
			collisionData.bodyB = entPtrj;
			bool collided = Physics::CheckCollision(&collisionData);
			if (collided) { collisions.push_back(collisionData); }
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
		Physics::CreateCollisionImpulse(&collision);
	}

	playerChar.rb.IntegrateForces();
	playerChar.rb.IntegrateVelocity(i_stepSize);

	for (auto iter = i_lvlEnts->begin(); iter != i_lvlEnts->end(); ++iter) {
		RigidBody* entPtr = &iter->rb;
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

void Game::Render(sf::RenderWindow* i_window, float i_elapsedMilliseconds) {
	QuadTree qTree = QuadTree(0, Vector2f(0.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);/*
	auto i_lvlEnts = levels[currLvl]->GetLvlEntites();
	for (auto iter1 = i_lvlEnts->begin(); iter1 != i_lvlEnts->end(); ++iter1) {
		qTree.Insert(&(*iter1));
	}*/
	GameRenderer::Render(i_window, i_elapsedMilliseconds, levels[currLvl]->GetLvlEntites(), playerChar, qTree.GetDrawableSectionLines());
}

void Game::TestCollision(RigidBody * rbA, RigidBody * rbB, std::vector<CollisionData>* collisionList)
{
	CollisionData collisionData = CollisionData();
	collisionData.bodyA = rbA;
	collisionData.bodyB = rbB;
	bool collided = Physics::CheckCollision(&collisionData);
	if (collided) { collisionList->push_back(collisionData); }
}

void Game::GenerateLevels(DIFFICULTY i_diff) {
	DeleteLevels();
	for (int i = 0; i < numLvls; ++i) {
		Level* lvl = new Level(i, i_diff);
		levels.push_back(lvl);
	}
}

void Game::PollKeys(float i_step, sf::Vector2i i_mousePos)
{
	float shipSpeed = 40 * i_step;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
		playerChar.rb.ApplyImpulse(DOWN * shipSpeed, NULL_VECTOR);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
		playerChar.rb.ApplyImpulse(LEFT * shipSpeed, NULL_VECTOR);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
		playerChar.rb.ApplyImpulse(UP * shipSpeed, NULL_VECTOR);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		playerChar.rb.ApplyImpulse(RIGHT * shipSpeed, NULL_VECTOR);
	}
	timeSinceFired = std::chrono::duration_cast<std::chrono::seconds>(hiResTime::now() - lastShotFired);
	if ((sf::Mouse::isButtonPressed(sf::Mouse::Left))  &&
		(timeSinceFired.count() >= shipRateOfFire)) {
		lastShotFired = hiResTime::now();
		Vector2f projectileDir = Vector2f(i_mousePos.x, i_mousePos.y) - playerChar.rb.transform.pos;
		projectileDir.normalize();
		std::shared_ptr<Shape> projectileShape = std::make_shared<Circle>(PROJECTILE_RADIUS);
		Material HeavyBall = Material(0.9f, 0.95f, 0.5f, 0.25f);
		RigidBody projBody = RigidBody(projectileShape, HeavyBall);
		Entity projectile = Entity(projBody);
		projectile.rb.ResetPosition(playerChar.rb.transform.pos);
		projectile.rb.AdjustPosition(projectileDir * 100.0f);
		projectile.rb.ApplyImpulse(projectileDir * 2000.0f, NULL_VECTOR);
		levels[currLvl]->AddEntityToLevel(projectile);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
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