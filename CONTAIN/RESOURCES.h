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
	sf::SoundBuffer m_bufferplayfulPickup;
	sf::SoundBuffer m_bufferplayfulMajorPickup;


	//Yal's Sounds
	sf::SoundBuffer Attack3;
	sf::SoundBuffer Attack4;
	sf::SoundBuffer Explode4;
	sf::SoundBuffer Explode5;
	sf::SoundBuffer Explode7;
	sf::SoundBuffer Explode17;
	sf::SoundBuffer Explode25;
	sf::SoundBuffer Explode36;
	sf::SoundBuffer bossexplode;
	sf::SoundBuffer bossexplode2;
	sf::SoundBuffer BigEggCollect1;
	sf::SoundBuffer Buzzer6;
	sf::SoundBuffer Buzzer9;
	sf::SoundBuffer Brake;
	sf::SoundBuffer coin3;
	sf::SoundBuffer Explode20;
	sf::SoundBuffer Explode37;
	sf::SoundBuffer Engine2;
	sf::SoundBuffer enemyDeath6;
	sf::SoundBuffer Fire5;
	sf::SoundBuffer Fireball;
	sf::SoundBuffer GameOver;
	sf::SoundBuffer laser14;
	sf::SoundBuffer magic2;
	sf::SoundBuffer magic6;
	sf::SoundBuffer magic7;
	sf::SoundBuffer magic8;
	sf::SoundBuffer magic9;
	sf::SoundBuffer magic10;
	sf::SoundBuffer MenuAccept;
	sf::SoundBuffer MenuAccept2;
	sf::SoundBuffer MenuAccept3;
	sf::SoundBuffer MenuAccept6;
	sf::SoundBuffer ouch1;
	sf::SoundBuffer ouch3;
	sf::SoundBuffer PlayerDie13;
	sf::SoundBuffer playershoot;
	sf::SoundBuffer Plop;
	sf::SoundBuffer PlopBanana;
	sf::SoundBuffer Powerup4;
	sf::SoundBuffer Powerup11;
	sf::SoundBuffer shoot11;
	sf::SoundBuffer shoot14;
	sf::SoundBuffer shoot23;
	sf::SoundBuffer shoot35;
	sf::SoundBuffer shot2;
	sf::SoundBuffer shot17;
	sf::SoundBuffer steam3;
	sf::SoundBuffer success1;
	sf::SoundBuffer sword5;
	sf::SoundBuffer Sword_Clash;
	sf::SoundBuffer warning3;
	sf::SoundBuffer warning7;
	sf::SoundBuffer Whoosh;

	sf::Sound sound_1;
	sf::Sound sound_2;
	sf::Sound sound_3;
	sf::Sound sound_4;
	sf::Sound sound_5;
	sf::Sound sound_6;
	sf::Sound sound_7;
	sf::Sound sound_8;
	sf::Sound sound_arr[8];
	int nextSoundIndex;

	sf::Sound repeatEngineSound;
	sf::Sound repeatExplosionSound;

	bool musicIsOn;
	std::mutex mtx;

public:
	sf::Music music;
	sf::Font m_font;
	float soundLvl;

	RESOURCES();
	~RESOURCES();

	enum SOUNDS { REGULAR_BEEP, NEGATIVE_BEEP, OCTAVE_BEEP, EXPLODE4, BIGEGGCOLLECT1, EXPLOSION, EXPLODE5, EXPLODE7, EXPLODE17, EXPLODE25, EXPLODE36, ENGINE2, CHROM_BEEP, ATTACK,
		HEALING, BRAKE, LEVELUP, PLAYFULPICKUP, MAJORPICKUP, PICKUP, ATTACK3, ATTACK4, BOSSEXPLODE, 
		BOSSEXPLODE2, BUZZER6, BUZZER9, COIN3, EXPLODE20, ENEMYDEATH6, FIRE5, FIREBALL, GAMEOVER, LASER14, MAGICAL2,
		MAGIC6, MAGIC7, MAGIC8, MAGIC9, MAGIC10, MENUACCEPT, MENUACCEPT2, MENUACCEPT3, MENUACCEPT6, 
		OUCH1, OUCH3, PLAYERSHOOT, PLAYERDIE13, POWERUP4, SHOOT11, SHOOT14, SHOOT23, 
		SHOOT35, SHOOT3, SHOT2, STEAM3, SUCCESS1, SWORD5, WARNING3, WARNING7, WHOOSH, PLOPBANANA, PLOP, SWORDCLASH};

	sf::Font GetFont();

	void PlaySound(int soundNum);

	void PlayMusicFromFile(int i_songNum);

	void SetSoundLevel(float i_soundLvl);

	void playEngineSound(bool i_play);

	void PlayExplosionSound(bool i_play);

	void TurnMusicOn(bool i_music);
};