#pragma once

#include "entities/sprite.h"

class GoreFeetBisouSky : public Sprite {
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
			pic = pbk_enemies.anim("sky_feet_right")[anim_step];
		} else {
			walk(1);
			pic = pbk_enemies.anim("sky_feet_left")[anim_step];
		}

		if (anim_step == 3 && platform_y(x, y) != 0) {
			engrave(x, y, pic);
			destroyed = true;
		}
	};
};
