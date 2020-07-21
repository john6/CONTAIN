#include "RESOURCES.h"



RESOURCES::RESOURCES()
{
	//font
	if (!m_font.loadFromFile("Font/kongtext.ttf")) {
		std::cerr << "error loading font \n";
	}
	//Beeps
	if (!m_bufferRegularBeep.loadFromFile("Sounds/BeepSounds/RegularBeep.wav")) {
		std::cerr << "error loading RegularBeep \n";
	}
	if (!m_bufferNegativeBeep.loadFromFile("Sounds/BeepSounds/NegativeBeep.wav")) {
		std::cerr << "error loading NegativeBeep \n";
	}
	if (!m_bufferOctaveBeep.loadFromFile("Sounds/BeepSounds/OctaveBeep.wav")) {
		std::cerr << "error loading OctaveBeep \n";
	}
	if (!m_buffer8BitExplosion.loadFromFile("Sounds/BeepSounds/8BitExplosion.wav")) {
		std::cerr << "error loading 8BitExplosion \n";
	}
	if (!m_bufferUpwardChromaticBeep.loadFromFile("Sounds/BeepSounds/UpwardChromaticBeep.wav")) {
		std::cerr << "error loading UpwardChromaticBeep \n";
	}
	//playful
	if (!m_bufferplayfulAttack.loadFromFile("Sounds/PlayFulSounds/Playful_Attack.wav")) {
		std::cerr << "error loading Playful_Attack.wav \n";
	}
	if (!m_bufferplayfulHealing.loadFromFile("Sounds/PlayFulSounds/Playful_Hit.wav")) {
		std::cerr << "error loading Playful_Hit.wav \n";
	}
	if (!m_bufferplayfulLevelUp.loadFromFile("Sounds/PlayFulSounds/Playful_LevelUp.wav")) {
		std::cerr << "error loading Playful_LevelUp.wav \n";
	}
	if (!m_bufferplayfulMajorPickup.loadFromFile("Sounds/PlayFulSounds/Playful_MajorPickup.wav")) {
		std::cerr << "error loading Playful_MajorPickup.wav \n";
	}
	if (!m_bufferplayfulPickup.loadFromFile("Sounds/PlayFulSounds/Playful_Pickup.wav")) {
		std::cerr << "error loading Playful_Pickup.wav \n";
	}
	//Yal's Sounds
	if (!Attack3.loadFromFile("Sounds/YalSounds/Attack3.wav")) {
		std::cerr << "error loading Attack3.wav \n";
	}
	if (!Attack4.loadFromFile("Sounds/YalSounds/Attack4.wav")) {
		std::cerr << "error loading Attack4.wav \n";
	}
	if (!bossexplode.loadFromFile("Sounds/YalSounds/bossexplode.wav")) {
		std::cerr << "error loading bossexplode.wav \n";
	}
	if (!bossexplode2.loadFromFile("Sounds/YalSounds/bossexplode2.wav")) {
		std::cerr << "error loading bossexplode2.wav \n";
	}
	if (!Buzzer6.loadFromFile("Sounds/YalSounds/Buzzer6.wav")) {
		std::cerr << "error loading Buzzer6.wav \n";
	}
	if (!Buzzer9.loadFromFile("Sounds/YalSounds/Buzzer9.wav")) {
		std::cerr << "error loading Buzzer9.wav \n";
	}
	if (!coin3.loadFromFile("Sounds/YalSounds/coin3.wav")) {
		std::cerr << "error loading coin3.wav \n";
	}
	if (!Explode5.loadFromFile("Sounds/YalSounds/Explode5.wav")) {
		std::cerr << "error loading .wav \n";
	}
	if (!Explode20.loadFromFile("Sounds/YalSounds/Explode20.wav")) {
		std::cerr << "error loading .wav \n";
	}
	if (!Explode20.loadFromFile("Sounds/YalSounds/Fire5.wav")) {
		std::cerr << "error loading Fire5.wav \n";
	}
	if (!laser14.loadFromFile("Sounds/YalSounds/laser14.wav")) {
		std::cerr << "error loading .wav \n";
	}
	if (!magic2.loadFromFile("Sounds/YalSounds/magic2.wav")) {
		std::cerr << "error loading .wav \n";
	}
	if (!magic6.loadFromFile("Sounds/YalSounds/magic6.wav")) {
		std::cerr << "error loading .wav \n";
	}
	if (!magic7.loadFromFile("Sounds/YalSounds/magic7.wav")) {
		std::cerr << "error loading .wav \n";
	}
	if (!magic8.loadFromFile("Sounds/YalSounds/magic8.wav")) {
		std::cerr << "error loading .wav \n";
	}
	if (!magic9.loadFromFile("Sounds/YalSounds/magic9.wav")) {
		std::cerr << "error loading .wav \n";
	}
	if (!magic10.loadFromFile("Sounds/YalSounds/magic10.wav")) {
		std::cerr << "error loading magic10.wav \n";
	}
	if (!MenuAccept2.loadFromFile("Sounds/YalSounds/MenuAccept2.wav")) {
		std::cerr << "error loading MenuAccept2.wav \n";
	}
	if (!MenuAccept3.loadFromFile("Sounds/YalSounds/MenuAccept3.wav")) {
		std::cerr << "error loading MenuAccept3.wav \n";
	}
	if (!MenuAccept6.loadFromFile("Sounds/YalSounds/MenuAccept6.wav")) {
		std::cerr << "error loading MenuAccept6.wav \n";
	}
	if (!ouch1.loadFromFile("Sounds/YalSounds/ouch1.wav")) {
		std::cerr << "error loading ouch1.wav \n";
	}
	if (!ouch3.loadFromFile("Sounds/YalSounds/ouch3.wav")) {
		std::cerr << "error loading ouch3.wav \n";
	}
	if (!PlayerDie13.loadFromFile("Sounds/YalSounds/PlayerDie13.wav")) {
		std::cerr << "error loading PlayerDie13.wav \n";
	}
	if (!PlayerDie13.loadFromFile("Sounds/YalSounds/PlayerDie13.wav")) {
		std::cerr << "error loading PlayerDie13.wav \n";
	}
	if (!Powerup4.loadFromFile("Sounds/YalSounds/Powerup4.wav")) {
		std::cerr << "error loading Powerup4.wav \n";
	}
	if (!shoot11.loadFromFile("Sounds/YalSounds/shoot11.wav")) {
		std::cerr << "error loading shoot11.wav \n";
	}
	if (!shoot14.loadFromFile("Sounds/YalSounds/shoot14.wav")) {
		std::cerr << "error loading shoot14.wav \n";
	}
	if (!shoot23.loadFromFile("Sounds/YalSounds/shoot23.wav")) {
		std::cerr << "error loading shoot23.wav \n";
	}
	if (!shoot35.loadFromFile("Sounds/YalSounds/shoot35.wav")) {
		std::cerr << "error loading shoot35.wav \n";
	}
	if (!shot2.loadFromFile("Sounds/YalSounds/shot2.wav")) {
		std::cerr << "error loading shot2.wav \n";
	}
	if (!shot17.loadFromFile("Sounds/YalSounds/shot17.wav")) {
		std::cerr << "error loading shot17.wav \n";
	}
	if (!steam3.loadFromFile("Sounds/YalSounds/steam3.wav")) {
		std::cerr << "error loading steam3.wav \n";
	}
	if (!sword5.loadFromFile("Sounds/YalSounds/sword5.wav")) {
		std::cerr << "error loading sword5.wav \n";
	}
	if (!warning3.loadFromFile("Sounds/YalSounds/warning3.wav")) {
		std::cerr << "error loading warning3.wav \n";
	}
	if (!warning7.loadFromFile("Sounds/YalSounds/warning7.wav")) {
		std::cerr << "error loading warning7.wav \n";
	}
	if (!Whoosh.loadFromFile("Sounds/YalSounds/Whoosh.wav")) {
		std::cerr << "error loading Whoosh.wav \n";
	}
	if (!Fireball.loadFromFile("Sounds/YalSounds/Fireball.wav")) {
		std::cerr << "error loading Fireball.wav \n";
	}
	if (!enemyDeath6.loadFromFile("Sounds/YalSounds/Enemy Death 6.wav")) {
		std::cerr << "error loading Enemy Death 6.wav \n";
	}
	if (!playershoot.loadFromFile("Sounds/YalSounds/playershoot.wav")) {
		std::cerr << "error loading player shoot .wav \n";
	}
	if (!Brake.loadFromFile("Sounds/YalSounds/Brake.wav")) {
		std::cerr << "error loading Brake.wav \n";
	}
	if (!Plop.loadFromFile("Sounds/YalSounds/Plop.wav")) {
		std::cerr << "error loading Plop.wav \n";
	}
	if (!PlopBanana.loadFromFile("Sounds/YalSounds/Plop Banana.wav")) {
		std::cerr << "error loading Plop Banana.wav \n";
	}
	if (!Sword_Clash.loadFromFile("Sounds/YalSounds/Sword Clash.wav")) {
		std::cerr << "error loading Sword Clash.wav \n";
	}
}

