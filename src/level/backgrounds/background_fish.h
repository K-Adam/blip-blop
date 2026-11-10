#pragma once

#include <cstdlib>

#include "entities/sprite.h"

class BackgroundFish : public Sprite {
   public:
	BackgroundFish(const char* right, const char* left) : right_(right), left_(left) { dir = rand() % 2; }

	void update() override {
		step(9, 3);
		x += dir ? 1 : -1;
		pic = assets().pics.level.anim(dir ? right_ : left_)[anim_step];
		if (x < level().camera.offset - 100 || x > level().camera.offset + 740) destroyed = true;
	}

   private:
	const char* right_;
	const char* left_;
};
