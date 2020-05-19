#include "QuadTree.h"



QuadTree::QuadTree(int i_lvl, Vector2f i_topLeftPoint, float i_width, float i_height) :
	level { i_lvl }, topLeftPoint { i_topLeftPoint }, width { i_width }, height {i_height}
{	
}


//void QuadTree::GetSubIndex(Rectangle i_rect)
//{
//}
//
//void QuadTree::GetSubIndex(Circle i_circ)
//{
//}

QuadTree::~QuadTree()
{
}

void QuadTree::Clear()
{
	//I DONT THINK I NEED THIS, this class just holds pointers and I dont call 'new' or anything
}

void QuadTree::Split()
{
	float  halfWidth = width / 2.0f;
	float halfHeight = height / 2.0f;
	children.push_back(QuadTree(level + 1, Vector2f(topLeftPoint(0), topLeftPoint(1)), halfWidth, halfHeight));
	children.push_back(QuadTree(level + 1, Vector2f(topLeftPoint(0) + halfWidth, topLeftPoint(1)), halfWidth, halfHeight));
	children.push_back(QuadTree(level + 1, Vector2f(topLeftPoint(0) + halfWidth , topLeftPoint(1) + halfHeight), halfWidth, halfHeight));
	children.push_back(QuadTree(level + 1, Vector2f(topLeftPoint(0), topLeftPoint(1) + halfHeight), halfWidth, halfHeight));
}

int QuadTree::GetSubIndex(Entity* i_ent)
{
	float halfHeight = height / 2.0f;
	float halfWidth = width / 2.0f;
	int index = -1;
	float horMidPoint = topLeftPoint(1) + halfHeight;
	float vertMidPoint = topLeftPoint(0) + halfWidth;

	// frick Im just gonna find the intersection here so I dont ahve to deal with it anywhere else

	bool aboveHorMidLine = false;
	bool onHorMidLine = true;
	bool leftVertMidLine = false;
	bool onVertMidLine = false;

	if (i_ent->rb.shape->GetType() == Shape::ShapeType::Circle) {
		std::shared_ptr<Circle> circlePtr = std::dynamic_pointer_cast<Circle>(i_ent->rb.shape);
		Vector2f position = i_ent->rb.transform.pos;
		float radius = circlePtr->radius;
		//check above or below horizontal midline
		if (position(1) + radius < horMidPoint) {
			aboveHorMidLine = true;
			onHorMidLine = false;
		}
		else {
			aboveHorMidLine = false;
			onHorMidLine = false;
		}
		if (abs(horMidPoint - position(1)) - radius <= 0.0f) {
			onHorMidLine = true;
		}
		//check left or right of vertical midline
		if (position(0) + radius < vertMidPoint) {
			leftVertMidLine = true;
			onVertMidLine = false;
		}
		else {
			leftVertMidLine = false;
			onVertMidLine = false;
		}
		if (abs(vertMidPoint - position(0)) - radius <= 0.0f) {
			onVertMidLine = true;
		}
	}
	else {
		std::vector<Vector2f> rectPoints = i_ent->rb.GetVertCords();
		bool firstCoordAboveMidPoint = rectPoints[0](1) < horMidPoint;
		onHorMidLine = false;
		for (Vector2f coord : rectPoints) {
			bool coordAboveMidPoint = coord(1) < horMidPoint;
			if (firstCoordAboveMidPoint != coordAboveMidPoint) {
				onHorMidLine = true;
				break;
			}
		}
		aboveHorMidLine = firstCoordAboveMidPoint;

		bool firstCoordLeftOfMidPoint = rectPoints[0](0) < vertMidPoint;
		onVertMidLine = false;
		for (Vector2f coord : rectPoints) {
			bool coordLeftOfMidPoint = coord(0) < vertMidPoint;
			if (firstCoordLeftOfMidPoint != coordLeftOfMidPoint) {
				onVertMidLine = true;
				break;
			}
		}
		leftVertMidLine = firstCoordLeftOfMidPoint;
	}

	if (onHorMidLine || onVertMidLine) {
		return -1;
	}
	else if (aboveHorMidLine) {
		if (leftVertMidLine) {
			return 0;
		}
		else {
			return 1;
		}
	}
	else {
		if (leftVertMidLine) {
			return 3;
		}
		else {
			return 2;
		}
	}
}

