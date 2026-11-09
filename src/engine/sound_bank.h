#pragma once

#include <vector>

#include "engine/sound.h"

class SoundBank {
	std::vector<Sound> tab_;

   public:
	bool loadSFX(const char* file_name);
	void play(int n, int flags = 0);
	void stop(int n);
};
