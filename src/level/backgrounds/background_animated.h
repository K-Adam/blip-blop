#pragma once

#include "entities/sprite.h"

class BackgroundAnimated : public Sprite {
   public:
	BackgroundAnimated(int first, int frames, int latency, bool expire) : first_(first), frames_(frames), latency_(latency), expire_(expire) {}

	void update() override {
		ss_etape = (ss_etape + 1) % latency_;
		if (ss_etape == 0) etape = (etape + 1) % frames_;

		pic = pbk_niveau[first_ + etape];
		if (expire_) updateADetruire();
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
			ss_etape_ = (ss_etape_ + 1) % LATENCY;
			if (ss_etape_ == 0) etape_ = (etape_ + 1) % FRAMES;
		}

		pic = pbk_niveau[FIRST + etape_];
		if (x < offset - 100) a_detruire = true;
	}

   private:
	static inline int etape_ = 0;
	static inline int ss_etape_ = 0;
	static inline bool synchro_ = false;
};

class BackgroundCube : public BackgroundSynced<0, 9, 5> {};
class BackgroundCoin : public BackgroundSynced<9, 4, 6> {};
class BackgroundRing : public BackgroundSynced<13, 4, 6> {};
