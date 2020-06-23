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
		std::shared_ptr<Shape> vertRect2 = std::make_shared<Rectangle>(HOR_MARGIN, COURT_HEIGHT);
		RigidBody leftWallBody = RigidBody(vertRect2, Static);
		Entity leftWall = Entity(leftWallBody);
		leftWall.rb.ResetPosition(Vector2f((HOR_MARGIN / 2.0f), VERT_MARGIN + (COURT_HEIGHT / 2.0f)));
		lvlEntites.push_back(leftWall);

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




		int numCubes = 5;
		float offset = 60.0f;
		std::vector<Vector2f> quadrantCoords;
		quadrantCoords.push_back(Vector2f(0.0f, 0.0f));
		quadrantCoords.push_back(Vector2f(SCREEN_WIDTH / 2.0f, 0.0f));
		quadrantCoords.push_back(Vector2f(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f));
		quadrantCoords.push_back(Vector2f(0.0f, SCREEN_HEIGHT / 2.0f));

		for (Vector2f quadrant : quadrantCoords) {
			for (int i = 0; i < numCubes; ++i) {

				std::shared_ptr<Shape> shape = std::make_shared<Rectangle>(50, 50);
				Material HeavyBall = Material(0.9f, 0.95f, 0.5f, 0.25f);
				RigidBody projBody = RigidBody(shape, HeavyBall);
				projBody.ApplyImpulse(Vector2f(2000, 2000), Vector2f(0.0f, 0.0f));
				Entity ent = Entity(projBody);
				ent.rb.ResetPosition(Vector2f(quadrant(0) + offset + ((i % (4)) * 250), quadrant(1) + offset + ((i / (4)) * 180)));
				lvlEntites.push_back(ent);
			}
		}


		/*
		QUADTREE:
			const static int MAX_OBJECTS = 2;
			const static int MAX_LEVELS = 10;

		120 circles makes there be a lot of collision misses



		QUADTREE:
			const static int MAX_OBJECTS = 2;
			const static int MAX_LEVELS = 10;

		
		*/
		//Entity ent1 = Entity();
		//ent1.rb.ResetPosition(Vector2f(100, 100.0f));
		//lvlEntites.push_back(ent1);
		//Entity ent2 = Entity();
		//ent2.rb.ResetPosition(Vector2f(300.0f, 100.0f));
		//lvlEntites.push_back(ent2);
		//Entity ent3 = Entity();
		//ent3.rb.ResetPosition(Vector2f(100.0f, 300.0f));
		//lvlEntites.push_back(ent3);
		//Entity ent4 = Entity();
		//ent4.rb.ResetPosition(Vector2f(300.0f, 300.0f));
		//lvlEntites.push_back(ent4);
		//Entity ent5 = Entity();
		//ent5.rb.ResetPosition(Vector2f(550, 100));
		//lvlEntites.push_back(ent5);
		//Entity ent6 = Entity();
		//ent6.rb.ResetPosition(Vector2f(700, 100.0f));
		//lvlEntites.push_back(ent6);
		//Entity ent7 = Entity();
		//ent7.rb.ResetPosition(Vector2f(550, 300.0f));
		//lvlEntites.push_back(ent7);
		//Entity ent8 = Entity();
		//ent8.rb.ResetPosition(Vector2f(700, 300.0f));
		//lvlEntites.push_back(ent8);

		//Entity ent9 = Entity();
		//ent9.rb.ResetPosition(Vector2f(1000.0f, 300.0f));
		//lvlEntites.push_back(ent9);
		//Entity ent10 = Entity();
		//ent10.rb.ResetPosition(Vector2f(1000.0f, 600.0f));
		//lvlEntites.push_back(ent2);
		//Entity ent11 = Entity();
		//ent3.rb.ResetPosition(Vector2f(300.0f, 300.0f));
		//lvlEntites.push_back(ent3);
		//Entity ent12 = Entity();
		//ent4.rb.ResetPosition(Vector2f(300.0f, 600.0f));
		//lvlEntites.push_back(ent4);
		//Entity ent13 = Entity();
		//ent13.rb.ResetPosition(Vector2f(1000.0f, 500.0f));
		//lvlEntites.push_back(ent13);
		//Entity ent14 = Entity();
		//ent14.rb.ResetPosition(Vector2f(1000.0f, 800.0f));
		//lvlEntites.push_back(ent14);
		//Entity ent15 = Entity();
		//ent15.rb.ResetPosition(Vector2f(300.0f, 500.0f));
		//lvlEntites.push_back(ent15);
		//Entity ent16 = Entity();
		//ent16.rb.ResetPosition(Vector2f(300.0f, 800.0f));
		//lvlEntites.push_back(ent16);

		//Entity ent9 = Entity();
		//ent9.rb.ResetPosition(Vector2f(1000.0f, 300.0f));
		//lvlEntites.push_back(ent9);
		//Entity ent10 = Entity();
		//ent10.rb.ResetPosition(Vector2f(1000.0f, 600.0f));
		//lvlEntites.push_back(ent2);
		//Entity ent11 = Entity();
		//ent3.rb.ResetPosition(Vector2f(300.0f, 300.0f));
		//lvlEntites.push_back(ent3);
		//Entity ent12 = Entity();
		//ent4.rb.ResetPosition(Vector2f(300.0f, 600.0f));
		//lvlEntites.push_back(ent4);
		//Entity ent13 = Entity();
		//ent13.rb.ResetPosition(Vector2f(1000.0f, 500.0f));
		//lvlEntites.push_back(ent13);
		//Entity ent14 = Entity();
		//ent14.rb.ResetPosition(Vector2f(1000.0f, 800.0f));
		//lvlEntites.push_back(ent14);
		//Entity ent15 = Entity();
		//ent15.rb.ResetPosition(Vector2f(300.0f, 500.0f));
		//lvlEntites.push_back(ent15);
		//Entity ent16 = Entity();
		//ent16.rb.ResetPosition(Vector2f(300.0f, 800.0f));
		//lvlEntites.push_back(ent16);

		//int numCubes = 15;
		//for (int i = 0; i < numCubes; ++i) {
		//	Entity ent = Entity();
		//	ent.rb.ResetPosition(Vector2f(HOR_MARGIN + i * (COURT_WIDTH / numCubes), VERT_MARGIN + i * (COURT_HEIGHT / numCubes)));
		//	lvlEntites.push_back(ent);
		//}
		//Current system: 25 cubes is enough to cause collision misses
		//at high speeds it can be less than that


		int numCircles = 10;
		for (int i = 0; i < numCircles; ++i) {
			std::shared_ptr<Shape> projectileShape = std::make_shared<Circle>(40);
			Material HeavyBall = Material(0.9f, 0.95f, 0.5f, 0.25f);
			RigidBody projBody = RigidBody(projectileShape, HeavyBall);
			Entity ent = Entity(projBody);
			ent.rb.ResetPosition(Vector2f(HOR_MARGIN + i * (COURT_WIDTH / numCircles), VERT_MARGIN + i * (COURT_HEIGHT / numCircles)));
			lvlEntites.push_back(ent);
		}

		//can have about 80 circles and everything works fine. 100 makes things lag visually and start to miss collisions
		//program can be made unresponsive at as low as 200 if you click fast enough, otherwise it works though I guess clamping is preventing a complete crash
		//should be enogh info to see if a BST works
		break;
	}
	}
}

Level::~Level()
{
}

int Level::GetLvlNum() { return m_lvl_num; }
std::list<Entity> Level::GetLvlBoundaries() { return lvlBoundaries; }
std::list<Entity>* Level::GetLvlEntites() { return &lvlEntites; }

void Level::AddEntityToLevel(Entity i_ent)
{
	lvlEntites.push_back(i_ent);
}
