#include "engine/sound.h"

#include "core/debug.h"

bool audio_init() {
	// Music is ogg only, sound effects still work without it
	if ((Mix_Init(MIX_INIT_OGG) & MIX_INIT_OGG) == 0) debug << "Mix_Init: " << Mix_GetError() << "\n";

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		debug << "Mix_OpenAudio: " << Mix_GetError() << "\n";
		return false;
	}
	return true;
}

void Sound::load(const char* file_path) {
	chunk_.reset(Mix_LoadWAV(file_path));
	if (!chunk_) debug << "Mix_LoadWAV(" << file_path << "): " << Mix_GetError() << "\n";
}

void Sound::play(int flags) {
	if (!chunk_) return;

	if (flags & SOUND_LOOP) {
		// Several sources may hold the same looping sound, it stops when the last one releases it
		if (loop_count_++ == 0) channel_ = Mix_PlayChannel(-1, chunk_.get(), -1);
	} else {
		channel_ = Mix_PlayChannel(-1, chunk_.get(), 0);
	}
}

void Sound::stop() {
	if (channel_ == -1) return;
	if (loop_count_ > 0 && --loop_count_ > 0) return;

	Mix_HaltChannel(channel_);
	channel_ = -1;
}
