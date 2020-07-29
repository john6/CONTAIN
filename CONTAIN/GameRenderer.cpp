#include "GameRenderer.h"


GameRenderer::GameRenderer(){}


GameRenderer::~GameRenderer()
{
}

void GameRenderer::Render(sf::RenderWindow* i_window, float i_lerp_fraction,
	std::list<std::shared_ptr<Entity>>* i_entityList, Entity* playerChar, std::vector<sf::Drawable*> i_HUDelem) {
	i_window->clear();
	if (i_entityList->size() != 0) {
		auto entIt = i_entityList->begin();
			while (entIt != i_entityList->end()) {
				//i_window->draw(*it._Ptr->_Myval->CreateDrawable(i_lerp_fraction));
				drawablePtrVect drawables = entIt._Ptr->_Myval->CreateDrawables(i_lerp_fraction);
				if (drawables->size() != 0) {
					auto drawbIt = drawables->begin();
					while (drawbIt != drawables->end()) {
						i_window->draw(*drawbIt._Ptr->get());
						drawbIt++;
					}
				}
				//sf::VertexArray line = entity.rb.CreatOrientationLine(i_lerp_fraction);
				//i_window->draw(line);
				//for (sf::CircleShape circle : entity.CreateStructurePoints(i_lerp_fraction)) {
				//	i_window->draw(circle);
				//}
				entIt++;
			}
	}
	

	for (sf::Drawable* drawble : i_HUDelem) {
		i_window->draw(*drawble);
	}

	//for (sf::VertexArray section : i_sections) {
	//	i_window->draw(section);
	//}

	//std::unique_ptr<sf::Shape> shape = playerChar->CreateDrawable(i_lerp_fraction);
	drawablePtrVect playerDrawables = playerChar->CreateDrawables(i_lerp_fraction);
	if (playerDrawables->size() != 0) {
		auto playerIt = playerDrawables->begin();
		while (playerIt != playerDrawables->end()) {
			i_window->draw(*playerIt._Ptr->get());
			playerIt++;
		}
	}
	//shape->setOutlineColor(sf::Color::Red);
	//i_window->draw(*shape);
	i_window->display();
}