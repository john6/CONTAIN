#include "RESOURCES.h"



RESOURCES::RESOURCES()
{
	soundLvl = 75.0f;
	musicIsOn = true;
	//font
	if (!m_font.loadFromFile("Font/kongtext.ttf")) {
		std::cerr << "error loading font \n";
	}
	//Beeps
	if (!m_bufferRegularBeep.loadFromFile("Sounds/BeepSounds/RegularBeep.ogg")) {
		std::cerr << "error loading RegularBeep \n";
	}
	if (!m_bufferNegativeBeep.loadFromFile("Sounds/BeepSounds/NegativeBeep.ogg")) {
		std::cerr << "error loading NegativeBeep \n";
	}
	if (!m_bufferOctaveBeep.loadFromFile("Sounds/BeepSounds/OctaveBeep.ogg")) {
		std::cerr << "error loading OctaveBeep \n";
	}
	if (!m_buffer8BitExplosion.loadFromFile("Sounds/BeepSounds/8BitExplosion.ogg")) {
		std::cerr << "error loading 8BitExplosion \n";
	}
	if (!m_bufferUpwardChromaticBeep.loadFromFile("Sounds/BeepSounds/UpwardChromaticBeep.ogg")) {
		std::cerr << "error loading UpwardChromaticBeep \n";
	}
	//playful
	if (!m_bufferplayfulAttack.loadFromFile("Sounds/PlayFulSounds/Playful_Attack.ogg")) {
		std::cerr << "error loading Playful_Attack.ogg \n";
	}
	if (!m_bufferplayfulHealing.loadFromFile("Sounds/PlayFulSounds/Playful_Hit.ogg")) {
		std::cerr << "error loading Playful_Hit.ogg \n";
	}
	if (!m_bufferplayfulLevelUp.loadFromFile("Sounds/PlayFulSounds/Playful_LevelUp.ogg")) {
		std::cerr << "error loading Playful_LevelUp.ogg \n";
	}
	if (!m_bufferplayfulMajorPickup.loadFromFile("Sounds/PlayFulSounds/Playful_MajorPickup.ogg")) {
		std::cerr << "error loading Playful_MajorPickup.ogg \n";
	}
	if (!m_bufferplayfulPickup.loadFromFile("Sounds/PlayFulSounds/Playful_Pickup.ogg")) {
		std::cerr << "error loading Playful_Pickup.ogg \n";
	}
	//Yal's Sounds
	if (!Attack3.loadFromFile("Sounds/YalSounds/Attack3.ogg")) {
		std::cerr << "error loading Attack3.ogg \n";
	}
	if (!Attack4.loadFromFile("Sounds/YalSounds/Attack4.ogg")) {
		std::cerr << "error loading Attack4.ogg \n";
	}
	if (!bossexplode.loadFromFile("Sounds/YalSounds/bossexplode.ogg")) {
		std::cerr << "error loading bossexplode.ogg \n";
	}
	if (!bossexplode2.loadFromFile("Sounds/YalSounds/bossexplode2.ogg")) {
		std::cerr << "error loading bossexplode2.ogg \n";
	}
	if (!Buzzer6.loadFromFile("Sounds/YalSounds/Buzzer6.ogg")) {
		std::cerr << "error loading Buzzer6.ogg \n";
	}
	if (!Buzzer9.loadFromFile("Sounds/YalSounds/Buzzer9.ogg")) {
		std::cerr << "error loading Buzzer9.ogg \n";
	}
	if (!coin3.loadFromFile("Sounds/YalSounds/coin3.ogg")) {
		std::cerr << "error loading coin3.ogg \n";
	}
	if (!Explode5.loadFromFile("Sounds/YalSounds/Explode5.ogg")) {
		std::cerr << "error loading .ogg \n";
	}
	if (!Explode20.loadFromFile("Sounds/YalSounds/Explode20.ogg")) {
		std::cerr << "error loading .ogg \n";
	}
	if (!Explode20.loadFromFile("Sounds/YalSounds/Fire5.ogg")) {
		std::cerr << "error loading Fire5.ogg \n";
	}
	if (!laser14.loadFromFile("Sounds/YalSounds/laser14.ogg")) {
		std::cerr << "error loading .ogg \n";
	}
	if (!magic2.loadFromFile("Sounds/YalSounds/magic2.ogg")) {
		std::cerr << "error loading .ogg \n";
	}
	if (!magic6.loadFromFile("Sounds/YalSounds/magic6.ogg")) {
		std::cerr << "error loading .ogg \n";
	}
	if (!magic7.loadFromFile("Sounds/YalSounds/magic7.ogg")) {
		std::cerr << "error loading .ogg \n";
	}
	if (!magic8.loadFromFile("Sounds/YalSounds/magic8.ogg")) {
		std::cerr << "error loading .ogg \n";
	}
	if (!magic9.loadFromFile("Sounds/YalSounds/magic9.ogg")) {
		std::cerr << "error loading .ogg \n";
	}
	if (!magic10.loadFromFile("Sounds/YalSounds/magic10.ogg")) {
		std::cerr << "error loading magic10.ogg \n";
	}
	if (!MenuAccept2.loadFromFile("Sounds/YalSounds/MenuAccept2.ogg")) {
		std::cerr << "error loading MenuAccept2.ogg \n";
	}
	if (!MenuAccept3.loadFromFile("Sounds/YalSounds/MenuAccept3.ogg")) {
		std::cerr << "error loading MenuAccept3.ogg \n";
	}
	if (!MenuAccept6.loadFromFile("Sounds/YalSounds/MenuAccept6.ogg")) {
		std::cerr << "error loading MenuAccept6.ogg \n";
	}
	if (!ouch1.loadFromFile("Sounds/YalSounds/ouch1.ogg")) {
		std::cerr << "error loading ouch1.ogg \n";
	}
	if (!ouch3.loadFromFile("Sounds/YalSounds/ouch3.ogg")) {
		std::cerr << "error loading ouch3.ogg \n";
	}
	if (!PlayerDie13.loadFromFile("Sounds/YalSounds/PlayerDie13.ogg")) {
		std::cerr << "error loading PlayerDie13.ogg \n";
	}
	if (!PlayerDie13.loadFromFile("Sounds/YalSounds/PlayerDie13.ogg")) {
		std::cerr << "error loading PlayerDie13.ogg \n";
	}
	if (!Powerup4.loadFromFile("Sounds/YalSounds/Powerup4.ogg")) {
		std::cerr << "error loading Powerup4.ogg \n";
	}
	if (!shoot11.loadFromFile("Sounds/YalSounds/shoot11.ogg")) {
		std::cerr << "error loading shoot11.ogg \n";
	}
	if (!shoot14.loadFromFile("Sounds/YalSounds/shoot14.ogg")) {
		std::cerr << "error loading shoot14.ogg \n";
	}
	if (!shoot23.loadFromFile("Sounds/YalSounds/shoot23.ogg")) {
		std::cerr << "error loading shoot23.ogg \n";
	}
	if (!shoot35.loadFromFile("Sounds/YalSounds/shoot35.ogg")) {
		std::cerr << "error loading shoot35.ogg \n";
	}
	if (!shot2.loadFromFile("Sounds/YalSounds/shot2.ogg")) {
		std::cerr << "error loading shot2.ogg \n";
	}
	if (!shot17.loadFromFile("Sounds/YalSounds/shot17.ogg")) {
		std::cerr << "error loading shot17.ogg \n";
	}
	if (!steam3.loadFromFile("Sounds/YalSounds/steam3.ogg")) {
		std::cerr << "error loading steam3.ogg \n";
	}
	if (!sword5.loadFromFile("Sounds/YalSounds/sword5.ogg")) {
		std::cerr << "error loading sword5.ogg \n";
	}
	if (!warning3.loadFromFile("Sounds/YalSounds/warning3.ogg")) {
		std::cerr << "error loading warning3.ogg \n";
	}
	if (!warning7.loadFromFile("Sounds/YalSounds/warning7.ogg")) {
		std::cerr << "error loading warning7.ogg \n";
	}
	if (!Whoosh.loadFromFile("Sounds/YalSounds/Whoosh.ogg")) {
		std::cerr << "error loading Whoosh.ogg \n";
	}
	if (!Fireball.loadFromFile("Sounds/YalSounds/Fireball.ogg")) {
		std::cerr << "error loading Fireball.ogg \n";
	}
	if (!enemyDeath6.loadFromFile("Sounds/YalSounds/Enemy Death 6.ogg")) {
		std::cerr << "error loading Enemy Death 6.ogg \n";
	}
	if (!playershoot.loadFromFile("Sounds/YalSounds/playershoot.ogg")) {
		std::cerr << "error loading player shoot .ogg \n";
	}
	if (!Brake.loadFromFile("Sounds/YalSounds/Brake.ogg")) {
		std::cerr << "error loading Brake.ogg \n";
	}
	if (!Plop.loadFromFile("Sounds/YalSounds/Plop.ogg")) {
		std::cerr << "error loading Plop.ogg \n";
	}
	if (!PlopBanana.loadFromFile("Sounds/YalSounds/Plop Banana.ogg")) {
		std::cerr << "error loading Plop Banana.ogg \n";
	}
	if (!Sword_Clash.loadFromFile("Sounds/YalSounds/Sword Clash.ogg")) {
		std::cerr << "error loading Sword Clash.ogg \n";
	}
}

