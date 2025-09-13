#pragma once

#include "entities/sprite.h"

class GoreFeetBisouZombie : public Sprite {
   public:
	void update() override {
		fall();

		if (anim_step < 3) {
			anim_tick += 1;
			anim_tick %= 6;

			if (anim_tick == 0) anim_step += 1;
		}

		if (dir == FACING_RIGHT) {
			walk(-1);
			pic = pbk_enemies[300 + anim_step];
		} else {
			walk(1);
			pic = pbk_enemies[304 + anim_step];
		}

		if (anim_step == 3 && platform_y(x, y) != 0) {
			engrave(x, y, pic);
			destroyed = true;
		}
	};
};
