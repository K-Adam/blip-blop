#pragma once

#include <algorithm>
#include <cstddef>

#include "entities/sprite.h"

// Loops the frames of one pbk_level animation
class BackgroundAnimated : public Sprite {
   public:
	BackgroundAnimated(const char* sheet, int latency, bool expire) : sheet_(sheet), latency_(latency), expire_(expire) {}

	void update() override {
		Flipbook frames = pbk_level.anim(sheet_);
		anim_tick = (anim_tick + 1) % latency_;
		if (anim_tick == 0) anim_step = (anim_step + 1) % frames.size();

		pic = frames[anim_step];
		if (expire_) updateDestroyed();
	}

   private:
	const char* sheet_;
	int latency_;
	bool expire_;
};

class BackgroundBossFan : public BackgroundAnimated {
   public:
	BackgroundBossFan() : BackgroundAnimated("boss_fan", 3, true) {}
};

class BackgroundDos : public BackgroundAnimated {
   public:
	BackgroundDos() : BackgroundAnimated("dos_prompt", 40, true) {}
};

class BackgroundFlag : public BackgroundAnimated {
   public:
	BackgroundFlag() : BackgroundAnimated("flag_wave", 5, false) {}
};

class BackgroundSnowScreen : public BackgroundAnimated {
   public:
	BackgroundSnowScreen() : BackgroundAnimated("snow_screen", 4, true) {}
};

class BackgroundSunflower1 : public BackgroundAnimated {
   public:
	BackgroundSunflower1() : BackgroundAnimated("sunflower_1", 10, false) {}
};

class BackgroundSunflower2 : public BackgroundAnimated {
   public:
	BackgroundSunflower2() : BackgroundAnimated("sunflower_2", 10, false) {}
};

class BackgroundSunflower3 : public BackgroundAnimated {
   public:
	BackgroundSunflower3() : BackgroundAnimated("sunflower_3", 10, false) {}
};

class BackgroundSunflower4 : public BackgroundAnimated {
   public:
	BackgroundSunflower4() : BackgroundAnimated("sunflower_4", 10, false) {}
};

// Lets a string literal name the sheet as a template argument
template <std::size_t N>
struct SheetName {
	char s[N];
	constexpr SheetName(const char (&v)[N]) { std::copy_n(v, N, s); }
};

// All instances of one kind share the frame counter so they animate in step
template <SheetName SHEET, int LATENCY>
class BackgroundSynced : public Sprite {
   public:
	void update() override {
		Flipbook frames = pbk_level.anim(SHEET.s);
		if (synchro_ != phase) {
			synchro_ = phase;
			anim_tick_ = (anim_tick_ + 1) % LATENCY;
			if (anim_tick_ == 0) anim_frame_ = (anim_frame_ + 1) % frames.size();
		}

		pic = frames[anim_frame_];
		if (x < offset - 100) destroyed = true;
	}

   private:
	static inline int anim_frame_ = 0;
	static inline int anim_tick_ = 0;
	static inline bool synchro_ = false;
};

class BackgroundCube : public BackgroundSynced<"cube_spin", 5> {};
class BackgroundCoin : public BackgroundSynced<"coin_spin", 6> {};
class BackgroundRing : public BackgroundSynced<"ring_spin", 6> {};
