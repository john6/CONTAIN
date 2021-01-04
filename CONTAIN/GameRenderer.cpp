#include "GameRenderer.h"
#include "Scenery.h"

GameRenderer::GameRenderer(){
	GameRenderer::worldView.reset(sf::FloatRect(0, 0, WORLD_SIZE_WINDOW_WIDTH, WORLD_SIZE_WINDOW_HEIGHT));
	GameRenderer::worldView.setCenter(WORLD_SIZE_WINDOW_WIDTH * 0.5f, WORLD_SIZE_WINDOW_HEIGHT * 0.5f);
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


	//first pass for scenery
	if (i_entityList->size() != 0) {
		auto entIt = i_entityList->begin();
		while (entIt != i_entityList->end()) {
			//i_window->draw(*it._Ptr->_Myval->CreateDrawable(i_lerp_fraction));
			if ((entIt._Ptr->_Myval->GetTypeID() == SCENERY)) {
				auto scenery = dynamic_cast<Scenery*>(entIt._Ptr->_Myval.get());
				if (scenery->renderLvl == 0) {
					drawablePtrVect drawables = entIt._Ptr->_Myval->CreateDrawables(i_lerp_fraction);
					if (drawables->size() != 0) {
						auto drawbIt = drawables->begin();
						while (drawbIt != drawables->end()) {
							i_window->draw(*drawbIt._Ptr->get());
							drawbIt++;
						}
					}
				}
			}
			entIt++;
		}
	}

	//second pass for other shit
	if (i_entityList->size() != 0) {
		auto entIt = i_entityList->begin();
		while (entIt != i_entityList->end()) {
			//i_window->draw(*it._Ptr->_Myval->CreateDrawable(i_lerp_fraction));
			if ((entIt._Ptr->_Myval->GetTypeID() == SCENERY)) {
				auto scenery = dynamic_cast<Scenery*>(entIt._Ptr->_Myval.get());
				if (scenery->renderLvl == 1) {
					drawablePtrVect drawables = entIt._Ptr->_Myval->CreateDrawables(i_lerp_fraction);
					if (drawables->size() != 0) {
						auto drawbIt = drawables->begin();
						while (drawbIt != drawables->end()) {
							i_window->draw(*drawbIt._Ptr->get());
							drawbIt++;
						}
					}
				}
			}
			else if ((entIt._Ptr->_Myval->GetTypeID() == ANIMATION)) {
				auto scenery = dynamic_cast<Scenery*>(entIt._Ptr->_Myval.get());
					drawablePtrVect drawables = entIt._Ptr->_Myval->CreateDrawables(i_lerp_fraction);
					if (drawables->size() != 0) {
						auto drawbIt = drawables->begin();
						while (drawbIt != drawables->end()) {
							animationDrawables.push_back(drawbIt._Ptr->get());
							drawbIt++;
						}
					}
			}
			else {
				auto scenery = dynamic_cast<Scenery*>(entIt._Ptr->_Myval.get());
				drawablePtrVect drawables = entIt._Ptr->_Myval->CreateDrawables(i_lerp_fraction);
				if (drawables->size() != 0) {
					auto drawbIt = drawables->begin();
					while (drawbIt != drawables->end()) {
						i_window->draw(*drawbIt._Ptr->get());
						drawbIt++;
					}
				}
			}
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

	//Third pass for filter
	if (i_entityList->size() != 0) {
		auto entIt = i_entityList->begin();
		while (entIt != i_entityList->end()) {
			//i_window->draw(*it._Ptr->_Myval->CreateDrawable(i_lerp_fraction));
			if ((entIt._Ptr->_Myval->GetTypeID() == SCENERY)) {
				auto scenery = dynamic_cast<Scenery*>(entIt._Ptr->_Myval.get());
				if (scenery->renderLvl == 2) {
					drawablePtrVect drawables = entIt._Ptr->_Myval->CreateDrawables(i_lerp_fraction);
					if (drawables->size() != 0) {
						auto drawbIt = drawables->begin();
						while (drawbIt != drawables->end()) {
							i_window->draw(*drawbIt._Ptr->get());
							drawbIt++;
						}
					}
				}
			}
			entIt++;
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
	GameRenderer::worldView.reset(sf::FloatRect(0, 0, WORLD_SIZE_WINDOW_WIDTH, WORLD_SIZE_WINDOW_HEIGHT));
	GameRenderer::worldView.setCenter(width / 2, height / 2);
}

void GameRenderer::SetHudSize(int width, int height)
{
	GameRenderer::hudView.reset(sf::FloatRect(0, 0, width, height));
}

//void GameRenderer::SetView(sf::View renderView) {
//	i_window->setView(renderView);
//}
