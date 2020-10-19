#include "CrazyBoi.h"
#include "Enemy.h"
#include "Entity.h"


CrazyBoi::CrazyBoi(DIFFICULTY i_diff, Vector2f i_startPosition, RigidBody i_rb) :
	Enemy(i_diff, i_startPosition, i_rb, ENEMY_RAND)
{
	sameDirTime = 0.9;
	timeTillDirSwitch = 0.0f;
	fillColor = CHARTREUSE;
	outlineColor = sf::Color::White;
	currDir = CreateRandomDir();
	SetDiffVars(i_diff);
}

CrazyBoi::~CrazyBoi()
{
}

void CrazyBoi::Update(float i_stepSize)
{
	//std::string str = "crazyStepSize: " + std::to_string(i_stepSize);
	//std::cout << str;
	UpdateHealth(i_stepSize);
	if (stunSecs < 0) {
		if (timeTillDirSwitch < 0) {
			timeTillDirSwitch = sameDirTime;
			currDir = CreateRandomDir();
			float moveDist = speed * i_stepSize;
			rb.ApplyImpulse(currDir * moveDist, NULL_VECTOR);
		}
		else {
			float secsInUpdate = i_stepSize / 1000.0f;
			timeTillDirSwitch -= secsInUpdate;
			float moveDist = speed * i_stepSize;
			rb.ApplyImpulse(currDir * moveDist, NULL_VECTOR);
		}
	}
	else {
		float secsInUpdate = i_stepSize / 1000.0f;
		stunSecs -= secsInUpdate;
	}
}

void CrazyBoi::CollideWithPainWall(PainWall * i_painWallPtr)
{
	//sectPtr->PlaySound(RESOURCES::FIRE5);
	TakeDamage(1);
}

void CrazyBoi::SetDiffVars(int i_diff)
{
	switch (i_diff) {
	case EASY: {
		speed = GLBVRS::ENEMYSPEEDEASY;
		break;
	}
	case MEDIUM: {
		speed = GLBVRS::ENEMYSPEEDMED;
		break;
	}
	case HARD: {
		speed = GLBVRS::ENEMYSPEEDHARD;
		break;
	}
	}
	speed *= 2;
}
