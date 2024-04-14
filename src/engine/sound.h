#pragma once

#include "engine/fmod.h"

#define SOUND_LOOP 1

class Sound {
   protected:
	FSOUND_SAMPLE* sample;

	int channel;
	int cpt_loop;

   public:
	Sound();
	~Sound();

	bool load(const char* fic);
	bool loadFromMem(void* ptr, int taille);
	void play(int flags = 0);
	void setVolume(int v);
	void stop();
	void close();
};

/*

bool DSOpenDirectSound(HWND wh, int coop = DSSCL_PRIORITY);
bool DSCreatePrimary(int f = 44100, int nb = 16);
void DSCloseDirectSound();

#ifndef BENSOUND_CPP_FILE
	extern IDirectSound *		dsound;
	extern IDirectSoundBuffer *	PrimarySBuffer;
#endif
*/
