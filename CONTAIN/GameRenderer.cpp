#include "GameRenderer.h"



GameRenderer::GameRenderer(sf::Vector2f i_screenDimensions, sf::RenderWindow* i_window) :
	screenDimensions {i_screenDimensions}, window {i_window}
{}


GameRenderer::~GameRenderer()
{
}

void GameRenderer::Render(float i_elapsedMilliseconds, std::vector<Entity> i_entityList) {

}

sf::RectangleShape GameRenderer::CreateDrawableEntity(Entity i_entity) {
	//!!!!!!!!!!!!!!!!!!!!!!!!Draw order matters, will draw shaped on top of previous shapes!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	sf::RectangleShape drawable; // WILL NEED MORE VARIABILITY HERE
	//drawable.setOrigin( //  CENTER OFFSET  //);
	//drawable.setPosition(//  ENTTIY POSITION + CENTER OFFSET  //);
	//drawable.setSize(sf::Vector2f(//  ENTITY DIMENSIONS  //);
	//drawable.setRotation(//  ENTITY ROTATION  //);
	drawable.setFillColor(sf::Color::White); //FOR TESTING UNTIL COMPLETE
	return drawable;
}