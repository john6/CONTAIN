#pragma once
#include <iostream>
#include <fstream>
#include <list>
#include <SFML/Graphics.hpp> 
#include <SFML/Audio.hpp>
// I HAVE NO CLUE WHY I HAVE TO HAVE THESE HERE FOR MY FILE STREAM TO WORK WHAT THE FUCK IS GOING ON

class SaveData
{
private:
	std::string saveFileName;
	std::list<int> highScores;

	std::string settingsFileName;
	std::vector<int> settingsConfig;

public:
	SaveData();
	~SaveData();

	void CheckForSaveFile();

	void CheckForSettingsFile();

	void CreateNewSaveFile();

	void CreateNewSettingsFile();

	void ReadInHighScores();

	void ReadInSettingsConfig();

	void SaveNewHighScore(int i_newScore);

	void EditSettingsConfig(int settingIndex, int newSetting);

	void SaveDataToFile();

	std::string GetHighScoresString();

	std::list<int> GetHighScoresVect();

	std::vector<int> GetSettingsVect();
};

