#pragma once

#include "engine/sound_bank.h"

class SoundBankBB : public SoundBank {
   public:
	void play(int n, int flags = 0);
	void stop(int n);
	void setVolume(int n, int vol);

	bool loadSFX(const char* file_name) override;
};
