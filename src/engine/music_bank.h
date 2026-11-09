#pragma once

#include <SDL2/SDL_mixer.h>

#include <memory>
#include <vector>

class MusicBank {
	std::vector<std::unique_ptr<Mix_Music, decltype(&Mix_FreeMusic)>> musics_;
	bool loop_ = true;

   public:
	bool open(const char* file, bool loop = true);
	void play(int n);
	void stop();
};
