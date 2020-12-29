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
	mainCreditLabel.setString("Programming / Design / Etc");
	mainCreditLabel.setCharacterSize(30);
	mainCreditLabel.setFillColor(sf::Color::Magenta);
	GLBVRS::SetTextOriginCenter(&mainCreditLabel);
	mainCreditLabel.setPosition(sf::Vector2f(GLBVRS::HR_MRG + GLBVRS::CRT_WDTH / 5.0f, GLBVRS::VRT_MRG * 6.5f));

	mainCreditName = sf::Text();
	mainCreditName.setFont(font);
	mainCreditName.setString("John Russell");
	mainCreditName.setCharacterSize(25);
	mainCreditName.setFillColor(sf::Color::White);
	GLBVRS::SetTextOriginCenter(&mainCreditName);
	mainCreditName.setPosition(sf::Vector2f(GLBVRS::HR_MRG + GLBVRS::CRT_WDTH / 5.0f, GLBVRS::VRT_MRG * 8.5f));

	MusicLabel = sf::Text();
	MusicLabel.setFont(font);
	MusicLabel.setString("Music / Sound Effects");
	MusicLabel.setCharacterSize(30);
	MusicLabel.setFillColor(sf::Color::Cyan);
	GLBVRS::SetTextOriginCenter(&MusicLabel);
	MusicLabel.setPosition(sf::Vector2f(GLBVRS::HR_MRG + GLBVRS::CRT_WDTH * ( 4.0f / 5.0f), GLBVRS::VRT_MRG * 6.5f));

	musicList = sf::Text();
	musicList.setFont(font);
	musicList.setString("Dafydd Harvey - DHSFX@itch.io\n      Yal - yaru@itch.io");
	musicList.setCharacterSize(25);
	musicList.setLineSpacing(1.5);
	musicList.setFillColor(sf::Color::White);
	GLBVRS::SetTextOriginCenter(&musicList);
	musicList.setPosition(sf::Vector2f(GLBVRS::HR_MRG + GLBVRS::CRT_WDTH * (4.0f / 5.0f), GLBVRS::VRT_MRG * 8.5f));

	playtestersLabel = sf::Text();
	playtestersLabel.setFont(font);
	playtestersLabel.setString("Playtesting / Design");
	playtestersLabel.setCharacterSize(30);
	playtestersLabel.setFillColor(sf::Color::Yellow);
	GLBVRS::SetTextOriginCenter(&playtestersLabel);
	playtestersLabel.setPosition(sf::Vector2f(GLBVRS::HR_MRG + GLBVRS::CRT_WDTH / 2.0f, GLBVRS::VRT_MRG * 13.5f));

	playtestersList = sf::Text();
	playtestersList.setFont(font);
	playtestersList.setString("Sam Russell, Vincent Marocco, Tyler Matthews, Chris Brown\nRay Marocco, Trevor Larson, Leisa Russell, Brian Russell,\n                   Gabby Peters, Jantzen Carl");
	playtestersList.setCharacterSize(25);
	playtestersList.setLineSpacing(1.5);
	playtestersList.setFillColor(sf::Color::White);
	GLBVRS::SetTextOriginCenter(&playtestersList);
	playtestersList.setPosition(sf::Vector2f(GLBVRS::HR_MRG + GLBVRS::CRT_WDTH / 2.0f, GLBVRS::VRT_MRG * 15.5f));
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
	window->draw(MusicLabel);
	window->draw(musicList);
	window->draw(playtestersLabel);
	window->draw(playtestersList);

	window->display();
}
