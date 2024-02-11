#pragma once

// The original game used FMOD 3. These are the calls it makes, implemented on SDL_mixer in fake_fmod.cpp.

extern "C" {

typedef struct FSOUND_SAMPLE FSOUND_SAMPLE;
typedef struct FSOUND_STREAM FSOUND_STREAM;

#define FSOUND_LOOP_OFF 0x00000001
#define FSOUND_LOOP_NORMAL 0x00000002
#define FSOUND_LOADMEMORY 0x00008000
#define FSOUND_FREE -1

signed char FSOUND_Init(int mixrate, int maxsoftwarechannels, unsigned int flags);
void FSOUND_Close();

FSOUND_SAMPLE* FSOUND_Sample_Load_File(const char* path);
FSOUND_SAMPLE* FSOUND_Sample_Load(int index, const char* buffer, unsigned int mode, int memlength);
signed char FSOUND_Sample_SetLoopMode(FSOUND_SAMPLE* sptr, unsigned int loopmode);
void FSOUND_Sample_Free(FSOUND_SAMPLE* sptr);
int FSOUND_PlaySound(int channel, FSOUND_SAMPLE* sptr);
signed char FSOUND_StopSound(int channel);

FSOUND_STREAM* FSOUND_Stream_OpenFile(const char* filename, unsigned int mode, int memlength);
int FSOUND_Stream_Play(int channel, FSOUND_STREAM* stream);
signed char FSOUND_Stream_Stop(FSOUND_STREAM* stream);
signed char FSOUND_Stream_Close(FSOUND_STREAM* stream);
}
