#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <thread>
#include <mutex>

class RESOURCES
{
private:
	sf::SoundBuffer m_bufferRegularBeep;
	sf::SoundBuffer m_bufferNegativeBeep;
	sf::SoundBuffer m_bufferOctaveBeep;
	sf::SoundBuffer m_buffer8BitExplosion;
	sf::SoundBuffer m_bufferUpwardChromaticBeep;
	sf::Sound m_sound;
	std::mutex mtx;

public:
	sf::Font m_font;

	RESOURCES();
	~RESOURCES();

	enum SOUNDS { REGULAR_BEEP, NEGATIVE_BEEP, OCTAVE_BEEP, EXPLOSION, CHROM_BEEP };

	sf::Font GetFont();

	void PlaySound(int soundNum);

};