#pragma once

#include <SDL2/SDL_mixer.h>

#include <memory>

#define SOUND_LOOP 1

bool audio_init();

class Sound {
	std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)> chunk_{nullptr, &Mix_FreeChunk};
	int channel_ = -1;
	int loop_count_ = 0;

   public:
	void load(const char* file_path);
	void play(int flags = 0);
	void stop();
};
