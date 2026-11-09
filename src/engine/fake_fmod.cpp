#include "engine/fmod.h"

#include <SDL2/SDL_mixer.h>

#include <iostream>

extern "C" {
struct FSOUND_STREAM {
	Mix_Music* music;
};
struct FSOUND_SAMPLE {
	Mix_Chunk* chunk;
	int loop;
};

FSOUND_SAMPLE* FSOUND_Sample_Load_File(const char* path) {
	FSOUND_SAMPLE* sample = new FSOUND_SAMPLE;

	sample->loop = 0;
	sample->chunk = Mix_LoadWAV(path);
	if (!sample->chunk) {
		std::cerr << "Mix_LoadWAV: " << Mix_GetError() << "\n";
	}

	return sample;
}

FSOUND_SAMPLE* FSOUND_Sample_Load(int index, const char* buffer, unsigned int mode, int memlength) {
	FSOUND_SAMPLE* sample = new FSOUND_SAMPLE;

	sample->loop = 0;
	sample->chunk = Mix_LoadWAV_RW(SDL_RWFromConstMem(buffer, memlength), 1);
	if (!sample->chunk) {
		std::cerr << "Mix_LoadWAV_RW: " << Mix_GetError() << "\n";
	}

	return sample;
}
int FSOUND_PlaySound(int channel, FSOUND_SAMPLE* sptr) { return Mix_PlayChannel(channel, sptr->chunk, sptr->loop); }
signed char FSOUND_Sample_SetLoopMode(FSOUND_SAMPLE* sptr, unsigned int loopmode) {
	sptr->loop = loopmode == FSOUND_LOOP_NORMAL ? -1 : 0;
	return true;
}

signed char FSOUND_StopSound(int channel) {
	Mix_HaltChannel(channel);
	return true;
}

void FSOUND_Sample_Free(FSOUND_SAMPLE* sptr) {
	Mix_FreeChunk(sptr->chunk);
	delete sptr;
}

FSOUND_STREAM* FSOUND_Stream_OpenFile(const char* filename, unsigned int mode, int memlength) {
	FSOUND_STREAM* stream = new FSOUND_STREAM;
	stream->music = Mix_LoadMUS(filename);
	if (!stream->music) {
		std::cerr << "Mix_LoadMUS(" << filename << "): " << Mix_GetError() << "\n";
	}
	return stream;
}
int FSOUND_Stream_Play(int channel, FSOUND_STREAM* stream) {
	Mix_PlayMusic(stream->music, 0);
	return true;
}
signed char FSOUND_Stream_Stop(FSOUND_STREAM* stream) {
	Mix_HaltMusic();
	return true;
}
signed char FSOUND_Stream_Close(FSOUND_STREAM* stream) {
	Mix_FreeMusic(stream->music);
	return true;
}

signed char FSOUND_Init(int mixrate, int maxsoftwarechannels, unsigned int flags) {
	// Music is ogg only, sound effects still work without it
	if ((Mix_Init(MIX_INIT_OGG) & MIX_INIT_OGG) == 0) std::cerr << "Mix_Init: " << Mix_GetError() << "\n";

	if (Mix_OpenAudio(mixrate, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		std::cerr << "Mix_OpenAudio: " << Mix_GetError() << "\n";
		return 0;
	}
	return 1;
}
void FSOUND_Close() {
	Mix_CloseAudio();
	while (Mix_Init(0)) Mix_Quit();
}
}
