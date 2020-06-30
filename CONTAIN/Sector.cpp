#include "Sector.h"

Sector::Sector(std::shared_ptr<Entity> i_charPtr) : 
	playerChar {i_charPtr}
{
	sectEnemyNum = 0;
}


Sector::~Sector()
{
}

std::list<std::shared_ptr<Entity>>* Sector::GetSectorEntities()
{
	return &lvlEntities;
}

void Sector::AddEntPtrToSector(std::shared_ptr<Entity> i_entPtr)
{
	lvlEntities.push_back(i_entPtr);
}

void Sector::GenerateLevelCubes(int i_numCubes)
{
	float offset = 60.0f;
	std::vector<Vector2f> quadrantCoords;
	quadrantCoords.push_back(Vector2f(100.0f, 100.0f));
	quadrantCoords.push_back(Vector2f(SCREEN_WIDTH / 2.0f, 0.0f));
	quadrantCoords.push_back(Vector2f(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f));
	quadrantCoords.push_back(Vector2f(0.0f, SCREEN_HEIGHT / 2.0f));
	for (Vector2f quadrant : quadrantCoords) {
		for (int i = 0; i < i_numCubes; ++i) {
			std::shared_ptr<Shape> shape = std::make_shared<Rectangle>(50, 50);
			Material HeavyBall = Material(0.9f, 0.95f, 0.5f, 0.25f);
			RigidBody projBody = RigidBody(shape, HeavyBall);
			std::shared_ptr<Entity> ent = std::make_shared<Enemy>(projBody,
				Vector2f(quadrant(0) + offset + ((i % (4)) * 250), quadrant(1) + offset + ((i / (4)) * 180)), playerChar, this);
			lvlEntities.push_back(ent);
			sectEnemyNum += 1;
		}
	}
}

void Sector::GenerateLevelCircles(int i_numCircs)
{
	for (int i = 0; i < i_numCircs; ++i) {
		std::shared_ptr<Shape> projectileShape = std::make_shared<Circle>(40);
		Material HeavyBall = Material(0.9f, 0.95f, 0.5f, 0.25f);
		RigidBody projBody = RigidBody(projectileShape, HeavyBall);
		std::shared_ptr<Entity> ent = std::make_shared<Enemy>(projBody,
			Vector2f(HOR_MARGIN + i * (COURT_WIDTH / i_numCircs), VERT_MARGIN + i * (COURT_HEIGHT / i_numCircs)), playerChar, this);
		lvlEntities.push_back(ent);
		sectEnemyNum += 1;
	}
}

void Sector::AddWallsToLevel()
{
	Material Static = Material(0.0f, 0.4f, 0.4f, 0.2f);

	std::shared_ptr<Shape> vertRect2 = std::make_shared<Rectangle>(HOR_MARGIN, COURT_HEIGHT);
	RigidBody leftWallBody = RigidBody(vertRect2, Static);
	std::shared_ptr<Entity> leftWall = std::make_shared<Wall>(leftWallBody,
		Vector2f((HOR_MARGIN / 2.0f), VERT_MARGIN + (COURT_HEIGHT / 2.0f)), this);
	AddEntPtrToSector(leftWall);

	std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(COURT_WIDTH, VERT_MARGIN);
	RigidBody wallBody1 = RigidBody(horRect1, Static);
	std::shared_ptr<Entity> upperWall = std::make_shared<Wall>(wallBody1,
		Vector2f(HOR_MARGIN + (COURT_WIDTH / 2.0f), (VERT_MARGIN / 2.0f)), this);
	lvlEntities.push_back(upperWall);

	std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(HOR_MARGIN, COURT_HEIGHT);
	RigidBody rightWallBody = RigidBody(vertRect1, Static);
	std::shared_ptr<Entity> rightWall = std::make_shared<Wall>(rightWallBody,
		Vector2f(HOR_MARGIN + (COURT_WIDTH)+(HOR_MARGIN / 2.0f), VERT_MARGIN + (COURT_HEIGHT / 2.0f)), this);
	lvlEntities.push_back(rightWall);

	std::shared_ptr<Shape> horRect2 = std::make_shared<Rectangle>(COURT_WIDTH, VERT_MARGIN);
	RigidBody lowerWallBody = RigidBody(horRect2, Static);
	std::shared_ptr<Entity> lowerWall = std::make_shared<Wall>(lowerWallBody,
		Vector2f(Vector2f(HOR_MARGIN + (COURT_WIDTH / 2.0f), (WALL_THICKNESS / 2.0f) + COURT_HEIGHT)), this);
	lvlEntities.push_back(lowerWall);

}

void Sector::RemoveDestroyedEntities() {
	std::list<std::shared_ptr<Entity>>::iterator iter = lvlEntities.begin();
	while (iter != lvlEntities.end()) {
		if (iter._Ptr->_Myval->MarkedForDeath()) {
			lvlEntities.erase(iter++);
		}
		else { ++iter; }
	}
}