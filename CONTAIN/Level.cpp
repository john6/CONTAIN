#include "Level.h"

Level::Level(int i_lvlNum, DIFFICULTY i_diff)
{
	switch (i_diff) {
		case EASY: {
			gameSpeedPerMill = SPEED_PER_MILLISECOND_EASY;
			playerSpeed = PLAYER_SPEED_PER_MILLISECOND_EASY;
			timeToComplete = SECONDS_TO_WIN_EASY;
			break;
		}
		case MEDIUM: {
			gameSpeedPerMill = SPEED_PER_MILLISECOND_MEDIUM;
			playerSpeed = PLAYER_SPEED_PER_MILLISECOND_MEDIUM;
			timeToComplete = SECONDS_TO_WIN_MEDIUM;
			break;
		}
		case HARD: {
			gameSpeedPerMill = SPEED_PER_MILLISECOND_HARD;
			playerSpeed = PLAYER_SPEED_PER_MILLISECOND_HARD;
			timeToComplete = SECONDS_TO_WIN_HARD;
			break;
		}
	}
	switch (i_lvlNum) {
		case 0: {
			//std::shared_ptr<Circle> circlePtr1 = std::make_shared<Circle>(40.0f);
			//Entity entity1(circlePtr1);
			//entity1.GetTransform()->SetPosition(Vector2f(100, 50));
			//entity1.SetVelocity(Vector2f(1.0f, 0));
			//lvlEntites.push_back(entity1);





			//std::shared_ptr<Circle> circlePtr2 = std::make_shared<Circle>(200.0f);
			//Entity entity2(circlePtr2);
			//entity2.GetTransform()->SetPosition(Vector2f(300, 300));
			//entity2.SetVelocity(Vector2f(30.0f, 30.0f));
			//lvlEntites.push_back(entity2);


			std::shared_ptr<Rectangle> rectPtr1 = std::make_shared<Rectangle>(600.0f, 600.0f);
			RigidBody entity3(rectPtr1);
			entity3.GetTransform()->SetPosition(Vector2f(100, 300));
			entity3.SetVelocity(Vector2f(2, 0.0f));
			lvlEntites.push_back(entity3);


			std::shared_ptr<Rectangle> rectPtr6 = std::make_shared<Rectangle>(200.0f, 300.0f);
			RigidBody entity6(rectPtr6);
			entity6.GetTransform()->SetPosition(Vector2f(725.0f, 550.0f));
			//entity6.SetRotation(RADIAN_RIGHT);
			entity6.SetVelocity(Vector2f(-20.0f, -20.0f));
			//entity6.SetRotation(PI/2.50f);
			entity6.torque = PI / 60.0f;
			lvlEntites.push_back(entity6);


			//std::shared_ptr<Rectangle> rectPtr2 = std::make_shared<Rectangle>(60.0f, 60.0f);
			//Entity entity4(rectPtr2);
			//entity4.GetTransform()->SetPosition(Vector2f(400, 325));
			//entity4.SetVelocity(Vector2f(-2, 0.0f));
			//lvlEntites.push_back(entity4);

			//std::shared_ptr<Rectangle> circlePtr5 = std::make_shared<Rectangle>(200.0f, 200.0f);
			//Entity entity5(circlePtr5);
			//entity5.GetTransform()->SetPosition(Vector2f(400, 200.0f));
			////entity5.SetRotation(RADIAN_RIGHT);
			////entity5.SetVelocity(Vector2f(0.0f, 5.0f));
			////entity5.SetRotation(2.5);
			//lvlEntites.push_back(entity5);


			break;
		}
		case 1: {
			//do stuff
			break;
		}
		case 2: {
			//do stuff
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