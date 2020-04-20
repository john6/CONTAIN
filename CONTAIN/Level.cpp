#include "Level.h"

Level::Level(int i_lvlNum, DIFFICULTY i_diff)
{
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
			RigidBody immobileCenterCircle = RigidBody(circleShape1);
			immobileCenterCircle.massData.SetMass(5.0f);
			immobileCenterCircle.massData.SetInertia(5.0f);
			immobileCenterCircle.transform.position = Vector2f(COURT_WIDTH / 2, COURT_HEIGHT / 2);
			lvlEntites.push_back(immobileCenterCircle);

			////LEFT MOVING CIRCLE
			//std::shared_ptr<Circle> circleShape2 = std::make_shared<Circle>(150.0f);
			//RigidBody leftMovingCircle = RigidBody(circleShape2);
			//leftMovingCircle.transform.position = Vector2f(COURT_WIDTH, COURT_HEIGHT / 2);
			//leftMovingCircle.massData.SetMass(1.0f);
			//leftMovingCircle.ApplyImpulse(Vector2f(-20.0f, 0.0f));
			//lvlEntites.push_back(leftMovingCircle);

			////RIGHT MOVING CIRCLE
			std::shared_ptr<Circle> circleShape3 = std::make_shared<Circle>(150.0f);
			RigidBody rightMovingCircle = RigidBody(circleShape3);
			rightMovingCircle.transform.position = Vector2f(0.0f, COURT_HEIGHT / 2);
			rightMovingCircle.massData.SetMass(1.0f);
			rightMovingCircle.massData.SetInertia(1.0f);
			//rightMovingCircle.ApplyImpulse(Vector2f(20.0f, 0.0f));
			rightMovingCircle.SetVelocity(Vector2f(20.0f, 0.0f));
			rightMovingCircle.SetAngularVelocity(0.5f);
			lvlEntites.push_back(rightMovingCircle);

			////DOWN MOVING CIRCLE
			//std::shared_ptr<Circle> circleShape4 = std::make_shared<Circle>(150.0f);
			//RigidBody downMovingCircle = RigidBody(circleShape4);
			//downMovingCircle.transform.position = Vector2f(COURT_WIDTH / 2, 0.0f);
			//downMovingCircle.massData.SetMass(1.0f);
			//downMovingCircle.ApplyImpulse(Vector2f(0.0f, 20.0f));
			//lvlEntites.push_back(downMovingCircle);

			////UP MOVING CIRCLE
			//std::shared_ptr<Circle> circleShape5 = std::make_shared<Circle>(150.0f);
			//RigidBody upMovingCircle = RigidBody(circleShape5);
			//upMovingCircle.transform.position = Vector2f(COURT_WIDTH / 2, COURT_HEIGHT);
			//upMovingCircle.massData.SetMass(1.0f);
			//upMovingCircle.ApplyImpulse(Vector2f(0.0f, -20.0f));
			//lvlEntites.push_back(upMovingCircle);


			break;
		}
		case 1: {
			//MOBILE SQUARE CENTER
			std::shared_ptr<Rectangle> centerSquareShape = std::make_shared<Rectangle>(200.0f, 800.0f);
			RigidBody immobileCenterSquare = RigidBody(centerSquareShape);
			immobileCenterSquare.transform.position = Vector2f(COURT_WIDTH / 2, COURT_HEIGHT / 2);
			immobileCenterSquare.massData.SetMass(10.0f);
			immobileCenterSquare.massData.SetInertia(10.0f);
			//immobileCenterSquare.ApplyRotationalImpulse(0.03f);
			lvlEntites.push_back(immobileCenterSquare);

			//////LEFT MOVING SQAURE
			std::shared_ptr<Rectangle> squarePointer2 = std::make_shared<Rectangle>(100.0f, 100.0f);
			RigidBody rightMovingSquare = RigidBody(squarePointer2);
			rightMovingSquare.transform.position = Vector2f(COURT_WIDTH, COURT_HEIGHT / 4);
			rightMovingSquare.massData.SetMass(2.0f);
			rightMovingSquare.massData.SetInertia(2.0f);
			rightMovingSquare.SetVelocity(Vector2f(-20.0f, 0.0f));
			rightMovingSquare.SetAngularVelocity(-0.5f);
			//rightMovingSquare.ApplyImpulse(Vector2f(20.0f, 0.0f));
			lvlEntites.push_back(rightMovingSquare);

			////RIGHT MOVING SQAURE
			//std::shared_ptr<Rectangle> squarePointer2 = std::make_shared<Rectangle>(100.0f, 100.0f);
			//RigidBody rightMovingSquare = RigidBody(squarePointer2);
			//rightMovingSquare.transform.position = Vector2f(0.0f, COURT_HEIGHT / 4);
			//rightMovingSquare.massData.SetMass(2.0f);
			//rightMovingSquare.massData.SetInertia(2.0f);
			//rightMovingSquare.SetVelocity(Vector2f(20.0f, 0.0f));
			//rightMovingSquare.SetAngularVelocity(-0.5f);
			////rightMovingSquare.ApplyImpulse(Vector2f(20.0f, 0.0f));
			//lvlEntites.push_back(rightMovingSquare);

			////DOWN MOVING SQAURE
			//std::shared_ptr<Rectangle> squarePointer3 = std::make_shared<Rectangle>(100.0f, 100.0f);
			//RigidBody downMovingSquare = RigidBody(squarePointer3);
			//downMovingSquare.transform.position = Vector2f(COURT_WIDTH / 2, 0.0f);
			//downMovingSquare.massData.SetMass(1.0f);
			//downMovingSquare.ApplyImpulse(Vector2f(0.0f, 20.0f));
			//lvlEntites.push_back(downMovingSquare);

			//////UP MOVING SQAURE
			//std::shared_ptr<Rectangle> squarePointer4 = std::make_shared<Rectangle>(100.0f, 100.0f);
			//RigidBody upMovingSquare = RigidBody(squarePointer4);
			//upMovingSquare.transform.position = Vector2f(COURT_WIDTH / 2, COURT_HEIGHT);
			//upMovingSquare.massData.SetMass(1.0f);
			//upMovingSquare.massData.SetInertia(1.0f);
			//upMovingSquare.ApplyImpulse(Vector2f(0.0f, -20.0f));
			//lvlEntites.push_back(upMovingSquare);

			//////DIAGONAL DOWN AND RIGHT MOVING SQUARE
			//std::shared_ptr<Rectangle> squarePointer5 = std::make_shared<Rectangle>(100.0f, 100.0f);
			//RigidBody downAndRightMovingSquare = RigidBody(squarePointer5);
			//downAndRightMovingSquare.transform.position = Vector2f(0.0f, 0.0f);
			//downAndRightMovingSquare.massData.SetMass(1.0f);
			//downAndRightMovingSquare.ApplyImpulse(Vector2f(20.0f, 20.0f));
			//lvlEntites.push_back(downAndRightMovingSquare);

			//////DIAGONAL DOWN AND RIGHT MOVING SQUARE
			//std::shared_ptr<Rectangle> squarePointer6 = std::make_shared<Rectangle>(100.0f, 100.0f);
			//RigidBody upAndLeftMovingSquare = RigidBody(squarePointer6);
			//upAndLeftMovingSquare.transform.position = Vector2f(COURT_WIDTH, COURT_HEIGHT);
			//upAndLeftMovingSquare.massData.SetMass(1.0f);
			//immobileCenterSquare.massData.SetInertia(1.0f);
			////upAndLeftMovingSquare.ApplyImpulse(Vector2f(-20.0f, -20.0f));
			//lvlEntites.push_back(upAndLeftMovingSquare);
			break;
		}
		case 2: {
			//HEAVY RECTANGLE CENTER
			std::shared_ptr<Rectangle> centerSquareShape = std::make_shared<Rectangle>(200.0f, 800.0f);
			RigidBody immobileCenterSquare = RigidBody(centerSquareShape);
			immobileCenterSquare.transform.position = Vector2f(COURT_WIDTH / 2, COURT_HEIGHT / 2);
			immobileCenterSquare.massData.SetMass(10.0f);
			immobileCenterSquare.massData.SetInertia(10.0f);
			//immobileCenterSquare.ApplyRotationalImpulse(0.03f);
			lvlEntites.push_back(immobileCenterSquare);

			//////LEFT MOVING SQAURE
			//std::shared_ptr<Rectangle> squarePointer1 = std::make_shared<Rectangle>(100.0f, 100.0f);
			//RigidBody leftMovingSquare = RigidBody(squarePointer1);
			//leftMovingSquare.transform.position = Vector2f(COURT_WIDTH, COURT_HEIGHT / 2);
			//leftMovingSquare.massData.SetMass(1.0f);
			//leftMovingSquare.ApplyImpulse(Vector2f(-20.0f, 0.0f));
			//lvlEntites.push_back(leftMovingSquare);

			//////RIGHT MOVING SQAURE
			//std::shared_ptr<Rectangle> squarePointer2 = std::make_shared<Rectangle>(100.0f, 100.0f);
			//RigidBody rightMovingSquare = RigidBody(squarePointer2);
			//rightMovingSquare.transform.position = Vector2f(0.0f, COURT_HEIGHT / 2);
			//rightMovingSquare.massData.SetMass(1.0f);
			//rightMovingSquare.ApplyImpulse(Vector2f(20.0f, 0.0f));
			//lvlEntites.push_back(rightMovingSquare);

			////DOWN MOVING SQAURE
			//std::shared_ptr<Rectangle> squarePointer3 = std::make_shared<Rectangle>(100.0f, 100.0f);
			//RigidBody downMovingSquare = RigidBody(squarePointer3);
			//downMovingSquare.transform.position = Vector2f(COURT_WIDTH / 2, 0.0f);
			//downMovingSquare.massData.SetMass(1.0f);
			//downMovingSquare.ApplyImpulse(Vector2f(0.0f, 20.0f));
			//lvlEntites.push_back(downMovingSquare);

			//////UP MOVING SQAURE
			//std::shared_ptr<Rectangle> squarePointer4 = std::make_shared<Rectangle>(100.0f, 100.0f);
			//RigidBody upMovingSquare = RigidBody(squarePointer4);
			//upMovingSquare.transform.position = Vector2f(COURT_WIDTH / 2, COURT_HEIGHT);
			//upMovingSquare.massData.SetMass(1.0f);
			//upMovingSquare.massData.SetInertia(1.0f);
			//upMovingSquare.ApplyImpulse(Vector2f(0.0f, -20.0f));
			//lvlEntites.push_back(upMovingSquare);

			//////DIAGONAL DOWN AND RIGHT MOVING SQUARE
			//std::shared_ptr<Rectangle> squarePointer5 = std::make_shared<Rectangle>(100.0f, 100.0f);
			//RigidBody downAndRightMovingSquare = RigidBody(squarePointer5);
			//downAndRightMovingSquare.transform.position = Vector2f(0.0f, 0.0f);
			//downAndRightMovingSquare.massData.SetMass(1.0f);
			//downAndRightMovingSquare.ApplyImpulse(Vector2f(20.0f, 20.0f));
			//lvlEntites.push_back(downAndRightMovingSquare);

			//////DIAGONAL DOWN AND RIGHT MOVING SQUARE
			//std::shared_ptr<Rectangle> squarePointer6 = std::make_shared<Rectangle>(100.0f, 100.0f);
			//RigidBody upAndLeftMovingSquare = RigidBody(squarePointer6);
			//upAndLeftMovingSquare.transform.position = Vector2f(COURT_WIDTH, COURT_HEIGHT);
			//upAndLeftMovingSquare.massData.SetMass(1.0f);
			//immobileCenterSquare.massData.SetInertia(1.0f);
			////upAndLeftMovingSquare.ApplyImpulse(Vector2f(-20.0f, -20.0f));
			//lvlEntites.push_back(upAndLeftMovingSquare);

			////RIGHT MOVING CIRCLE
			std::shared_ptr<Circle> circleShape3 = std::make_shared<Circle>(150.0f);
			RigidBody rightMovingCircle = RigidBody(circleShape3);
			rightMovingCircle.transform.position = Vector2f(0.0f, COURT_HEIGHT / 4);
			rightMovingCircle.massData.SetMass(1.0f);
			rightMovingCircle.massData.SetInertia(1.0f);
			//rightMovingCircle.ApplyImpulse(Vector2f(20.0f, 0.0f));
			rightMovingCircle.SetVelocity(Vector2f(20.0f, 0.0f));
			//rightMovingCircle.SetAngularVelocity(0.5f);
			lvlEntites.push_back(rightMovingCircle);
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