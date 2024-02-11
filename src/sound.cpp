#define BENSOUND_CPP_FILE

#include <fcntl.h>
#include "ben_debug.h"
#include "sound.h"

Sound::Sound() : sample(NULL), channel(-1) {}

Sound::~Sound() { close(); }

bool Sound::load(const char* fic) {
	sample = FSOUND_Sample_Load_File(fic);

	if (sample == NULL) return false;

	return true;
}

bool Sound::loadFromMem(void* ptr, int taille) {
	sample = FSOUND_Sample_Load(FSOUND_FREE, (char*)ptr, FSOUND_LOADMEMORY | FSOUND_LOOP_OFF, taille);

	if (sample == NULL) {
		debug << "Sound::loadFromMem\n";
		return false;
	}

	channel = -1;
	cpt_loop = 0;

	return true;
}

void Sound::play(int flags) {
	if (flags & SOUND_LOOP) {
		FSOUND_Sample_SetLoopMode(sample, FSOUND_LOOP_NORMAL);

		if (cpt_loop++ == 0) {
			channel = FSOUND_PlaySound(FSOUND_FREE, sample);
		}
	} else {
		FSOUND_Sample_SetLoopMode(sample, FSOUND_LOOP_OFF);
		channel = FSOUND_PlaySound(FSOUND_FREE, sample);
	}
}

void Sound::setVolume(int v) {}

void Sound::stop() {
	if (channel != -1) {
		if (cpt_loop > 0) {
			if (--cpt_loop == 0) {
				FSOUND_StopSound(channel);
				channel = -1;
			}
		} else {
			FSOUND_StopSound(channel);
			channel = -1;
		}
	}
}

void Sound::close() { FSOUND_Sample_Free(sample); }
