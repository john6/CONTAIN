#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <chrono>
#include <ctime>    
#include "GLOBAL_CONSTANTS.h"
#include "RESOURCES.h"
#include "Game.h"
#include "Menu.h"
#include "SettingsMenu.h"
#include "YouWonMenu.h"
#include "YouLostMenu.h"
#include "SaveData.h"
#include "MessageBus.h"
#include "main.h"

//TODO: Contact points should be in local space I think, I never convert them

int main()
{
	//https://eigen.tuxfamily.org/dox//TopicMultiThreading.html
	Eigen::initParallel();
	SaveData saveData = SaveData();
	RSLTN resolution = NNTN_TN;
	float soundLvl = 50.0f;
	bool fullScreen = true;
	sf::RenderWindow window(GLBVRS::GetVideoMode(resolution), "CONTAIN", sf::Style::Fullscreen);

	sf::Cursor cursor;
	if (cursor.loadFromSystem(sf::Cursor::Cross)) {
		window.setMouseCursor(cursor);
	}

	RESOURCES resources;
	MessageBus mBus;
	GLBVRS::SetGlobalConstants(window.getSize().x, window.getSize().y, &resources, &mBus, NULL, NULL, soundLvl);
	Game globalGame = Game(&window, &resources);
	GLBVRS::SetGlobalConstants(window.getSize().x, window.getSize().y, &resources, &mBus, &globalGame, globalGame.playerChar, soundLvl);

	//View Code Test
	sf::View mainMenusView;
	mainMenusView.reset(sf::FloatRect(0, 0, GLBVRS::SCREEN_WIDTH, GLBVRS::SCREEN_HEIGHT));
	window.setView(mainMenusView);


	//sf::View hudView;
	//hudView.reset(sf::FloatRect(0, 0, 2592, 1458));
	//window.setView(hudView);
	//View Code Test
	std::cout << "Window Size x: " << window.getSize().x << "\n";
	std::cout << "Window Size y: " << window.getSize().y << "\n";

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
	Vector2f currSectorDimensions = Vector2f(0.0f ,0.0f );
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
				GLBVRS::SetGlobalConstants(window.getSize().x, window.getSize().y, &resources, &mBus, &globalGame, globalGame.playerChar, soundLvl);

				settingsMenu.ResetButtons();
			}
			if (currEvent.type == sf::Event::LostFocus) {
				GLBVRS::SetGlobalConstants(window.getSize().x, window.getSize().y, &resources, &mBus, &globalGame, globalGame.playerChar, soundLvl);
				settingsMenu.ResetButtons();
				notFullScreen = true;
			}
		}
		if (window.hasFocus()) {
			if (notFullScreen) {
				notFullScreen = false;
				GLBVRS::SetGlobalConstants(window.getSize().x, window.getSize().y, &resources, &mBus, &globalGame, globalGame.playerChar, soundLvl);
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
					sf::Vector2i mScreenPos = sf::Mouse::getPosition(window);
					sf::Vector2f mousePosition = window.mapPixelToCoords(mScreenPos, window.getView());
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
					sf::Vector2i mScreenPos = sf::Mouse::getPosition(window);
					sf::Vector2f mousePosition = window.mapPixelToCoords(mScreenPos, window.getView());
					if (justSwitchedBackToMenu) {
						window.setView(mainMenusView);
						justSwitchedBackToMenu = false;
					}
					state = settingsMenu.Update(static_cast<float>(lag.count()), &window, mousePosition);
					if (state == APPLY) {
						settingsMenu.ResetMenu();
						window.setView(mainMenusView);
						if (fullScreen) {
							window.create(GLBVRS::GetVideoMode(resolution), "CONTAIN", sf::Style::Fullscreen);
							GLBVRS::SetGlobalConstants(window.getSize().x, window.getSize().y, &resources, &mBus, &globalGame, globalGame.playerChar, soundLvl);
							window.setView(mainMenusView);
						}
						else {
							window.create(GLBVRS::GetVideoMode(resolution), "CONTAIN", sf::Style::Default);
							GLBVRS::SetGlobalConstants(window.getSize().x, window.getSize().y, &resources, &mBus, &globalGame, globalGame.playerChar, soundLvl);
							window.setView(mainMenusView);
						}
						resources.SetSoundLevel(soundLvl);
						GLBVRS::SetGlobalConstants(window.getSize().x, window.getSize().y, &resources, &mBus, &globalGame, globalGame.playerChar, soundLvl);
						window.setView(mainMenusView);
						state = SETTINGS;
					}
					if (state == MENU) {
						justSwitchedBackToMenu = true;
					}
					break;
				}
				case IN_GAME: {
					sf::Vector2i mScreenPos = sf::Mouse::getPosition(window);
					sf::Vector2f mousePosition = window.mapPixelToCoords(mScreenPos, window.getView());
					hiRes_time_point beforePhysicsUpdate = hiResTime::now();
					state = globalGame.Update(static_cast<float>(lag.count()), &window, mousePosition);
					hiRes_time_point afterPhysicsUpdate = hiResTime::now();
					
					Vector2f newFrameDimensions = globalGame.GetCurrSectorDimensions();

					//if ((currSectorDimensions[0] != newFrameDimensions[0]) || (currSectorDimensions[1] != newFrameDimensions[1])){
					//	currSectorDimensions = newFrameDimensions;
					//	worldView.setCenter((float)(((int)currSectorDimensions[0]) / 2), (float)(((int)currSectorDimensions[1]) / 2));
					//	window.setView(worldView);
					//}


					GLBVRS::MBUSPTR->notify();
					microSec currInterval = std::chrono::duration_cast<microSec>(afterPhysicsUpdate - beforePhysicsUpdate);
					std::string str = "Physics update took " + std::to_string(currInterval.count()) + " microseconds \n";
					std::cout << str;
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
					sf::Vector2i mScreenPos = sf::Mouse::getPosition(window);
					sf::Vector2f mousePosition = window.mapPixelToCoords(mScreenPos, window.getView());
					state = winMenu.Update(static_cast<float>(lag.count()), &window, mousePosition);
					if (state == MENU) {
						justSwitchedBackToMenu = true;
					}
					break;
				}
				case LOSE: {
					sf::Vector2i mScreenPos = sf::Mouse::getPosition(window);
					sf::Vector2f mousePosition = window.mapPixelToCoords(mScreenPos, window.getView());
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
				hiRes_time_point beforePhysicsUpdate = hiResTime::now();
				globalGame.Render(percentUpdateElapsed);
				hiRes_time_point afterPhysicsUpdate = hiResTime::now();
				microSec currInterval = std::chrono::duration_cast<microSec>(afterPhysicsUpdate - beforePhysicsUpdate);
				std::string str = "Render took " + std::to_string(currInterval.count()) + " microseconds \n \n";
				std::cout << str;
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