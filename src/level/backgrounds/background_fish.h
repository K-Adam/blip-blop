#pragma once

#include <cstdlib>

#include "entities/sprite.h"

class BackgroundFish : public Sprite {
   public:
	BackgroundFish(const char* right, const char* left) : right_(right), left_(left) { dir = rand() % 2; }

	void update() override {
		step(9, 3);
		x += dir ? 1 : -1;
		pic = pbk_level.anim(dir ? right_ : left_)[anim_step];
		if (x < offset - 100 || x > offset + 740) destroyed = true;
	}

   private:
	const char* right_;
	const char* left_;
};