void QuadTree::Insert(Entity* i_ent)
{
	if (children.size() != 0) {
		int index = GetSubIndex(i_ent);
		if (index != -1) {
			children[index].Insert(i_ent);
			return;
		}
		else {
			//if there are children, and you straddle a dividing line,
			//you still have to put the element in the tree
			ents.push_back(i_ent);
		}
	}
	else {
		ents.push_back(i_ent);
	}
	
	if ((ents.size() > MAX_OBJECTS) && (level < MAX_LEVELS)) {
		if (children.size() == 0) {
			Split();
		}
		int i = 0;
		while (i < ents.size()) {
			//this will work now because I'vfe already placed the straddling item in the tree. 
			//There can possibly be more than MAX_OBJECTS in a level but only if there are more straddling objects than MAX_Objects
			int index = GetSubIndex(ents[i]);
			if (index != -1) {
				Insert(ents[i]);
				ents.erase(ents.begin()+i);
			}
			else {
				++i;
			}
		}

	}

}

std::vector<Entity*> QuadTree::GetSectorEntities(Entity* i_ent)
{
	//std::vector<Entity*> sameSectorEnts;
	//int index = GetSubIndex(i_ent);
	//if (index != -1 && children.size() != 0) {
	//	return children[index].GetSectorEntities(i_ent);
	//}
	//else {
	//	for (Entity* entPtr : ents) {
	//		sameSectorEnts.push_back(entPtr);
	//	}
	//	if (children.size() != 0) {
	//		for (QuadTree quad : children) {
	//			quad.GetSectorEntities(i_ent);
	//		}
	//	}
	//}
	//return sameSectorEnts;

	std::vector<Entity*> sameSectorEnts;
	int index = GetSubIndex(i_ent);
	for (Entity* entPtr : ents) {
		sameSectorEnts.push_back(entPtr);
	}
	if ((index == -1) && (children.size() != 0)) {
		for (QuadTree quad : children) {
			std::vector<Entity*> childSectorEnts = quad.GetSectorEntities(i_ent);
			for (Entity* entPtr : childSectorEnts) {
				sameSectorEnts.push_back(entPtr);
			}
		}
	}
	else if (children.size() != 0) {
		std::vector<Entity*> childSectorEnts = children[index].GetSectorEntities(i_ent);
		for (Entity* entPtr : childSectorEnts) {
			sameSectorEnts.push_back(entPtr);
		}
	}
	return sameSectorEnts;
}

std::vector<sf::VertexArray> QuadTree::GetDrawableSectionLines()
{
	std::vector<sf::VertexArray> lines;
	if (children.size() != 0) {
		sf::VertexArray vertLine = sf::VertexArray(sf::LinesStrip, 2);
		sf::VertexArray horLine = sf::VertexArray(sf::LinesStrip, 2);
		sf::Vector2f vertPos1 = sf::Vector2f(topLeftPoint(0) + (width / 2.0f), topLeftPoint(1));
		sf::Vector2f vertPos2 = sf::Vector2f(topLeftPoint(0) + (width / 2.0f), topLeftPoint(1) + (height));
		sf::Vector2f horPos1 = sf::Vector2f(topLeftPoint(0), topLeftPoint(1) + (height / 2.0f));
		sf::Vector2f horPos2 = sf::Vector2f(topLeftPoint(0) + (width), topLeftPoint(1) + (height / 2.0f));
		vertLine[0] = sf::Vertex(vertPos1, sf::Color::Red);
		vertLine[1] = sf::Vertex(vertPos2, sf::Color::Red);
		horLine[0] = sf::Vertex(horPos1, sf::Color::Red);
		horLine[1] = sf::Vertex(horPos2, sf::Color::Red);
		lines.push_back(vertLine);
		lines.push_back(horLine);

		for (QuadTree child : children) {
			std::vector<sf::VertexArray> childLines = child.GetDrawableSectionLines();
			for (sf::VertexArray line : childLines) {
				lines.push_back(line);
			}
		}
	}

	return lines;
}