RESOURCES::~RESOURCES()
{
}


void RESOURCES::PlayMusicFromFile(int i_songNum)
{
	mtx.lock();
	switch (i_songNum) {
	case 0: {
		if (!music.openFromFile("Sounds/Music/BF980.wav")) {
			std::cerr << "error loading 'Sounds / Music / BF980.wav' \n";
		}
		break;
	}
	case 1: {
		if (!music.openFromFile("Sounds/Music/D2PAK.wav")) {
			std::cerr << "error loading 'Sounds / Music / D2PAK.wav' \n";
		}
		break;
	}
	case 2: {
		if (!music.openFromFile("Sounds/Music/EPROM.wav")) {
			std::cerr << "error loading 'Sounds / Music / EPROM.wav' \n";
		}
		break;
	}
	case 3: {
		if (!music.openFromFile("Sounds/Music/FinFET.wav")) {
			std::cerr << "error loading 'Sounds / Music / finFET.wav' \n";
		}
		break;
	}
	case 4: {
		if (!music.openFromFile("Sounds/Music/MOSFET.wav")) {
			std::cerr << "error loading 'Sounds / Music / MOSFET.wav' \n";
		}
		break;
	}
	case 5: {
		if (!music.openFromFile("Sounds/Music/TO-3.wav")) {
			std::cerr << "error loading 'Sounds / Music / TO-3.wav' \n";
		}
		break;
	}
	case 6: {
		if (!music.openFromFile("Sounds/Music/Vs Excite.wav")) {
			std::cerr << "error loading 'Sounds / Music / VS Excite.wav' \n";
		}
		break;
	}
	//THESE TWO FILES WONT LOAD:
			//Failed to open WAV sound file (invalid or unsupported file)
			//error loading 'Sounds / Music / CMOS.wav'
			//Failed to play audio stream: sound parameters have not been initialized (call initialize() first)
	//case 7: {
	//	if (!music.openFromFile("Sounds/Music/CMOS.wav")) {
	//		std::cerr << "error loading 'Sounds / Music / CMOS.wav' \n";
	//	}
	//	break;
	//}

	//case 8: {
	//	if (!music.openFromFile("Sounds/Music/MITS.wav")) {
	//		std::cerr << "error loading 'Sounds / Music / MITS.wav' \n";
	//	}
	//	break;
	//}
	}
	music.setLoop(true);
	music.play();
	mtx.unlock();
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
	case (ATTACK): {
		m_sound.setBuffer(m_bufferplayfulAttack);
		m_sound.play();
		break;
	}
	case (HEALING): {
		m_sound.setBuffer(m_bufferplayfulHealing);
		m_sound.play();
		break;
	}
	case (LEVELUP): {
		m_sound.setBuffer(m_bufferplayfulLevelUp);
		m_sound.play();
		break;
	}
	case (PLAYFULPICKUP): {
		m_sound.setBuffer(m_bufferplayfulPickup);
		m_sound.play();
		break;
	}
	case (MAJORPICKUP): {
		m_sound.setBuffer(m_bufferplayfulMajorPickup);
		m_sound.play();
		break;
	}
	case (PICKUP): {
		m_sound.setBuffer(m_bufferplayfulPickup);
		m_sound.play();
		break;
	}
	case (ATTACK3): {
		m_sound.setBuffer(Attack3);
		m_sound.play();
		break;
	}
	case (ATTACK4): {
		m_sound.setBuffer(Attack4);
		m_sound.play();
		break;
	}
	case (BOSSEXPLODE): {
		m_sound.setBuffer(bossexplode);
		m_sound.play();
		break;
	}
	case (BOSSEXPLODE2): {
		m_sound.setBuffer(bossexplode2);
		m_sound.play();
		break;
	}
	case (BUZZER6): {
		m_sound.setBuffer(Buzzer6);
		m_sound.play();
		break;
	}
	case (BUZZER9): {
		m_sound.setBuffer(Buzzer9);
		m_sound.play();
		break;
	}
	case (COIN3): {
		m_sound.setBuffer(coin3);
		m_sound.play();
		break;
	}
	case (EXPLODE5): {
		m_sound.setBuffer(Explode5);
		m_sound.play();
		break;
	}
	case (EXPLODE20): {
		m_sound.setBuffer(Explode20);
		m_sound.play();
		break;
	}
	case (FIRE5): {
		m_sound.setBuffer(Fire5);
		m_sound.play();
		break;
	}
	case (LASER14): {
		m_sound.setBuffer(laser14);
		m_sound.play();
		break;
	}
	case (MAGICAL2): {
		m_sound.setBuffer(magic2);
		m_sound.play();
		break;
	}
	case (MAGIC6): {
		m_sound.setBuffer(magic6);
		m_sound.play();
		break;
	}
	case (MAGIC7): {
		m_sound.setBuffer(magic7);
		m_sound.play();
		break;
	}
	case (MAGIC8): {
		m_sound.setBuffer(magic8);
		m_sound.play();
		break;
	}
	case (MAGIC9): {
		m_sound.setBuffer(magic9);
		m_sound.play();
		break;
	}
	case (MAGIC10): {
		m_sound.setBuffer(magic10);
		m_sound.play();
		break;
	}
	case (MENUACCEPT): {
		m_sound.setBuffer(MenuAccept);
		m_sound.play();
		break;
	}
	case (MENUACCEPT2): {
		m_sound.setBuffer(MenuAccept2);
		m_sound.play();
		break;
	}
	case (MENUACCEPT3): {
		m_sound.setBuffer(MenuAccept3);
		m_sound.play();
		break;
	}
	case (MENUACCEPT6): {
		m_sound.setBuffer(MenuAccept6);
		m_sound.play();
		break;
	}
	case (OUCH1): {
		m_sound.setBuffer(ouch1);
		m_sound.play();
		break;
	}
	case (OUCH3): {
		m_sound.setBuffer(ouch3);
		m_sound.play();
		break;
	}
	case (PLAYERDIE13): {
		m_sound.setBuffer(PlayerDie13);
		m_sound.play();
		break;
	}
	case (POWERUP4): {
		m_sound.setBuffer(Powerup4);
		m_sound.play();
		break;
	}
	case (SHOOT11): {
		m_sound.setBuffer(shoot11);
		m_sound.play();
		break;
	}
	case (SHOOT14): {
		m_sound.setBuffer(shoot14);
		m_sound.play();
		break;
	}
	case (SHOOT23): {
		m_sound.setBuffer(shoot23);
		m_sound.play();
		break;
	}
	case (SHOOT35): {
		m_sound.setBuffer(shoot35);
		m_sound.play();
		break;
	}
	case (SHOT2): {
		m_sound.setBuffer(shot2);
		m_sound.play();
		break;
	}
	case (STEAM3): {
		m_sound.setBuffer(steam3);
		m_sound.play();
		break;
	}
	case (SWORD5): {
		m_sound.setBuffer(sword5);
		m_sound.play();
		break;
	}
	case (WARNING3): {
		m_sound.setBuffer(warning3);
		m_sound.play();
		break;
	}
	case (WHOOSH): {
		m_sound.setBuffer(Whoosh);
		m_sound.play();
		break;
	}
	case (WARNING7): {
		m_sound.setBuffer(warning7);
		m_sound.play();
		break;
	}
	case (FIREBALL): {
		m_sound.setBuffer(Fireball);
		m_sound.play();
		break;
	}
	case (ENEMYDEATH6): {
		m_sound.setBuffer(enemyDeath6);
		m_sound.play();
		break;
	}
	case (BRAKE): {
		m_sound.setBuffer(Brake);
		m_sound.play();
		break;
	}	
	case (PLOPBANANA): {
		m_sound.setBuffer(PlopBanana);
		m_sound.play();
		break;
	}
	case (PLOP): {
		m_sound.setBuffer(Plop);
		m_sound.play();
		break;
	}
	case (SWORDCLASH): {
		m_sound.setBuffer(Sword_Clash);
		m_sound.play();
		break;
	}
	case (PLAYERSHOOT): {
		m_sound.setBuffer(Sword_Clash);
		m_sound.play();
		break;
	}
	
	default: {
		std::cerr << "soundNum not found \n";
	}
	}
	mtx.unlock();
}

