#include "GameRenderer.h"


GameRenderer::GameRenderer(){}


GameRenderer::~GameRenderer()
{
}

void GameRenderer::Render(sf::RenderWindow* i_window, float i_lerp_fraction,
	std::list<std::shared_ptr<Entity>>* i_entityList, Entity* playerChar, std::vector<sf::Drawable*> i_HUDelem) {
	i_window->clear();
	auto it = i_entityList->begin();
	while (it != i_entityList->end()) {
		i_window->draw(*it._Ptr->_Myval->CreateDrawable(i_lerp_fraction));
		//sf::VertexArray line = entity.rb.CreatOrientationLine(i_lerp_fraction);
		//i_window->draw(line);
		//for (sf::CircleShape circle : entity.CreateStructurePoints(i_lerp_fraction)) {
		//	i_window->draw(circle);
		//}
		it++;
	}

	for (sf::Drawable* drawble : i_HUDelem) {
		i_window->draw(*drawble);
	}

	//std::vector<sf::Vector2f> vector;
	//vector.push_back(sf::Vector2f(-50.0, -50.0));
	//vector.push_back(sf::Vector2f(50.0, -50.0));
	//vector.push_back(sf::Vector2f(0.0, 50.0));




	//sf::ConvexShape poly = sf::ConvexShape(vector);
	//int size = pointArr.size();
	//for (int i = 0; i < size; ++i) {
	//	sf::Vector2f vect = sf::Vector2f(pointArr[i][0], pointArr[i][0]);
	//	polyPtr->setPoint(i, vect);
	//}
	//std::unique_ptr<sf::Shape> shapePtr = move(polyPtr);
	//return shapePtr;

	
// create an empty shape
sf::ConvexShape convex;

// resize it to 5 points
convex.setPointCount(5);

// define the points
//convex.setPoint(0, sf::Vector2f(0, 0));
//convex.setPoint(1, sf::Vector2f(150, 10));
//convex.setPoint(2, sf::Vector2f(120, 90));
//convex.setPoint(3, sf::Vector2f(30, 100));
//convex.setPoint(4, sf::Vector2f(0, 50));
//i_window->draw(convex);



	//for (sf::VertexArray section : i_sections) {
	//	i_window->draw(section);
	//}

	std::unique_ptr<sf::Shape> shape = playerChar->CreateDrawable(i_lerp_fraction);
	shape->setOutlineColor(sf::Color::Red);
	i_window->draw(*shape);
	i_window->display();
}