RESOURCES::~RESOURCES()
{
}


void RESOURCES::PlayMusicFromFile(int i_songNum)
{
	mtx.lock();
	if (musicIsOn) {
		music.setVolume(soundLvl);
		switch (i_songNum) {
		case 0: {
			if (!music.openFromFile("Sounds/Music/BF980.ogg")) {
				std::cerr << "error loading 'Sounds / Music / BF980.ogg' \n";
			}
			break;
		}
		case 1: {
			if (!music.openFromFile("Sounds/Music/CMOS.ogg")) {
				std::cerr << "error loading 'Sounds / Music / CMOS.ogg' \n";
			}
			break;
		}
		case 2: {
			if (!music.openFromFile("Sounds/Music/D2PAK.ogg")) {
				std::cerr << "error loading 'Sounds / Music / D2PAK.ogg' \n";
			}
			break;
		}
		case 3: {
			if (!music.openFromFile("Sounds/Music/EPROM.ogg")) {
				std::cerr << "error loading 'Sounds / Music / EPROM.ogg' \n";
			}
			break;
		}
		case 4: {
			if (!music.openFromFile("Sounds/Music/FinFET.ogg")) {
				std::cerr << "error loading 'Sounds / Music / finFET.ogg' \n";
			}
			break;
		}
		case 5: {
			if (!music.openFromFile("Sounds/Music/MITS.ogg")) {
				std::cerr << "error loading 'Sounds / Music / MITS.ogg' \n";
			}
			break;
		}
		case 6: {
			if (!music.openFromFile("Sounds/Music/MOSFET.ogg")) {
				std::cerr << "error loading 'Sounds / Music / MOSFET.ogg' \n";
			}
			break;
		}
		case 7: {
			if (!music.openFromFile("Sounds/Music/TO-3.ogg")) {
				std::cerr << "error loading 'Sounds / Music / TO-3.ogg' \n";
			}
			break;
		}
		case 8: {
			if (!music.openFromFile("Sounds/Music/Vs Excite.ogg")) {
				std::cerr << "error loading 'Sounds / Music / VS Excite.ogg' \n";
			}
			break;
		}
		}
		music.setLoop(true);
		music.play();
	}
	mtx.unlock();
}

void RESOURCES::SetSoundLevel(float i_soundLvl)
{
	m_sound.setVolume(i_soundLvl);
	soundLvl = i_soundLvl;
}

void RESOURCES::TurnMusicOn(bool i_music)
{
	musicIsOn = i_music;
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

