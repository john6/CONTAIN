#include "GameRenderer.h"


GameRenderer::GameRenderer(){}


GameRenderer::~GameRenderer()
{
}

void GameRenderer::Render(sf::RenderWindow* i_window, float i_lerp_fraction, std::list<Entity>* i_entityList, Entity playerChar, std::vector<sf::VertexArray> i_sections) {
	i_window->clear();
	for (Entity entity : *i_entityList) {
		std::unique_ptr<sf::Shape> shape = entity.rb.CreateDrawable(i_lerp_fraction);
		i_window->draw(*shape);
		//sf::VertexArray line = entity.rb.CreatOrientationLine(i_lerp_fraction);
		//i_window->draw(line);
		//for (sf::CircleShape circle : entity.CreateStructurePoints(i_lerp_fraction)) {
		//	i_window->draw(circle);
		//}
	}

	for (sf::VertexArray section : i_sections) {
		i_window->draw(section);
	}

	std::unique_ptr<sf::Shape> shape = playerChar.rb.CreateDrawable(i_lerp_fraction);
	shape->setOutlineColor(sf::Color::Red);
	i_window->draw(*shape);
	i_window->display();
}