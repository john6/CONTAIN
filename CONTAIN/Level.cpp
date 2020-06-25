#include "Level.h"
#include <stdlib.h>

Level::Level(int i_lvlNum, DIFFICULTY i_diff)
{
	Material Rock = Material(0.6f, 0.1f, 0.6f, 0.3f);
	Material Wood = Material(0.3f, 0.2f, 0.5f, 0.25f);
	Material Metal = Material(1.2f, 0.05f, 0.4f, 0.2f);
	Material BouncyBall = Material(0.3f, 0.8f, 0.7f, 0.2f);
	Material SuperBall = Material(0.3f, 0.95f, 0.5f, 0.25f);
	Material Pillow = Material(0.1f, 0.2f, 0.4f, 0.2f);
	Material Static = Material(0.0f, 0.0f, 0.4f, 0.2f);
	Material StaticVeryFriction = Material(0.0f, 0.0f, 0.9f, 0.7f);
	Material Rubber = Material(0.3f, 0.8f, 0.9f, 0.6f);
	Material Ice = Material(0.3f, 0.1f, 0.7f, 0.05f);

	switch (i_diff) {
		case EASY: {
			gameSpeedPerMill = SPEED_MULTIPLIER_EASY;
			playerSpeed = PLAYER_SPEED_MULTIPLIER_EASY;
			timeToComplete = LEVEL_TIME_LIMIT_EASY;
			break;
		}
		case MEDIUM: {
			gameSpeedPerMill = SPEED_MULTIPLIER_MEDIUM;
			playerSpeed = PLAYER_SPEED_MULTIPLIER_MEDIUM;
			timeToComplete = LEVEL_TIME_LIMIT_MEDIUM;
			break;
		}
		case HARD: {
			gameSpeedPerMill = SPEED_MULTIPLIER_HARD;
			playerSpeed = PLAYER_SPEED_MULTIPLIER_HARD;
			timeToComplete = LEVEL_TIME_LIMIT_HARD;
			break;
		}
	}
	switch (i_lvlNum) {
	case 0: {
		AddWallsToLevel();
		AddEnemyCubeToLevel(1);

		///*
		//QUADTREE:
		//	const static int MAX_OBJECTS = 2;
		//	const static int MAX_LEVELS = 10;

		//120 circles makes there be a lot of collision misses

		//QUADTREE:
		//	const static int MAX_OBJECTS = 2;
		//	const static int MAX_LEVELS = 10;

		////can have about 80 circles and everything works fine. 100 makes things lag visually and start to miss collisions
		////program can be made unresponsive at as low as 200 if you click fast enough, otherwise it works though I guess clamping is preventing a complete crash
		////should be enogh info to see if a BST works
		//break;
	}
	}
}

Level::~Level()
{
}

int Level::GetLvlNum() { return m_lvl_num; }
//std::list<Entity> Level::GetLvlBoundaries() { return lvlBoundaries; }
std::list<std::shared_ptr<Entity>>* Level::GetLvlEntites() { return &lvlEntities; }

//void Level::AddEntityToLevel(Entity i_ent)
//{
//	//I think this will copy a previously created entity which does the work twice and is inneficient
//	lvlEntites.push_back(i_ent);
//}

void Level::AddEntPtrToLevel(std::shared_ptr<Entity> i_entPtr)
{
	lvlEntities.push_back(i_entPtr);
}

