#pragma once

#include <algorithm>
#include <cstddef>
#include <span>

#include "entities/sprite.h"

// Loops the frames of one level bank animation
class BackgroundAnimated : public Sprite {
   public:
	BackgroundAnimated(const char* sheet, int latency, bool expire) : sheet_(sheet), latency_(latency), expire_(expire) {}

	void update() override {
		Flipbook frames = assets().pics.level.anim(sheet_);
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

// Plays a hand timed order of the frames of one level bank animation
class BackgroundSequence : public Sprite {
   public:
	BackgroundSequence(const char* sheet, std::span<const int> order, int delay) : sheet_(sheet), order_(order), delay_(delay) {}
	void update() override { pic = assets().pics.level.anim(sheet_)[animate(order_.data(), int(order_.size()), delay_)]; }

   private:
	const char* sheet_;
	std::span<const int> order_;
	int delay_;
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
		Flipbook frames = assets().pics.level.anim(SHEET.s);
		Clock& clock = level().shared<Clock>();
		if (clock.synchro != session().phase) {
			clock.synchro = session().phase;
			clock.tick = (clock.tick + 1) % LATENCY;
			if (clock.tick == 0) clock.frame = (clock.frame + 1) % frames.size();
		}

		pic = frames[clock.frame];
		if (x < level().camera.offset - 100) destroyed = true;
	}

   private:
	struct Clock {
		int frame = 0;
		int tick = 0;
		bool synchro = false;
	};
};

class BackgroundCube : public BackgroundSynced<"cube_spin", 5> {};
class BackgroundCoin : public BackgroundSynced<"coin_spin", 6> {};
class BackgroundRing : public BackgroundSynced<"ring_spin", 6> {};
