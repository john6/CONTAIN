#include "GameRenderer.h"


GameRenderer::GameRenderer(){}


GameRenderer::~GameRenderer()
{
}

void GameRenderer::Render(sf::RenderWindow* i_window, float i_lerp_fraction, std::vector<Entity>* i_entityList, Entity playerChar) {
	i_window->clear();
	for (Entity entity : *i_entityList) {
		std::unique_ptr<sf::Shape> shape = entity.rb.CreateDrawable(i_lerp_fraction);
		i_window->draw(*shape);
		//sf::VertexArray line = entity.CreatOrientationLine(i_lerp_fraction);
		//i_window->draw(line);
		//for (sf::CircleShape circle : entity.CreateStructurePoints(i_lerp_fraction)) {
		//	i_window->draw(circle);
		//}
	}

	std::unique_ptr<sf::Shape> shape = playerChar.rb.CreateDrawable(i_lerp_fraction);
	shape->setOutlineColor(sf::Color::Red);
	i_window->draw(*shape);
	i_window->display();
}