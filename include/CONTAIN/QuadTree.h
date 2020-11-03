#pragma once
#include <array>
#include "Entity.h"
#include "Shape.h"
#include "Rectangle.h"
#include "Circle.h"

class QuadTree
{
private:
	const static int MAX_OBJECTS = 4;
	const static int MAX_LEVELS = 3;

	int level;
	std::vector<Entity*> ents;
	std::vector<QuadTree> children;
	
	Vector2f topLeftPoint;
	float width;
	float height;
	
	//int GetSubIndex(Rectangle i_rect);

	//int GetSubIndex(Circle i_circ);

public:
	QuadTree(int i_lvl, Vector2f topLeftPoint, float width, float height);
	~QuadTree();

	void Clear();
	
	void Split();

	int GetSubIndex(Entity* i_ent);

	void Insert(Entity* i_ent);

	std::vector<Entity*> GetSectorEntities(Entity* i_ent);

	std::vector<sf::VertexArray> GetDrawableSectionLines();
};
