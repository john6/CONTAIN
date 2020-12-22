#include "RESOURCES.h"



RESOURCES::RESOURCES()
{
	//Sound basics
	soundLvl = 75.0f;
	musicIsOn = true;
	
	sound_arr[0] = sound_1;
	sound_arr[1] = sound_2;
	sound_arr[2] = sound_3;
	sound_arr[3] = sound_4;
	sound_arr[4] = sound_5;
	sound_arr[5] = sound_6;
	sound_arr[6] = sound_7;
	sound_arr[7] = sound_8;
	nextSoundIndex = 0;



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
	//m_sound.setVolume(i_soundLvl);
	//soundLvl = i_soundLvl;
	soundLvl = i_soundLvl;
	for (int i = 0; i < 8; i++) {
		sound_arr[i].setVolume(soundLvl);
	}
}

void RESOURCES::TurnMusicOn(bool i_music)
{
	musicIsOn = i_music;
}

sf::Font RESOURCES::GetFont() {
	return m_font;
}

void RESOURCES::PlaySound(int soundNum) { 
	//getting weird sound errors rn, I think it has to do with the fact that Im switching sound buffers all the time, might be the double access Im gonna turn mutex back on and see if there are still occasional errors
	mtx.lock();
	sf::Sound* currSound = &sound_arr[nextSoundIndex];
	nextSoundIndex = (nextSoundIndex + 1) % 7;
	switch (soundNum) {
	case (REGULAR_BEEP): {
		currSound->setBuffer(m_bufferRegularBeep);
		currSound->play();
		break;
	}
	case (NEGATIVE_BEEP): {
		currSound->setBuffer(m_bufferNegativeBeep);
		currSound->play();
		break;
	}
	case (OCTAVE_BEEP): {
		currSound->setBuffer(m_bufferOctaveBeep);
		currSound->play();
		break;
	}
	case (EXPLOSION): {
		currSound->setBuffer(m_buffer8BitExplosion);
		currSound->play();
		break;
	}
	case (CHROM_BEEP): {
		currSound->setBuffer(m_bufferUpwardChromaticBeep);
		currSound->play();
		break;
	}
	case (ATTACK): {
		currSound->setBuffer(m_bufferplayfulAttack);
		currSound->play();
		break;
	}
	case (HEALING): {
		currSound->setBuffer(m_bufferplayfulHealing);
		currSound->play();
		break;
	}
	case (LEVELUP): {
		currSound->setBuffer(m_bufferplayfulLevelUp);
		currSound->play();
		break;
	}
	case (PLAYFULPICKUP): {
		currSound->setBuffer(m_bufferplayfulPickup);
		currSound->play();
		break;
	}
	case (MAJORPICKUP): {
		currSound->setBuffer(m_bufferplayfulMajorPickup);
		currSound->play();
		break;
	}
	case (PICKUP): {
		currSound->setBuffer(m_bufferplayfulPickup);
		currSound->play();
		break;
	}
	case (ATTACK3): {
		currSound->setBuffer(Attack3);
		currSound->play();
		break;
	}
	case (ATTACK4): {
		currSound->setBuffer(Attack4);
		currSound->play();
		break;
	}
	case (BOSSEXPLODE): {
		currSound->setBuffer(bossexplode);
		currSound->play();
		break;
	}
	case (BOSSEXPLODE2): {
		currSound->setBuffer(bossexplode2);
		currSound->play();
		break;
	}
	case (BUZZER6): {
		currSound->setBuffer(Buzzer6);
		currSound->play();
		break;
	}
	case (BUZZER9): {
		currSound->setBuffer(Buzzer9);
		currSound->play();
		break;
	}
	case (COIN3): {
		currSound->setBuffer(coin3);
		currSound->play();
		break;
	}
	case (EXPLODE5): {
		currSound->setBuffer(Explode5);
		currSound->play();
		break;
	}
	case (EXPLODE20): {
		currSound->setBuffer(Explode20);
		currSound->play();
		break;
	}
	case (FIRE5): {
		currSound->setBuffer(Fire5);
		currSound->play();
		break;
	}
	case (LASER14): {
		currSound->setBuffer(laser14);
		currSound->play();
		break;
	}
	case (MAGICAL2): {
		currSound->setBuffer(magic2);
		currSound->play();
		break;
	}
	case (MAGIC6): {
		currSound->setBuffer(magic6);
		currSound->play();
		break;
	}
	case (MAGIC7): {
		currSound->setBuffer(magic7);
		currSound->play();
		break;
	}
	case (MAGIC8): {
		currSound->setBuffer(magic8);
		currSound->play();
		break;
	}
	case (MAGIC9): {
		currSound->setBuffer(magic9);
		currSound->play();
		break;
	}
	case (MAGIC10): {
		currSound->setBuffer(magic10);
		currSound->play();
		break;
	}
	case (MENUACCEPT): {
		currSound->setBuffer(MenuAccept);
		currSound->play();
		break;
	}
	case (MENUACCEPT2): {
		currSound->setBuffer(MenuAccept2);
		currSound->play();
		break;
	}
	case (MENUACCEPT3): {
		currSound->setBuffer(MenuAccept3);
		currSound->play();
		break;
	}
	case (MENUACCEPT6): {
		currSound->setBuffer(MenuAccept6);
		currSound->play();
		break;
	}
	case (OUCH1): {
		currSound->setBuffer(ouch1);
		currSound->play();
		break;
	}
	case (OUCH3): {
		currSound->setBuffer(ouch3);
		currSound->play(); 
		break;
	}
	case (PLAYERDIE13): {
		currSound->setBuffer(PlayerDie13);
		currSound->play();
		break;
	}
	case (POWERUP4): {
		currSound->setBuffer(Powerup4);
		currSound->play();
		break;
	}
	case (SHOOT11): {
		currSound->setBuffer(shoot11);
		currSound->play();
		break;
	}
	case (SHOOT14): {
		currSound->setBuffer(shoot14);
		currSound->play();
		break;
	}
	case (SHOOT23): {
		currSound->setBuffer(shoot23);
		currSound->play();
		break;
	}
	case (SHOOT35): {
		currSound->setBuffer(shoot35);
		currSound->play();
		break;
	}
	case (SHOT2): {
		currSound->setBuffer(shot2);
		currSound->play();
		break;
	}
	case (STEAM3): {
		currSound->setBuffer(steam3);
		currSound->play();
		break;
	}
	case (SWORD5): {
		currSound->setBuffer(sword5);
		currSound->play();
		break;
	}
	case (WARNING3): {
		currSound->setBuffer(warning3);
		currSound->play();
		break;
	}
	case (WHOOSH): {
		currSound->setBuffer(Whoosh);
		currSound->play();
		break;
	}
	case (WARNING7): {
		currSound->setBuffer(warning7);
		currSound->play();
		break;
	}
	case (FIREBALL): {
		currSound->setBuffer(Fireball);
		currSound->play();
		break;
	}
	case (ENEMYDEATH6): {
		currSound->setBuffer(enemyDeath6);
		currSound->play();
		break;
	}
	case (BRAKE): {
		currSound->setBuffer(Brake);
		currSound->play();
		break;
	}	
	case (PLOPBANANA): {
		currSound->setBuffer(PlopBanana);
		currSound->play();
		break;
	}
	case (PLOP): {
		currSound->setBuffer(Plop);
		currSound->play();
		break;
	}
	case (SWORDCLASH): {
		currSound->setBuffer(Sword_Clash);
		currSound->play();
		break;
	}
	case (PLAYERSHOOT): {
		currSound->setBuffer(Sword_Clash);
		currSound->play();
		break;
	}
	
	default: {
		std::cerr << "soundNum not found \n";
	}
	}
	mtx.unlock();
}

