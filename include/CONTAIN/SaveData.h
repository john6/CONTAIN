#pragma once
#include <iostream>
#include <fstream>
#include <list>
#include <SFML/Graphics.hpp> 
#include <SFML/Audio.hpp>
// I HAVE NO FUCKING CLUE WHY I HAVE TO HAVE THESE HERE FOR MY FILE STREAM TO WORK WHAT THE FUCK IS GOING ON

class SaveData
{
private:
	std::string saveFileName;
	std::list<int> highScores;

public:
	SaveData();
	~SaveData();

	void CheckForSaveFile();

	void CreateNewSaveFile();

	void ReadInHighScores();

	void SaveNewHighScore(int i_newScore);

	void SaveDataToFile();

	std::string GetHighScoresString();

	std::list<int> GetHighScoresVect();
};

