#include "GameRenderer.h"


GameRenderer::GameRenderer(){}


GameRenderer::~GameRenderer()
{
}

void GameRenderer::Render(sf::RenderWindow* i_window, float i_lerp_fraction, std::list<std::shared_ptr<Entity>>* i_entityList, Entity* playerChar, std::vector<sf::VertexArray> i_sections) {
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

	for (sf::VertexArray section : i_sections) {
		i_window->draw(section);
	}

	std::unique_ptr<sf::Shape> shape = playerChar->CreateDrawable(i_lerp_fraction);
	shape->setOutlineColor(sf::Color::Red);
	i_window->draw(*shape);
	i_window->display();
}