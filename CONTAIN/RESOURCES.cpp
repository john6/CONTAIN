#include "RESOURCES.h"



RESOURCES::RESOURCES()
{
	if (!m_bufferRegularBeep.loadFromFile("Sounds/RegularBeep.wav")) {
		std::cerr << "error loading RegularBeep \n";
	}
	if (!m_bufferNegativeBeep.loadFromFile("Sounds/NegativeBeep.wav")) {
		std::cerr << "error loading NegativeBeep \n";
	}
	if (!m_bufferOctaveBeep.loadFromFile("Sounds/OctaveBeep.wav")) {
		std::cerr << "error loading OctaveBeep \n";
	}
	if (!m_buffer8BitExplosion.loadFromFile("Sounds/8BitExplosion.wav")) {
		std::cerr << "error loading 8BitExplosion \n";
	}
	if (!m_bufferUpwardChromaticBeep.loadFromFile("Sounds/UpwardChromaticBeep.wav")) {
		std::cerr << "error loading UpwardChromaticBeep \n";
	}
	if (!m_font.loadFromFile("Font/kongtext.ttf")) {
		std::cerr << "error loading font \n";
	}
}

RESOURCES::~RESOURCES()
{
}


sf::Font RESOURCES::GetFont() {
	return m_font;
}

void RESOURCES::PlaySound(int soundNum) {
	mtx.lock();
	switch (soundNum) {
	case (REGULAR_BEEP): {
		m_sound.setBuffer(m_bufferRegularBeep);
		m_sound.play();
		break;
	}
	case (NEGATIVE_BEEP): {
		m_sound.setBuffer(m_bufferNegativeBeep);
		m_sound.play();
		break;
	}
	case (OCTAVE_BEEP): {
		m_sound.setBuffer(m_bufferOctaveBeep);
		m_sound.play();
		break;
	}
	case (EXPLOSION): {
		m_sound.setBuffer(m_buffer8BitExplosion);
		m_sound.play();
		break;
	}
	case (CHROM_BEEP): {
		m_sound.setBuffer(m_bufferUpwardChromaticBeep);
		m_sound.play();
		break;
	}
	default: {
		std::cerr << "soundNum not found \n";
	}
	}
	mtx.unlock();
}