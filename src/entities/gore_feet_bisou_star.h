#pragma once

#include "entities/sprite.h"

class GoreFeetBisouStar : public Sprite {
   public:
	int status;
	int nb;

	GoreFeetBisouStar() : status(0), nb(0) {}

	void update() override {
		static const int anim_right[] = {3, 4, 5, 4};
		static const int anim_left[] = {3, 4, 5, 4};

		if (status == 0) {
			fall();

			if (anim_step < 3) {
				anim_tick += 1;
				anim_tick %= 6;

				if (anim_tick == 0) anim_step += 1;
			}

			if (dir == FACING_RIGHT) {
				walk(-1);
				pic = pbk_enemies.anim("star_feet_right")[anim_step];
			} else {
				walk(1);
				pic = pbk_enemies.anim("star_feet_left")[anim_step];
			}

			if (anim_step == 3 && platform_y(x, y) != 0) {
				status = 1;
				anim_tick = anim_step = 0;
			}
		} else {
			nb += 1;

			if (dir == FACING_RIGHT)
				pic = pbk_enemies.anim("star_feet_right")[animate(anim_right, 4, 5)];
			else
				pic = pbk_enemies.anim("star_feet_left")[animate(anim_left, 4, 5)];

			if (nb >= 100) {
				engrave(x, y, pic);
				destroyed = true;
			}
		}
	};
};
