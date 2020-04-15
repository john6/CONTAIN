#include "GameRenderer.h"


GameRenderer::GameRenderer(){}


GameRenderer::~GameRenderer()
{
}

void GameRenderer::Render(sf::RenderWindow* i_window, float i_elapsedMilliseconds, std::vector<RigidBody>* i_entityList) {
	i_window->clear();
	for (RigidBody entity : *i_entityList) {
		std::unique_ptr<sf::Shape> shape = entity.CreateDrawable();
		i_window->draw(*shape);
		sf::VertexArray line = entity.CreatOrientationLine();
		i_window->draw(line);
		for (sf::CircleShape circle : entity.CreateStructurePoints()) {
			i_window->draw(circle);
		}
	}

	i_window->display();
}