#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <thread>
#include <mutex>

class RESOURCES
{
private:
	//Beeps
	sf::SoundBuffer m_bufferRegularBeep;
	sf::SoundBuffer m_bufferNegativeBeep;
	sf::SoundBuffer m_bufferOctaveBeep;
	sf::SoundBuffer m_buffer8BitExplosion;
	sf::SoundBuffer m_bufferUpwardChromaticBeep;
	//Playful Sounds
	sf::SoundBuffer m_bufferplayfulAttack;
	sf::SoundBuffer m_bufferplayfulHealing;
	sf::SoundBuffer m_bufferplayfulLevelUp;
	sf::SoundBuffer m_bufferplayfulMajorPickup;
	sf::SoundBuffer m_bufferplayfulPickup;

	//Yal's Sounds
	sf::SoundBuffer Attack3;
	sf::SoundBuffer Attack4;
	sf::SoundBuffer bossexplode;
	sf::SoundBuffer bossexplode2;
	sf::SoundBuffer Buzzer6;
	sf::SoundBuffer Buzzer9;
	sf::SoundBuffer coin3;
	sf::SoundBuffer Explode5;
	sf::SoundBuffer Explode20;
	sf::SoundBuffer laser14;
	sf::SoundBuffer magic2;
	sf::SoundBuffer magic6;
	sf::SoundBuffer magic7;
	sf::SoundBuffer magic8;
	sf::SoundBuffer magic9;
	sf::SoundBuffer magic10;
	sf::SoundBuffer ouch1;
	sf::SoundBuffer ouch3;
	sf::SoundBuffer PlayerDie13;
	sf::SoundBuffer shoot11;
	sf::SoundBuffer shoot14;
	sf::SoundBuffer shoot23;
	sf::SoundBuffer shoot35;
	sf::SoundBuffer shot2;
	sf::SoundBuffer steam3;
	sf::SoundBuffer sword5;
	sf::SoundBuffer warning3;
	sf::SoundBuffer Whoosh;

	sf::Sound m_sound;
	sf::Music music;
	std::mutex mtx;

public:
	sf::Font m_font;

	RESOURCES();
	~RESOURCES();

	enum SOUNDS { REGULAR_BEEP, NEGATIVE_BEEP, OCTAVE_BEEP, EXPLOSION, CHROM_BEEP, ATTACK,
		HEALING, LEVELUP, MAJORPICKUP, PICKUP, ATTACK3, ATTACK4, BOSSEXPLODE, BOSSEXPLODE2,
		BUZZER6, BUZZER9, COIN3, EXPLODE5, EXPLODE20, LASER14, MAGICAL2, MAGIC6, MAGIC7,
		MAGIC8, MAGIC9, MAGIC10, OUCH1, OUCH3, PLAYERDIE13, SHOOT11, SHOOT14, SHOOT23, 
		SHOOT35, SHOT2, STEAM3, SWORD5, WARNING3, WHOOSH};

	sf::Font GetFont();

	void PlaySound(int soundNum);

	void PlayMusicFromFile(int i_songNum);
};