void Level::AddEnemyCubeToLevel(int i_numCubes)
{
	float offset = 60.0f;
	std::vector<Vector2f> quadrantCoords;
	quadrantCoords.push_back(Vector2f(0.0f, 0.0f));
	quadrantCoords.push_back(Vector2f(SCREEN_WIDTH / 2.0f, 0.0f));
	quadrantCoords.push_back(Vector2f(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f));
	quadrantCoords.push_back(Vector2f(0.0f, SCREEN_HEIGHT / 2.0f));
	for (Vector2f quadrant : quadrantCoords) {
		for (int i = 0; i < i_numCubes; ++i) {
			std::shared_ptr<Shape> shape = std::make_shared<Rectangle>(50, 50);
			Material HeavyBall = Material(0.9f, 0.95f, 0.5f, 0.25f);
			RigidBody projBody = RigidBody(shape, HeavyBall);
			projBody.ApplyImpulse(Vector2f(2000, 2000), Vector2f(0.0f, 0.0f));
			std::shared_ptr<Entity> ent = std::make_shared<Enemy>(projBody);
			ent->rb.ResetPosition(Vector2f(quadrant(0) + offset + ((i % (4)) * 250), quadrant(1) + offset + ((i / (4)) * 180)));
			lvlEntities.push_back(ent);
		}
	}
}

void Level::AddEnemyCircleToLevel(int i_numCircs)
{
for (int i = 0; i < i_numCircs; ++i) {
	std::shared_ptr<Shape> projectileShape = std::make_shared<Circle>(40);
	Material HeavyBall = Material(0.9f, 0.95f, 0.5f, 0.25f);
	RigidBody projBody = RigidBody(projectileShape, HeavyBall);
	std::shared_ptr<Entity> ent = std::make_shared<Enemy>(projBody);
	ent->rb.ResetPosition(Vector2f(HOR_MARGIN + i * (COURT_WIDTH / i_numCircs), VERT_MARGIN + i * (COURT_HEIGHT / i_numCircs)));
	lvlEntities.push_back(ent);
}
}

void Level::AddWallsToLevel()
{
Material Static = Material(0.0f, 0.4f, 0.4f, 0.2f);

std::shared_ptr<Shape> vertRect2 = std::make_shared<Rectangle>(HOR_MARGIN, COURT_HEIGHT);
RigidBody leftWallBody = RigidBody(vertRect2, Static);
std::shared_ptr<Entity> leftWall = std::make_shared<Wall>(leftWallBody);
leftWall->rb.ResetPosition(Vector2f((HOR_MARGIN / 2.0f), VERT_MARGIN + (COURT_HEIGHT / 2.0f)));
AddEntPtrToLevel(leftWall);

std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(COURT_WIDTH, VERT_MARGIN);
RigidBody wallBody1 = RigidBody(horRect1, Static);
std::shared_ptr<Entity> upperWall = std::make_shared<Wall>(wallBody1);
upperWall->rb.ResetPosition(Vector2f(HOR_MARGIN + (COURT_WIDTH / 2.0f), (VERT_MARGIN / 2.0f)));
lvlEntities.push_back(upperWall);

std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(HOR_MARGIN, COURT_HEIGHT);
RigidBody rightWallBody = RigidBody(vertRect1, Static);
std::shared_ptr<Entity> rightWall = std::make_shared<Wall>(rightWallBody);
rightWall->rb.ResetPosition(Vector2f(HOR_MARGIN + (COURT_WIDTH) + (HOR_MARGIN / 2.0f), VERT_MARGIN + (COURT_HEIGHT / 2.0f)));
lvlEntities.push_back(rightWall);

std::shared_ptr<Shape> horRect2 = std::make_shared<Rectangle>(COURT_WIDTH, VERT_MARGIN);
RigidBody lowerWallBody = RigidBody(horRect2, Static);
std::shared_ptr<Entity> lowerWall = std::make_shared<Wall>(lowerWallBody);
lowerWall->rb.ResetPosition(Vector2f(Vector2f(HOR_MARGIN + (COURT_WIDTH / 2.0f), (WALL_THICKNESS / 2.0f) + COURT_HEIGHT)));
lvlEntities.push_back(lowerWall);

}

void Level::RemoveDestroyedEntities() {
	std::list<std::shared_ptr<Entity>>::iterator iter = lvlEntities.begin();
	while (iter != lvlEntities.end()) {
		if (iter._Ptr->_Myval->MarkedForDeath()) {
			lvlEntities.erase(iter++);
		}
		else { ++iter;  }
	}
}