#include "SaveData.h"

SaveData::SaveData()
{
	CheckForSaveFile();
	ReadInHighScores();
	CheckForSettingsFile();
	ReadInSettingsConfig();
}

SaveData::~SaveData()
{
}

void SaveData::CheckForSaveFile()
{
	std::ifstream SaveFileIn("SAVEDATA.txt");
	std::string saveText;
	std::getline(SaveFileIn, saveText);
	SaveFileIn.close();
	if (saveText.size() == 0) {
		CreateNewSaveFile();
	}
}

void SaveData::CheckForSettingsFile()
{
	std::ifstream SaveFileIn("SETTINGS.txt");
	std::string saveText;
	std::getline(SaveFileIn, saveText);
	SaveFileIn.close();
	if (saveText.size() == 0) {
		CreateNewSettingsFile();
	}
}

void SaveData::CreateNewSaveFile()
{
	std::ofstream SaveFileOut("SAVEDATA.txt");
	SaveFileOut << "SAVE_FILE_DONT_TOUCH_PLZ\n 0\n 0\n 0\n 0\n 0\n 0\n 0\n 0\n 0\n 0\n 0\n 0\n 0\n 0\n 0\n 0\n 0\n 0\n 0\n 0\n 0\n 0\n 0\n 0\n 0\n 0\n 0\n 0\n 0\n 0\n";
	SaveFileOut.close();
}

void SaveData::CreateNewSettingsFile()
{
	std::ofstream SaveFileOut("SETTINGS.txt");
	SaveFileOut << "SETTINGS_FILE_DONT_TOUCH_PLZ\n 0\n 0\n 0\n 0\n";
	SaveFileOut.close();
}

void SaveData::ReadInHighScores()
{
	std::ifstream SaveFileIn("SAVEDATA.txt");
	std::string saveText;
	std::vector<std::string> stringVect;
	while (std::getline(SaveFileIn, saveText)) {
		stringVect.push_back(saveText);
	}
	SaveFileIn.close();
	if (stringVect[0] == "SAVE_FILE_DONT_TOUCH_PLZ") {
		int i = 1;
		while (i < stringVect.size()) {
			int i_dec = std::stoi(stringVect[i]);
			highScores.push_back(i_dec);
			++i;
		}
	}
	else {
		std::cout << "There was an issue reading the save file\n";
	}
}

void SaveData::ReadInSettingsConfig()
{
	std::ifstream SaveFileIn("SETTINGS.txt");
	std::string saveText;
	std::vector<std::string> stringVect;
	while (std::getline(SaveFileIn, saveText)) {
		stringVect.push_back(saveText);
	}
	SaveFileIn.close();
	if (stringVect[0] == "SETTINGS_FILE_DONT_TOUCH_PLZ") {
		int i = 1;
		while (i < stringVect.size()) {
			int i_dec = std::stoi(stringVect[i]);
			settingsConfig.push_back(i_dec);
			++i;
		}
	}
	else {
		std::cout << "There was an issue reading the save file\n";
	}
}

void SaveData::SaveNewHighScore(int i_newScore)
{ //keeping high schore limit to 10, for visual style and ol' fashion sake
	highScores.push_back(i_newScore);
	highScores.sort();
	highScores.reverse();
	if (highScores.size() > 10) {
		int i = 0;
		auto iter = highScores.begin();
		while (i < 10) {
			++iter;
			++i;
		}
		highScores.erase(iter, highScores.end());
	}
}

void SaveData::EditSettingsConfig(int settingIndex, int newSetting)
{
	settingsConfig[settingIndex] = newSetting;
	SaveDataToFile();
}

void SaveData::SaveDataToFile()
{
	std::ofstream SaveFileOut;
	SaveFileOut.open("SAVEDATA.txt", std::ofstream::out | std::ofstream::trunc);
	
	SaveFileOut << "SAVE_FILE_DONT_TOUCH_PLZ\n";
	auto iter = highScores.begin();
	while (iter != highScores.end()) {
	SaveFileOut << std::to_string(iter._Ptr->_Myval) << "\n";
	++iter;
	}
	SaveFileOut.close();


	std::ofstream SettingsFileOut;
	SettingsFileOut.open("SETTINGS.txt", std::ofstream::out | std::ofstream::trunc);

	SettingsFileOut << "SETTINGS_FILE_DONT_TOUCH_PLZ\n";
	for (int i = 0; i < settingsConfig.size(); i++) {
		SettingsFileOut << std::to_string(settingsConfig[i]) << "\n";
	}
	SettingsFileOut.close();
}

std::string SaveData::GetHighScoresString()
{
	std::string highScoreString = "High Scores: ";
	for (int highScore : highScores) {
		highScoreString += " " + std::to_string(highScore);
	}
	return highScoreString;
}

std::list<int> SaveData::GetHighScoresVect()
{
	return highScores;
}

std::vector<int> SaveData::GetSettingsVect()
{
	return settingsConfig;
}


