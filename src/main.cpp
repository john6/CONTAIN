#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <chrono>
#include <ctime>    
#include <CONTAIN/GLOBAL_CONSTANTS.h>
#include <CONTAIN/RESOURCES.h>
#include <CONTAIN/Game.h>
#include <CONTAIN/Menu.h>
#include <CONTAIN/SettingsMenu.h>
#include <CONTAIN/YouWonMenu.h>
#include <CONTAIN/YouLostMenu.h>
#include <CONTAIN/SaveData.h>
//TODO: Contact points should be in local space I think, I never convert them

int main()
{
	//https://eigen.tuxfamily.org/dox//TopicMultiThreading.html
	Eigen::initParallel();
	SaveData saveData = SaveData();
	//16:10 aspect ratio resolutions: 1280x800, 1440x900, 1680x1050, 1920x1200, and 2560x1600.
	RSLTN resolution = NNTN_TN;
	float soundLvl = 50.0f;
	bool fullScreen = true;
	sf::RenderWindow window(GLBVRS::GetVideoMode(resolution), "CONTAIN", sf::Style::Fullscreen);
	//sf::RenderWindow window(sf::VideoMode(1920, 1080), "CONTAIN");
	//sf::RenderWindow window(sf::VideoMode(1440, 900), "CONTAIN");
	//sf::RenderWindow window(sf::VideoMode(1280, 720), "CONTAIN");
	//sf::RenderWindow window(sf::VideoMode(1920, 1080), "CONTAIN");

	//::ShowWindow(window.getSystemHandle(), SW_MAXIMIZE);

	RESOURCES resources;

	GLBVRS::SetGlobalConstants(window.getSize().x, window.getSize().y, &resources, NULL, NULL, soundLvl);
	Game globalGame = Game(&window, &resources);
	GLBVRS::SetGlobalConstants(window.getSize().x, window.getSize().y, &resources, &globalGame, globalGame.playerChar, soundLvl);
	int currLvl;

	DIFFICULTY difficulty = MEDIUM;

	Menu menu(&resources);
	SettingsMenu settingsMenu(&resources, &resolution, &fullScreen);
	YouWonMenu winMenu(&resources, true);
	YouWonMenu lostMenu(&resources, false);
	GAME_STATE state = MENU;
	bool justSwitchedBackToMenu = false;
	int lastRunScore = 0;
	hiRes_time_point currTime = hiResTime::now();
	const microSec UPDATE_INTERVAL(16666);   //16666.66 microseconds ~~ 16 milliseconds == 60 updates per second
	microSec lag(0);

	bool notFullScreen = false;
	while (window.isOpen())
	{
		sf::Event currEvent;
		while (window.pollEvent(currEvent)) {
			if (((currEvent.type == sf::Event::Closed) || (state == EXIT_GAME))
				|| sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
				window.close();
			}
			if (currEvent.type == sf::Event::Resized) {
				GLBVRS::SetGlobalConstants(window.getSize().x, window.getSize().y, &resources, &globalGame, globalGame.playerChar, soundLvl);
					settingsMenu.ResetButtons();
			}
			if (currEvent.type == sf::Event::LostFocus) {
				//window.create(sf::VideoMode(400, 400), "CONTAIN");
				GLBVRS::SetGlobalConstants(window.getSize().x, window.getSize().y, &resources, &globalGame, globalGame.playerChar, soundLvl);
				settingsMenu.ResetButtons();
				notFullScreen = true;
			}
		}
		if (window.hasFocus()) {
			if (notFullScreen) {
				notFullScreen = false;
				//window.create(sf::VideoMode(1920, 1080), "CONTAIN", sf::Style::Fullscreen);
				GLBVRS::SetGlobalConstants(window.getSize().x, window.getSize().y, &resources, &globalGame, globalGame.playerChar, soundLvl);
			}
			hiRes_time_point newTime = hiResTime::now();
			microSec currInterval = std::chrono::duration_cast<microSec>(newTime - currTime);
			currTime = newTime;
			lag += currInterval;
			//Set a max for lag if I wanna clamp
			if (lag > UPDATE_INTERVAL * (2)) {
				lag = UPDATE_INTERVAL * (2);
			}
			while (lag >= UPDATE_INTERVAL) {
				switch (state) {
				case MENU: {
					sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
					if (justSwitchedBackToMenu) {
						menu.ResetMenu();
						justSwitchedBackToMenu = false;
					}
					state = menu.Update(static_cast<float>(lag.count()), &window, mousePosition);
					if (state == SETTINGS) {
						justSwitchedBackToMenu = true;
					}
					break;
				}
				case SETTINGS: {
					sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
					if (justSwitchedBackToMenu) {
						settingsMenu.ResetMenu();
						justSwitchedBackToMenu = false;
					}
					state = settingsMenu.Update(static_cast<float>(lag.count()), &window, mousePosition);
					if (state == APPLY) {
						settingsMenu.ResetMenu();
						if (fullScreen) {
							window.create(GLBVRS::GetVideoMode(resolution), "CONTAIN", sf::Style::Fullscreen);
							GLBVRS::SetGlobalConstants(window.getSize().x, window.getSize().y, &resources, &globalGame, globalGame.playerChar, soundLvl);
							settingsMenu.ResetMenu();
						}
						else {
							window.create(GLBVRS::GetVideoMode(resolution), "CONTAIN", sf::Style::Default);
							GLBVRS::SetGlobalConstants(window.getSize().x, window.getSize().y, &resources, &globalGame, globalGame.playerChar, soundLvl);
							settingsMenu.ResetMenu();
						}
						resources.SetSoundLevel(soundLvl);
						GLBVRS::SetGlobalConstants(window.getSize().x, window.getSize().y, &resources, &globalGame, globalGame.playerChar, soundLvl);
						settingsMenu.ResetMenu();
						state = SETTINGS;
					}
					if (state == MENU) {
						justSwitchedBackToMenu = true;
					}
					break;
				}
				case IN_GAME: {
					sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
					// hiRes_time_point beforePhysicsUpdate = hiResTime::now();
					state = globalGame.Update(static_cast<float>(lag.count()), &window, mousePosition);
					// hiRes_time_point afterPhysicsUpdate = hiResTime::now();
					// microSec currInterval = std::chrono::duration_cast<microSec>(afterPhysicsUpdate - beforePhysicsUpdate);
					// std::string str = "Physics update took " + std::to_string(currInterval.count()) + " microseconds \n";
					// std::cout << str;
					if (state == LOSE) {
						saveData.SaveNewHighScore(globalGame.currRunScore);
						lostMenu.SetPrevScore(globalGame.currRunScore);
						lostMenu.SetHighScores(saveData.GetHighScoresVect());
					}
					else if (state == WIN) {
						saveData.SaveNewHighScore(globalGame.currRunScore);
						winMenu.SetPrevScore(globalGame.currRunScore);
						winMenu.SetHighScores(saveData.GetHighScoresVect());
					}
					break;
				}
				case WIN: {
					sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
					state = winMenu.Update(static_cast<float>(lag.count()), &window, mousePosition);
					if (state == MENU) {
						justSwitchedBackToMenu = true;
					}
					break;
				}
				case LOSE: {
					sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
					state = lostMenu.Update(static_cast<float>(lag.count()), &window, mousePosition);
					if (state == MENU) {
						justSwitchedBackToMenu = true;
					}
					break;
				}
				case START_GAME: {
					currLvl = 0;
					globalGame.InitGame(menu.GetDifficulty());
					//globalGame.loadTestLevel();
					state = IN_GAME;
					break;
				}
				case START_TUTORIAL: {
					currLvl = 0;
					globalGame.InitTutorial();
					state = IN_GAME;
					break;
				}
				case EXIT_GAME: {
					break;
				}
				}
				lag -= UPDATE_INTERVAL;
			}
			switch (state) {
			case IN_GAME: {
				float percentUpdateElapsed = static_cast<float>(lag.count()) / static_cast<float>(UPDATE_INTERVAL.count());
				// hiRes_time_point beforePhysicsUpdate = hiResTime::now();
				globalGame.Render(percentUpdateElapsed);
				// hiRes_time_point afterPhysicsUpdate = hiResTime::now();
				// microSec currInterval = std::chrono::duration_cast<microSec>(afterPhysicsUpdate - beforePhysicsUpdate);
				// std::string str = "Render took " + std::to_string(currInterval.count()) + " microseconds \n \n";
				// std::cout << str;
				break;
			}
			case MENU: {
				menu.Render(&window);
				break;
			}
			case SETTINGS: {
				settingsMenu.Render(&window);
				break;
			}
			case WIN: {
				winMenu.Render(&window);
				break;
			}
			case LOSE: {
				lostMenu.Render(&window);
				break;
			}
			}
		}
	}
	saveData.SaveDataToFile();
	return 0;
}