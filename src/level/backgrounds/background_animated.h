#pragma once

#include <algorithm>
#include <cstddef>
#include <span>

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

// Plays a hand timed order of the frames of one pbk_level animation
class BackgroundSequence : public Sprite {
   public:
	BackgroundSequence(const char* sheet, std::span<const int> order, int delay) : sheet_(sheet), order_(order), delay_(delay) {}
	void update() override { pic = pbk_level.anim(sheet_)[animate(order_.data(), int(order_.size()), delay_)]; }

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
