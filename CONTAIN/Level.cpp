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
	Material Static = Material(0.0f, 0.4f, 0.4f, 0.2f);
	Material StaticVeryFriction = Material(0.0f, 0.1f, 0.9f, 0.7f);
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
			////IMMOBILE Circle CIRCLE
			std::shared_ptr<Circle> circleShape1 = std::make_shared<Circle>(200.0f);
			RigidBody immobileCenterCircle = RigidBody(circleShape1, Rock);
			immobileCenterCircle.transform.pos = Vector2f(COURT_WIDTH / 2, COURT_HEIGHT / 2);
			immobileCenterCircle.vel = (Vector2f(0.0f, 0.0f));
			lvlEntites.push_back(immobileCenterCircle);

			////RIGHT MOVING CIRCLE
			std::shared_ptr<Circle> circleShape3 = std::make_shared<Circle>(150.0f);
			RigidBody rightMovingCircle = RigidBody(circleShape3, Rock);
			rightMovingCircle.transform.pos = Vector2f(0.0f, COURT_HEIGHT / 2);
			rightMovingCircle.vel = (Vector2f(2.0f, 0.0f));
			rightMovingCircle.angVel = (0.1f);
			lvlEntites.push_back(rightMovingCircle);
			break;
		}
		case 1: {
			//MOBILE SQUARE CENTER
			std::shared_ptr<Rectangle> centerSquareShape = std::make_shared<Rectangle>(200.0f, 800.0f);
			RigidBody immobileCenterSquare = RigidBody(centerSquareShape, Rock);
			immobileCenterSquare.transform.pos = Vector2f(COURT_WIDTH / 2, COURT_HEIGHT / 2);
			immobileCenterSquare.transform.orient = (-0.5f);
			lvlEntites.push_back(immobileCenterSquare);

			//////LEFT MOVING SQAURE
			std::shared_ptr<Rectangle> squarePointer2 = std::make_shared<Rectangle>(100.0f, 100.0f);
			RigidBody leftMovingSquare = RigidBody(squarePointer2, Rock);
			leftMovingSquare.transform.pos = Vector2f(COURT_WIDTH, COURT_HEIGHT / 4);
			leftMovingSquare.vel = (Vector2f(-3.0f, 0.0f));
			leftMovingSquare.angVel = (-0.1f);
			lvlEntites.push_back(leftMovingSquare);
		}
		case 2: {
			//HEAVY RECTANGLE CENTER
			std::shared_ptr<Rectangle> centerSquareShape = std::make_shared<Rectangle>(200.0f, 800.0f);
			RigidBody immobileCenterSquare = RigidBody(centerSquareShape, Rock);
			immobileCenterSquare.transform.pos = Vector2f(COURT_WIDTH / 2, COURT_HEIGHT / 2);
			lvlEntites.push_back(immobileCenterSquare);

			////RIGHT MOVING CIRCLE
			std::shared_ptr<Circle> circleShape3 = std::make_shared<Circle>(150.0f);
			RigidBody rightMovingCircle = RigidBody(circleShape3, Rock);
			rightMovingCircle.transform.pos = Vector2f(0.0f, COURT_HEIGHT / 4);
			rightMovingCircle.vel = (Vector2f(3.0f, 0.0f));
			rightMovingCircle.angVel = (0.1f);
			lvlEntites.push_back(rightMovingCircle);
			break;
		}
		case 3: {
			//Item stacking

			//GRAVITY WITH FLOOR
			std::shared_ptr<Rectangle> floorRect = std::make_shared<Rectangle>(COURT_WIDTH, 300.0f);
			RigidBody floorBody = RigidBody(floorRect, Static);
			floorBody.transform.pos = Vector2f(COURT_WIDTH / 2, COURT_HEIGHT);
			lvlEntites.push_back(floorBody);

			////RIGHT MOVING CIRCLE
			std::shared_ptr<Circle> circleShape1 = std::make_shared<Circle>(150.0f);
			RigidBody fallingCircle3 = RigidBody(circleShape1, Rock);
			fallingCircle3.transform.pos = Vector2f((COURT_WIDTH / 2), COURT_HEIGHT - 1200);
			lvlEntites.push_back(fallingCircle3);

			////RIGHT MOVING CIRCLE
			std::shared_ptr<Circle> circleShape3 = std::make_shared<Circle>(150.0f);
			RigidBody fallingCircle = RigidBody(circleShape3, Rock);
			fallingCircle.transform.pos = Vector2f((COURT_WIDTH / 2) + -200, COURT_HEIGHT - 800);
			lvlEntites.push_back(fallingCircle);

			////RIGHT MOVING CIRCLE
			std::shared_ptr<Circle> circleShape4 = std::make_shared<Circle>(150.0f);
			RigidBody fallingCircle2 = RigidBody(circleShape4, Rock);
			fallingCircle2.transform.pos = Vector2f((COURT_WIDTH / 2) + 300, COURT_HEIGHT - 800);
			lvlEntites.push_back(fallingCircle2);

			////RIGHT MOVING CIRCLE
			std::shared_ptr<Circle> circleShape5 = std::make_shared<Circle>(150.0f);
			RigidBody fallingCircle5 = RigidBody(circleShape5, Rock);
			fallingCircle5.transform.pos = Vector2f((COURT_WIDTH / 2) - 100, COURT_HEIGHT - 1400);
			lvlEntites.push_back(fallingCircle5);
			break;
		}

		case 4: {
			//Rolling test

			//STATIC FLOOR
			Material AlmostStatic = Material(0.999f, 0.1f, 0.4f, 0.2f);
			std::shared_ptr<Rectangle> floorRect = std::make_shared<Rectangle>(COURT_WIDTH, 100.0f);
			RigidBody floorBody = RigidBody(floorRect, Static);
			floorBody.transform.pos = Vector2f(COURT_WIDTH / 2, COURT_HEIGHT);
			//floorBody.transform.orient = (RADIAN_UP / 2.0); //(500000.0f);
			lvlEntites.push_back(floorBody);

			////ROLLING BALL
			std::shared_ptr<Circle> circleShape1 = std::make_shared<Circle>(150.0f);
			RigidBody fallingCircle3 = RigidBody(circleShape1, Rubber);
			fallingCircle3.transform.pos = Vector2f((COURT_WIDTH / 4), COURT_HEIGHT - 200.0f);
			//fallingCircle3.vel = (Vector2f(1.0f, 0.0f));
			fallingCircle3.angVel = (0.025f);
			lvlEntites.push_back(fallingCircle3);

			break;
		}

		case 5: {
			//framerate test

			//STATIC FLOOR
			std::shared_ptr<Rectangle> floorRect = std::make_shared<Rectangle>(HOR_MARGIN*2.0f + COURT_WIDTH, 100.0f);
			RigidBody floorBody = RigidBody(floorRect, Static);
			floorBody.transform.pos = Vector2f(HOR_MARGIN + COURT_WIDTH / 2.0f, COURT_HEIGHT - 100.0);
			lvlEntites.push_back(floorBody);

			////STATIC LEFT WALL
			std::shared_ptr<Rectangle> leftWallRect = std::make_shared<Rectangle>(100.0f, COURT_HEIGHT);
			RigidBody leftWall = RigidBody(leftWallRect, Static);
			leftWall.transform.pos = Vector2f(HOR_MARGIN, VERT_MARGIN + COURT_HEIGHT / 2.0f);
			lvlEntites.push_back(leftWall);

			//STATIC RIGHT WALL
			std::shared_ptr<Rectangle> rightWallRect = std::make_shared<Rectangle>(100.0f, COURT_HEIGHT);
			RigidBody rightWall = RigidBody(rightWallRect, Static);
			rightWall.transform.pos = Vector2f(COURT_WIDTH, VERT_MARGIN + COURT_HEIGHT / 2.0f);
			lvlEntites.push_back(rightWall);

			for (int i = 0; i < 20; ++i) {
				std::shared_ptr<Circle> circleShape = std::make_shared<Circle>(50.0f);
				RigidBody circleRand = RigidBody(circleShape, Wood);
				circleRand.transform.pos = Vector2f((COURT_WIDTH / 2) + (pow(-1, i) * ((i * 1374) % 200)), COURT_HEIGHT - 200.0f - (i * 100));
				lvlEntites.push_back(circleRand);
			}

			break;
		}
		case 6: {
			//framerate test WITH RECTS

			//STATIC FLOOR
			std::shared_ptr<Rectangle> floorRect = std::make_shared<Rectangle>(HOR_MARGIN*2.0f + COURT_WIDTH, 100.0f);
			RigidBody floorBody = RigidBody(floorRect, Static);
			floorBody.transform.pos = Vector2f(HOR_MARGIN + COURT_WIDTH / 2.0f, COURT_HEIGHT - 100.0);
			lvlEntites.push_back(floorBody);

			////STATIC LEFT WALL
			std::shared_ptr<Rectangle> leftWallRect = std::make_shared<Rectangle>(100.0f, COURT_HEIGHT);
			RigidBody leftWall = RigidBody(leftWallRect, Static);
			leftWall.transform.pos = Vector2f(HOR_MARGIN, VERT_MARGIN + COURT_HEIGHT / 2.0f);
			lvlEntites.push_back(leftWall);

			//STATIC RIGHT WALL
			std::shared_ptr<Rectangle> rightWallRect = std::make_shared<Rectangle>(100.0f, COURT_HEIGHT);
			RigidBody rightWall = RigidBody(rightWallRect, Static);
			rightWall.transform.pos = Vector2f(COURT_WIDTH, VERT_MARGIN + COURT_HEIGHT / 2.0f);
			lvlEntites.push_back(rightWall);

			for (int i = 0; i < 20; ++i) {
				std::shared_ptr<Rectangle> circleShape = std::make_shared<Rectangle>(150.0f, 50.0f);
				RigidBody circleRand = RigidBody(circleShape, Wood);
				circleRand.transform.pos = Vector2f((COURT_WIDTH / 2) + (pow(-1, i) * ((i * 1374) % 200)), COURT_HEIGHT - 200.0f - (i * 100));
				//circleRand.transform.orient = rand() % static_cast<int>(RADIAN_DOWN);
				lvlEntites.push_back(circleRand);
			}
			break;
		}
		case 7: {
			//RECTANGLE STATIC BODY COLLISION TEST

			//STATIC FLOOR
			std::shared_ptr<Rectangle> floorRect = std::make_shared<Rectangle>(HOR_MARGIN*2.0f + COURT_WIDTH, 100.0f);
			RigidBody floorBody = RigidBody(floorRect, Static);
			floorBody.transform.pos = Vector2f(HOR_MARGIN + COURT_WIDTH / 2.0f, COURT_HEIGHT - 100.0);
			lvlEntites.push_back(floorBody);


			std::shared_ptr<Rectangle> rectShape = std::make_shared<Rectangle>(300.0f, 100.0f);
			RigidBody rectRand = RigidBody(rectShape, Rock);
			rectRand.transform.pos = Vector2f(COURT_WIDTH / 2, COURT_HEIGHT - 400.0f);
			//rectRand.transform.pos = Vector2f(COURT_WIDTH / 2, COURT_HEIGHT - 300.0f);
			rectRand.transform.orient = rand() % static_cast<int>(RADIAN_DOWN);
			lvlEntites.push_back(rectRand);
			break;
		}
		case 8: {
			////IMMOBILE Circle CIRCLE
			std::shared_ptr<Circle> circleShape1 = std::make_shared<Circle>(200.0f);
			RigidBody immobileCenterCircle = RigidBody(circleShape1, Rock);
			immobileCenterCircle.transform.pos = Vector2f(COURT_WIDTH / 2, COURT_HEIGHT / 2);
			immobileCenterCircle.vel = (Vector2f(0.0f, 0.0f));
			immobileCenterCircle.angVel = 0.2f;
			lvlEntites.push_back(immobileCenterCircle);
			break;
		}
	}
}

Level::~Level()
{
}

int Level::GetLvlNum() { return m_lvl_num; }
std::vector<RigidBody> Level::GetLvlBoundaries() { return lvlBoundaries; }
std::vector<RigidBody>* Level::GetLvlEntites() { return &lvlEntites; }