#include "SplashScreen.h"



SplashScreen::SplashScreen(RESOURCES* i_resources) :
	resources{ i_resources }
{
	font = resources->GetFont();

	splashView.reset(sf::FloatRect(0, 0, GLBVRS::SCREEN_WIDTH, GLBVRS::SCREEN_HEIGHT));

	title = sf::Text();
	title.setFont(font);
	title.setString("CONTAIN");
	title.setCharacterSize(160);
	title.setFillColor(sf::Color::Green);
	title.setOutlineColor(sf::Color::White);
	title.setOutlineThickness(3);
	GLBVRS::SetTextOriginCenter(&title);
	title.setPosition(sf::Vector2f(GLBVRS::HR_MRG + GLBVRS::CRT_WDTH / 2.0f, GLBVRS::VRT_MRG * 2.5f));

	mainCreditLabel = sf::Text();
	mainCreditLabel.setFont(font);
	mainCreditLabel.setString("Programming/Design/Etc");
	mainCreditLabel.setCharacterSize(23);
	mainCreditLabel.setFillColor(sf::Color::Magenta);
	GLBVRS::SetTextOriginCenter(&mainCreditLabel);
	mainCreditLabel.setPosition(sf::Vector2f(GLBVRS::HR_MRG + GLBVRS::CRT_WDTH * (5 / 10.0f), GLBVRS::VRT_MRG * 7.5f));

	mainCreditName = sf::Text();
	mainCreditName.setFont(font);
	mainCreditName.setString("John Russell");
	mainCreditName.setCharacterSize(23);
	mainCreditName.setFillColor(sf::Color::White);
	GLBVRS::SetTextOriginCenter(&mainCreditName);
	mainCreditName.setPosition(sf::Vector2f(GLBVRS::HR_MRG + GLBVRS::CRT_WDTH * (5 / 10.0f), GLBVRS::VRT_MRG * 8.5f));

	portingLabel = sf::Text();
	portingLabel.setFont(font);
	portingLabel.setString("Linux Port");
	portingLabel.setCharacterSize(23);
	portingLabel.setFillColor(sf::Color::Cyan);
	GLBVRS::SetTextOriginCenter(&portingLabel);
	portingLabel.setPosition(sf::Vector2f(GLBVRS::HR_MRG + GLBVRS::CRT_WDTH * (2 / 10.0f), GLBVRS::VRT_MRG * 10.5f));

	portingList = sf::Text();
	portingList.setFont(font);
	portingList.setString("!MrZeus");
	portingList.setCharacterSize(23);
	portingList.setLineSpacing(1.5);
	portingList.setFillColor(sf::Color::White);
	GLBVRS::SetTextOriginCenter(&portingList);
	portingList.setPosition(sf::Vector2f(GLBVRS::HR_MRG + GLBVRS::CRT_WDTH * (2 / 10.0f), GLBVRS::VRT_MRG * 11.5f));

	MusicLabel = sf::Text();
	MusicLabel.setFont(font);
	MusicLabel.setString("Sound");
	MusicLabel.setCharacterSize(23);
	MusicLabel.setFillColor(sf::Color::Cyan);
	GLBVRS::SetTextOriginCenter(&MusicLabel);
	MusicLabel.setPosition(sf::Vector2f(GLBVRS::HR_MRG + GLBVRS::CRT_WDTH * (5 / 10.0f), GLBVRS::VRT_MRG * 10.5f));

	musicList = sf::Text();
	musicList.setFont(font);
	musicList.setString("DHSFX@itch.io\nyaru@itch.io");
	musicList.setCharacterSize(23);
	musicList.setLineSpacing(1.5);
	musicList.setFillColor(sf::Color::White);
	GLBVRS::SetTextOriginCenter(&musicList);
	musicList.setPosition(sf::Vector2f(GLBVRS::HR_MRG + GLBVRS::CRT_WDTH * (5 / 10.0f), GLBVRS::VRT_MRG * 11.5f));

	texturesLabel = sf::Text();
	texturesLabel.setFont(font);
	texturesLabel.setString("Textures");
	texturesLabel.setCharacterSize(23);
	texturesLabel.setFillColor(sf::Color::Cyan);
	GLBVRS::SetTextOriginCenter(&texturesLabel);
	texturesLabel.setPosition(sf::Vector2f(GLBVRS::HR_MRG + GLBVRS::CRT_WDTH * (8 / 10.0f), GLBVRS::VRT_MRG * 10.5f));

	texturesList = sf::Text();
	texturesList.setFont(font);
	texturesList.setString("mutantLeg@itch.io");
	texturesList.setCharacterSize(23);
	texturesList.setFillColor(sf::Color::White);
	GLBVRS::SetTextOriginCenter(&texturesList);
	texturesList.setPosition(sf::Vector2f(GLBVRS::HR_MRG + GLBVRS::CRT_WDTH * (8 / 10.0f), GLBVRS::VRT_MRG * 11.5f));

	playtestersLabel = sf::Text();
	playtestersLabel.setFont(font);
	playtestersLabel.setString("Playtesting / Design");
	playtestersLabel.setCharacterSize(23);
	playtestersLabel.setFillColor(sf::Color::Yellow);
	GLBVRS::SetTextOriginCenter(&playtestersLabel);
	playtestersLabel.setPosition(sf::Vector2f(GLBVRS::HR_MRG + GLBVRS::CRT_WDTH / 2.0f, GLBVRS::VRT_MRG * 14.5f));

	playtestersList = sf::Text();
	playtestersList.setFont(font);
	playtestersList.setString("Sam Russell, Vincent Marocco, Tyler Matthews, Chris Brown\nRay Marocco, Trevor Larson, Leisa Russell, Brian Russell,\n        Gabby Peters, Jantzen Carl, Michael Lane");
	playtestersList.setCharacterSize(23);
	playtestersList.setLineSpacing(1.5);
	playtestersList.setFillColor(sf::Color::White);
	GLBVRS::SetTextOriginCenter(&playtestersList);
	playtestersList.setPosition(sf::Vector2f(GLBVRS::HR_MRG + GLBVRS::CRT_WDTH / 2.0f, GLBVRS::VRT_MRG * 16.5f));
}


SplashScreen::~SplashScreen()
{
}

void SplashScreen::Render(sf::RenderWindow * window)
{
	window->clear();

	window->setView(splashView);

	window->draw(title);
	window->draw(mainCreditLabel);
	window->draw(mainCreditName);
	window->draw(portingLabel);
	window->draw(portingList);
	window->draw(texturesLabel);
	window->draw(texturesList);
	window->draw(MusicLabel);
	window->draw(musicList);
	window->draw(playtestersLabel);
	window->draw(playtestersList);

	window->display();
}
