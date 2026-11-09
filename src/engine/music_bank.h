#pragma once

#include <memory>
#include <vector>

#include "core/debug.h"
#include "engine/fmod.h"

#define CHANNEL_NUMBER 32

class Music {
   public:
	virtual ~Music() {}
	virtual void Play() const = 0;
	virtual void Stop() const = 0;
	virtual void set_volume(int v) = 0;
};

class Mp3Music : public Music {
   public:
	Mp3Music(const std::string& fname, bool loop = true) {
		int loopflag = loop ? FSOUND_LOOP_NORMAL : FSOUND_LOOP_OFF;
		mp3_ = FSOUND_Stream_OpenFile(fname.c_str(), FSOUND_LOOP_NORMAL | loopflag, 0);
		if (!mp3_) {
			debug << "Mp3Music(\"" << fname << "\") failed\n";
			throw std::runtime_error("Mp3Music failed loading");
		}
	}

	~Mp3Music() override { FSOUND_Stream_Close(mp3_); }
	void Play() const override { FSOUND_Stream_Play(0, mp3_); }
	void Stop() const override { FSOUND_Stream_Stop(mp3_); }
	void set_volume(int) override { debug << "Mp3Music::set_volume is a noop\n"; }

   private:
	FSOUND_STREAM* mp3_;
};

class MusicBank {
   protected:
	std::vector<std::unique_ptr<Music>> musics_;

   public:
	void setVol(int v);
	bool open(const char* file, bool loop = true);
	void play(int n);
	void stop(int n);
	void stop();
};
