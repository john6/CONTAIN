#include "Sector.h"

Sector::Sector(std::shared_ptr<Entity> i_charPtr, RESOURCES* i_resources) :
	playerChar {i_charPtr}, resources { i_resources }
{
	InitializeSector();
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
	float offset = SCREEN_WIDTH / 20.0f;
	std::vector<Vector2f> cornerZones;
	float cornerZoneWidth = COURT_WIDTH / 8.0f;
	float cornerZoneHeight = COURT_HEIGHT / 8.0f;
	//array of the positions representing the top left corners of the zones in which I will regularly spawn enemies
	cornerZones.push_back(Vector2f(HOR_MARGIN, VERT_MARGIN));
	cornerZones.push_back(Vector2f(HOR_MARGIN + (COURT_WIDTH * 7.0f / 8.0f), VERT_MARGIN));
	cornerZones.push_back(Vector2f(HOR_MARGIN + (COURT_WIDTH * 7.0f / 8.0f), VERT_MARGIN + (COURT_HEIGHT * 7.0f / 8.0f)));
	cornerZones.push_back(Vector2f(HOR_MARGIN, VERT_MARGIN + (COURT_HEIGHT * 7.0f / 8.0f)));
		for (int i = 0; i < i_numCubes; ++i) {

			std::random_device rdRoomSeed;  //Will be used to obtain a seed for the random number engine
			std::mt19937 genRoomSeed(rdRoomSeed()); //Standard mersenne_twister_engine seeded with rd()
			std::discrete_distribution<> extraRoomDist({ 12, 13, 50, 13, 12 });
			int rand1 = extraRoomDist(genRoomSeed);
			int randSizeDiff = (rand1 - 2) * 15;
			int rand2 = extraRoomDist(genRoomSeed);
			int randSizeDiff2 = (rand2 - 2) * 15;

			std::shared_ptr<Shape> shape = std::make_shared<Rectangle>(60 + randSizeDiff, 60 + randSizeDiff2);
			Material Rock = Material(0.6f, 0.1f, 0.6f, 0.3f);
			RigidBody projBody = RigidBody(shape, Rock);
			int zoneIndex = i % (cornerZones.size());
			Vector2f spawnLocation(cornerZones[zoneIndex][0] + (cornerZoneWidth / i_numCubes), cornerZones[zoneIndex][1] + (cornerZoneHeight / i_numCubes));
			std::shared_ptr<Entity> ent = std::make_shared<Enemy>(projBody, spawnLocation, playerChar, this);
			lvlEntities.push_back(ent);
			sectEnemyNum += 1;
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


	std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(HOR_MARGIN, COURT_HEIGHT);
	RigidBody rightWallBody = RigidBody(vertRect1, Static);
	std::shared_ptr<Entity> rightWall = std::make_shared<Wall>(rightWallBody,
		Vector2f(HOR_MARGIN + (COURT_WIDTH)+(HOR_MARGIN / 2.0f), VERT_MARGIN + (COURT_HEIGHT / 2.0f)), this);
	lvlEntities.push_back(rightWall);

	std::shared_ptr<Shape> vertRect2 = std::make_shared<Rectangle>(HOR_MARGIN, COURT_HEIGHT);
	RigidBody leftWallBody = RigidBody(vertRect2, Static);
	std::shared_ptr<Entity> leftWall = std::make_shared<Wall>(leftWallBody,
		Vector2f((HOR_MARGIN / 2.0f), VERT_MARGIN + (COURT_HEIGHT / 2.0f)), this);
	AddEntPtrToSector(leftWall);

	std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(COURT_WIDTH + (HOR_MARGIN * 2), VERT_MARGIN);
	RigidBody wallBody1 = RigidBody(horRect1, Static);
	std::shared_ptr<Entity> upperWall = std::make_shared<Wall>(wallBody1,
		Vector2f(HOR_MARGIN + (COURT_WIDTH / 2.0f), (VERT_MARGIN / 2.0f)), this);
	lvlEntities.push_back(upperWall);


	std::shared_ptr<Shape> horRect2 = std::make_shared<Rectangle>(COURT_WIDTH + (HOR_MARGIN * 2), VERT_MARGIN);
	RigidBody lowerWallBody = RigidBody(horRect2, Static);
	std::shared_ptr<Entity> lowerWall = std::make_shared<Wall>(lowerWallBody,
		Vector2f(Vector2f(HOR_MARGIN + (COURT_WIDTH / 2.0f), (VERT_MARGIN / 2.0f) + COURT_HEIGHT + VERT_MARGIN)), this);
	lvlEntities.push_back(lowerWall);

}

void Sector::AddPainfullWallsToLevel()
{
	Material Static = Material(0.0f, 0.4f, 0.4f, 0.2f);


	std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(HOR_MARGIN, COURT_HEIGHT);
	RigidBody rightWallBody = RigidBody(vertRect1, Static);
	std::shared_ptr<Entity> rightWall = std::make_shared<PainWall>(rightWallBody,
		Vector2f(HOR_MARGIN + (COURT_WIDTH)+(HOR_MARGIN / 2.0f), VERT_MARGIN + (COURT_HEIGHT / 2.0f)), this);
	lvlEntities.push_back(rightWall);

	std::shared_ptr<Shape> vertRect2 = std::make_shared<Rectangle>(HOR_MARGIN, COURT_HEIGHT);
	RigidBody leftWallBody = RigidBody(vertRect2, Static);
	std::shared_ptr<Entity> leftWall = std::make_shared<PainWall>(leftWallBody,
		Vector2f((HOR_MARGIN / 2.0f), VERT_MARGIN + (COURT_HEIGHT / 2.0f)), this);
	AddEntPtrToSector(leftWall);

	std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(COURT_WIDTH + (HOR_MARGIN * 2), VERT_MARGIN);
	RigidBody wallBody1 = RigidBody(horRect1, Static);
	std::shared_ptr<Entity> upperWall = std::make_shared<PainWall>(wallBody1,
		Vector2f(HOR_MARGIN + (COURT_WIDTH / 2.0f), (VERT_MARGIN / 2.0f)), this);
	lvlEntities.push_back(upperWall);


	std::shared_ptr<Shape> horRect2 = std::make_shared<Rectangle>(COURT_WIDTH + (HOR_MARGIN * 2), VERT_MARGIN);
	RigidBody lowerWallBody = RigidBody(horRect2, Static);
	std::shared_ptr<Entity> lowerWall = std::make_shared<PainWall>(lowerWallBody,
		Vector2f(Vector2f(HOR_MARGIN + (COURT_WIDTH / 2.0f), (VERT_MARGIN / 2.0f) + COURT_HEIGHT + VERT_MARGIN)), this);
	lvlEntities.push_back(lowerWall);

}

void Sector::AddEndLevelObject()
{
	Material Static = Material(0.0f, 0.4f, 0.4f, 0.2f);
	std::shared_ptr<Shape> square = std::make_shared<Rectangle>(100.0f, 100.0f);
	RigidBody lowerWallBody = RigidBody(square, Static);
	std::shared_ptr<Entity> lowerWall = std::make_shared<EndObject>(lowerWallBody,
		Vector2f(Vector2f(HOR_MARGIN + (COURT_WIDTH / 2.0f), COURT_HEIGHT / 2.0)), this);
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

void Sector::AddTerrain(int i_terrainType)
{
	Material Static = Material(0.0f, 0.4f, 0.4f, 0.2f);

	switch (i_terrainType) {
		//small walls
	case 0: {
		std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(HOR_MARGIN / 8.0f, COURT_HEIGHT / 4.0f);
		RigidBody rightWallBody = RigidBody(vertRect1, Static);
		std::shared_ptr<Entity> rightWall = std::make_shared<Wall>(rightWallBody,
			Vector2f(HOR_MARGIN + (COURT_WIDTH * ( 1.0f / 5.0f)), VERT_MARGIN + (COURT_HEIGHT / 2.0f)), this);
		lvlEntities.push_back(rightWall);
		break;
	}
	case 1: {
		std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(HOR_MARGIN / 8.0f, COURT_HEIGHT / 4.0f);
		RigidBody rightWallBody = RigidBody(vertRect1, Static);
		std::shared_ptr<Entity> rightWall = std::make_shared<Wall>(rightWallBody,
			Vector2f(HOR_MARGIN + (COURT_WIDTH *  (4.0f / 5.0f)), VERT_MARGIN + (COURT_HEIGHT / 2.0f)), this);
		lvlEntities.push_back(rightWall);
		break;
	}
	case 2: {
		std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(COURT_WIDTH / 4.0f, VERT_MARGIN / 8.0f);
		RigidBody wallBody1 = RigidBody(horRect1, Static);
		std::shared_ptr<Entity> upperWall = std::make_shared<Wall>(wallBody1,
			Vector2f(HOR_MARGIN + (COURT_WIDTH / 2.0f), VERT_MARGIN + (COURT_HEIGHT * (1.0f / 5.0f))), this);
		lvlEntities.push_back(upperWall);
		break;
	}
	case 3: {
		std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(COURT_WIDTH / 4.0f, VERT_MARGIN / 8.0f);
		RigidBody wallBody1 = RigidBody(horRect1, Static);
		std::shared_ptr<Entity> upperWall = std::make_shared<Wall>(wallBody1,
			Vector2f(HOR_MARGIN + (COURT_WIDTH / 2.0f), VERT_MARGIN + (COURT_HEIGHT * (4.0f / 5.0f))), this);
		lvlEntities.push_back(upperWall);
		break;
	}
	//big walls
	case 4: {
		std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(HOR_MARGIN / 4.0f, COURT_HEIGHT / 2.0f);
		RigidBody rightWallBody = RigidBody(vertRect1, Static);
		std::shared_ptr<Entity> rightWall = std::make_shared<Wall>(rightWallBody,
			Vector2f(HOR_MARGIN + (COURT_WIDTH * (1.0f / 5.0f)), VERT_MARGIN + (COURT_HEIGHT / 2.0f)), this);
		lvlEntities.push_back(rightWall);
		break;
	}
	case 5: {
		std::shared_ptr<Shape> vertRect1 = std::make_shared<Rectangle>(HOR_MARGIN / 4.0f, COURT_HEIGHT / 2.0f);
		RigidBody rightWallBody = RigidBody(vertRect1, Static);
		std::shared_ptr<Entity> rightWall = std::make_shared<Wall>(rightWallBody,
			Vector2f(HOR_MARGIN + (COURT_WIDTH *  (4.0f / 5.0f)), VERT_MARGIN + (COURT_HEIGHT / 2.0f)), this);
		lvlEntities.push_back(rightWall);
		break;
	}
	case 6: {
		std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(COURT_WIDTH / 2.0f, VERT_MARGIN / 4.0f);
		RigidBody wallBody1 = RigidBody(horRect1, Static);
		std::shared_ptr<Entity> upperWall = std::make_shared<Wall>(wallBody1,
			Vector2f(HOR_MARGIN + (COURT_WIDTH / 2.0f), VERT_MARGIN + (COURT_HEIGHT * (1.0f / 5.0f))), this);
		lvlEntities.push_back(upperWall);
		break;
	}
	case 7: {
		std::shared_ptr<Shape> horRect1 = std::make_shared<Rectangle>(COURT_WIDTH / 2.0f, VERT_MARGIN / 4.0f);
		RigidBody wallBody1 = RigidBody(horRect1, Static);
		std::shared_ptr<Entity> upperWall = std::make_shared<Wall>(wallBody1,
			Vector2f(HOR_MARGIN + (COURT_WIDTH / 2.0f), VERT_MARGIN + (COURT_HEIGHT * (4.0f / 5.0f))), this);
		lvlEntities.push_back(upperWall);
		break;
	}
	}
}

void Sector::InitializeSector()
{
	//AddWallsToLevel();
	AddPainfullWallsToLevel();
}

void Sector::PlaySound(int i_soundNum)
{
	resources->PlaySound(i_soundNum);
}