#pragma once

#include "entities/sprite.h"

class GoreFeetBisou : public Sprite {
   public:
	GoreFeetBisou(const char* right, const char* left) : right_(right), left_(left) {}

	void update() override {
		fall();

		if (anim_step < 3) {
			anim_tick += 1;
			anim_tick %= 6;

			if (anim_tick == 0) anim_step += 1;
		}

		if (dir == FACING_RIGHT) {
			walk(-1);
			pic = assets().pics.enemies.anim(right_)[anim_step];
		} else {
			walk(1);
			pic = assets().pics.enemies.anim(left_)[anim_step];
		}

		if (anim_step == 3 && level().terrain.platform_y(x, y) != 0) {
			engrave(x, y, pic);
			destroyed = true;
		}
	};

   private:
	const char* right_;
	const char* left_;
};
