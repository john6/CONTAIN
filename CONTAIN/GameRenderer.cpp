#include "GameRenderer.h"


GameRenderer::GameRenderer(){
	GameRenderer::worldView.reset(sf::FloatRect(0, 0, WORLDSIZEWIDTH, WORLDSIZEHEIGHT));
	GameRenderer::worldView.setCenter(WORLDSIZEWIDTH * 0.5f, WORLDSIZEHEIGHT * 0.5f);
	GameRenderer::hudView.reset(sf::FloatRect(0, 0, GLBVRS::SCREEN_WIDTH, GLBVRS::SCREEN_HEIGHT));
}


GameRenderer::~GameRenderer()
{
}

void GameRenderer::Render(sf::RenderWindow* i_window, float i_lerp_fraction,
	std::list<std::shared_ptr<Entity>>* i_entityList, Entity* playerChar, std::vector<sf::Drawable*> i_HUDelem) {

	i_window->clear();
	
	i_window->setView(GameRenderer::worldView);


	std::vector<const sf::Drawable*> animationDrawables;

	if (i_entityList->size() != 0) {
		auto entIt = i_entityList->begin();
			while (entIt != i_entityList->end()) {
				//i_window->draw(*it._Ptr->_Myval->CreateDrawable(i_lerp_fraction));
				drawablePtrVect drawables = entIt._Ptr->_Myval->CreateDrawables(i_lerp_fraction);
				if (drawables->size() != 0) {
					auto drawbIt = drawables->begin();
					while (drawbIt != drawables->end()) {
						if (entIt._Ptr->_Myval->GetTypeID() == ANIMATION) {
							animationDrawables.push_back(drawbIt._Ptr->get());
						}
						else {
							i_window->draw(*drawbIt._Ptr->get());
						}

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


	//std::unique_ptr<sf::Shape> shape = playerChar->CreateDrawable(i_lerp_fraction);
	drawablePtrVect playerDrawables = playerChar->CreateDrawables(i_lerp_fraction);
	if (playerDrawables->size() != 0) {
		auto playerIt = playerDrawables->begin();
		while (playerIt != playerDrawables->end()) {
			i_window->draw(*playerIt._Ptr->get());
			playerIt++;
		}
	}

	for (int i = 0; i < animationDrawables.size(); i++) {
		i_window->draw(*animationDrawables[i]);
	}
	
	i_window->setView(GameRenderer::hudView);

	for (sf::Drawable* drawble : i_HUDelem) {
		i_window->draw(*drawble);
	}

	//for (sf::VertexArray section : i_sections) {
	//	i_window->draw(section);
	//}


	//shape->setOutlineColor(sf::Color::Red);
	//i_window->draw(*shape);
	i_window->display();

	i_window->setView(GameRenderer::worldView);
}

void GameRenderer::ResetWorldSize(int width, int height)
{
	GameRenderer::worldView.reset(sf::FloatRect(0, 0, 3744, 2106));
	GameRenderer::worldView.setCenter(width / 2, height / 2);
}

void GameRenderer::SetHudSize(int width, int height)
{
	GameRenderer::hudView.reset(sf::FloatRect(0, 0, width, height));
}
