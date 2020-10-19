#pragma once
#include "Enemy.h"
#include "Entity.h"

class Sector;

class CrazyBoi :
	public Enemy
{
private:
	Vector2f currDir;
	float sameDirTime;
	float timeTillDirSwitch;

public:
	CrazyBoi(DIFFICULTY i_diff, Vector2f i_startPosition, RigidBody i_rb);
	~CrazyBoi();

	void Update(float i_stepSize) override;

	void CollideWithPainWall(PainWall * i_painWallPtr) override;

	void SetDiffVars(int i_diff) override;
};