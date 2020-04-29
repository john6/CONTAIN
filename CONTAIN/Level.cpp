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
		//std::shared_ptr<Shape> shape = std::make_shared<Rectangle>(100.0f, 100.0f);
		//RigidBody rb(shape);
		//Entity ent(rb);
		//lvlEntites.push_back(ent);
		std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(COURT_WIDTH, VERT_MARGIN);
		RigidBody wallBody1 = RigidBody(horRect1, Static);
		Entity upperWall = Entity(wallBody1);
		upperWall.rb.ResetPosition(Vector2f(HOR_MARGIN + (COURT_WIDTH / 2.0f), (VERT_MARGIN / 2.0f)));
		lvlEntites.push_back(upperWall);

		std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(HOR_MARGIN, COURT_HEIGHT);
		RigidBody rightWallBody = RigidBody(vertRect1, Static);
		Entity rightWall = Entity(rightWallBody);
		rightWall.rb.ResetPosition(Vector2f(HOR_MARGIN + (COURT_WIDTH) + (HOR_MARGIN / 2.0f), VERT_MARGIN + (COURT_HEIGHT / 2.0f)));
		lvlEntites.push_back(rightWall);

		std::shared_ptr<Shape> horRect2 = std::make_shared<Rectangle>(COURT_WIDTH, VERT_MARGIN);
		RigidBody lowerWallBody = RigidBody(horRect2, Static);
		Entity lowerWall = Entity(lowerWallBody);
		lowerWall.rb.ResetPosition(Vector2f(Vector2f(HOR_MARGIN + (COURT_WIDTH / 2.0f), (WALL_THICKNESS / 2.0f) + COURT_HEIGHT)));
		lvlEntites.push_back(lowerWall);

		std::shared_ptr<Shape> vertRect2 = std::make_shared<Rectangle>(HOR_MARGIN, COURT_HEIGHT);
		RigidBody leftWallBody = RigidBody(vertRect2, Static);
		Entity leftWall = Entity(leftWallBody);
		leftWall.rb.ResetPosition(Vector2f((HOR_MARGIN / 2.0f), VERT_MARGIN + (COURT_HEIGHT / 2.0f)));
		lvlEntites.push_back(leftWall);

		Entity ent1 = Entity();
		ent1.rb.ResetPosition(Vector2f(1000.0f, 300.0f));
		lvlEntites.push_back(ent1);
		Entity ent2 = Entity();
		ent2.rb.ResetPosition(Vector2f(1000.0f, 600.0f));
		lvlEntites.push_back(ent2);
		Entity ent3 = Entity();
		ent3.rb.ResetPosition(Vector2f(300.0f, 300.0f));
		lvlEntites.push_back(ent3);
		Entity ent4 = Entity();
		ent4.rb.ResetPosition(Vector2f(300.0f, 600.0f));
		lvlEntites.push_back(ent4);
		break;
	}
	}
}

Level::~Level()
{
}

int Level::GetLvlNum() { return m_lvl_num; }
std::vector<Entity> Level::GetLvlBoundaries() { return lvlBoundaries; }
std::vector<Entity>* Level::GetLvlEntites() { return &lvlEntites; }

void Level::AddEntityToLevel(Entity i_ent)
{
	lvlEntites.push_back(i_ent);
}
