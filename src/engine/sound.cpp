#define BENSOUND_CPP_FILE

#include <fcntl.h>
#include "core/debug.h"
#include "engine/sound.h"

Sound::Sound() : sample(NULL), channel(-1) {}

Sound::~Sound() { close(); }

bool Sound::load(const char* file_path) {
	sample = FSOUND_Sample_Load_File(file_path);

	if (sample == NULL) return false;

	return true;
}

bool Sound::loadFromMem(void* ptr, int length) {
	sample = FSOUND_Sample_Load(FSOUND_FREE, (char*)ptr, FSOUND_LOADMEMORY | FSOUND_LOOP_OFF, length);

	if (sample == NULL) {
		debug << "Sound::loadFromMem\n";
		return false;
	}

	channel = -1;
	loop_count = 0;

	return true;
}

void Sound::play(int flags) {
	if (flags & SOUND_LOOP) {
		FSOUND_Sample_SetLoopMode(sample, FSOUND_LOOP_NORMAL);

		if (loop_count++ == 0) {
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
		if (loop_count > 0) {
			if (--loop_count == 0) {
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
