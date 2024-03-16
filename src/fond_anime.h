#pragma once

#include "sprite.h"

class FondAnime : public Sprite {
   public:
	FondAnime(int first, int frames, int latency, bool expire) : first_(first), frames_(frames), latency_(latency), expire_(expire) {}

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

class FondBossventilo : public FondAnime {
   public:
	FondBossventilo() : FondAnime(60, 4, 3, true) {}
};

class FondDos : public FondAnime {
   public:
	FondDos() : FondAnime(58, 2, 40, true) {}
};

class FondDrapeau : public FondAnime {
   public:
	FondDrapeau() : FondAnime(31, 10, 5, false) {}
};

class FondEcranneige : public FondAnime {
   public:
	FondEcranneige() : FondAnime(45, 4, 4, true) {}
};

class FondTournesol1 : public FondAnime {
   public:
	FondTournesol1() : FondAnime(20, 2, 10, false) {}
};

class FondTournesol2 : public FondAnime {
   public:
	FondTournesol2() : FondAnime(22, 2, 10, false) {}
};

class FondTournesol3 : public FondAnime {
   public:
	FondTournesol3() : FondAnime(24, 2, 10, false) {}
};

class FondTournesol4 : public FondAnime {
   public:
	FondTournesol4() : FondAnime(26, 2, 10, false) {}
};

// All instances of one kind share the frame counter so they animate in step
template <int FIRST, int FRAMES, int LATENCY>
class FondSynchro : public Sprite {
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

class FondCube : public FondSynchro<0, 9, 5> {};
class FondPiece : public FondSynchro<9, 4, 6> {};
class FondRing : public FondSynchro<13, 4, 6> {};
