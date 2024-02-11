#pragma once

#include "sound_bank.h"

class SoundBankBB : public SoundBank {
   public:
	void play(int n, int flags = 0);
	void stop(int n);
	void setVolume(int n, int vol);

	virtual bool loadSFX(const char* nom_fic);
};
