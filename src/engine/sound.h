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
