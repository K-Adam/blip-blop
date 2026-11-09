#pragma once

#include "engine/fmod.h"

#define SOUND_LOOP 1

class Sound {
   protected:
	FSOUND_SAMPLE* sample;

	int channel;
	int loop_count;

   public:
	Sound();
	~Sound();

	bool load(const char* file_path);
	bool loadFromMem(void* ptr, int length);
	void play(int flags = 0);
	void setVolume(int v);
	void stop();
	void close();
};
