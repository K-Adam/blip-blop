#pragma once

#include <SDL2/SDL_mixer.h>

#include <memory>

#define SOUND_LOOP 1

// Which outputs play, the banks skip loading and playing what is off
class Audio {
	bool music_ = true;
	bool sound_ = true;

   public:
	// Turns everything off when the device cannot open
	bool init();
	bool music() const { return music_; }
	bool sound() const { return sound_; }
	void muteMusic() { music_ = false; }
};

extern Audio audio;

class Sound {
	std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)> chunk_{nullptr, &Mix_FreeChunk};
	int channel_ = -1;
	int loop_count_ = 0;

   public:
	void load(const char* file_path);
	void play(int flags = 0);
	void stop();
};
