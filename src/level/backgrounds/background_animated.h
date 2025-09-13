#pragma once

#include "entities/sprite.h"

class BackgroundAnimated : public Sprite {
   public:
	BackgroundAnimated(int first, int frames, int latency, bool expire) : first_(first), frames_(frames), latency_(latency), expire_(expire) {}

	void update() override {
		anim_tick = (anim_tick + 1) % latency_;
		if (anim_tick == 0) anim_step = (anim_step + 1) % frames_;

		pic = pbk_level[first_ + anim_step];
		if (expire_) updateDestroyed();
	}

   private:
	int first_, frames_, latency_;
	bool expire_;
};

class BackgroundBossFan : public BackgroundAnimated {
   public:
	BackgroundBossFan() : BackgroundAnimated(60, 4, 3, true) {}
};

class BackgroundDos : public BackgroundAnimated {
   public:
	BackgroundDos() : BackgroundAnimated(58, 2, 40, true) {}
};

class BackgroundFlag : public BackgroundAnimated {
   public:
	BackgroundFlag() : BackgroundAnimated(31, 10, 5, false) {}
};

class BackgroundSnowScreen : public BackgroundAnimated {
   public:
	BackgroundSnowScreen() : BackgroundAnimated(45, 4, 4, true) {}
};

class BackgroundSunflower1 : public BackgroundAnimated {
   public:
	BackgroundSunflower1() : BackgroundAnimated(20, 2, 10, false) {}
};

class BackgroundSunflower2 : public BackgroundAnimated {
   public:
	BackgroundSunflower2() : BackgroundAnimated(22, 2, 10, false) {}
};

class BackgroundSunflower3 : public BackgroundAnimated {
   public:
	BackgroundSunflower3() : BackgroundAnimated(24, 2, 10, false) {}
};

class BackgroundSunflower4 : public BackgroundAnimated {
   public:
	BackgroundSunflower4() : BackgroundAnimated(26, 2, 10, false) {}
};

// All instances of one kind share the frame counter so they animate in step
template <int FIRST, int FRAMES, int LATENCY>
class BackgroundSynced : public Sprite {
   public:
	void update() override {
		if (synchro_ != phase) {
			synchro_ = phase;
			anim_tick_ = (anim_tick_ + 1) % LATENCY;
			if (anim_tick_ == 0) anim_frame_ = (anim_frame_ + 1) % FRAMES;
		}

		pic = pbk_level[FIRST + anim_frame_];
		if (x < offset - 100) destroyed = true;
	}

   private:
	static inline int anim_frame_ = 0;
	static inline int anim_tick_ = 0;
	static inline bool synchro_ = false;
};

class BackgroundCube : public BackgroundSynced<0, 9, 5> {};
class BackgroundCoin : public BackgroundSynced<9, 4, 6> {};
class BackgroundRing : public BackgroundSynced<13, 4, 6> {};
