#pragma once

#include "entities/sprite.h"

class GoreFeetBisouStar : public Sprite {
   public:
	int status;
	int nb;

	GoreFeetBisouStar() : status(0), nb(0) {}

	void update() override {
		static const int anim_droite[] = {97, 98, 99, 98};
		static const int anim_gauche[] = {103, 104, 105, 104};

		if (status == 0) {
			fall();

			if (anim_step < 3) {
				anim_tick += 1;
				anim_tick %= 6;

				if (anim_tick == 0) anim_step += 1;
			}

			if (dir == FACING_RIGHT) {
				marche(-1);
				pic = pbk_enemies[94 + anim_step];
			} else {
				marche(1);
				pic = pbk_enemies[100 + anim_step];
			}

			if (anim_step == 3 && platform_y(x, y) != 0) {
				status = 1;
				anim_tick = anim_step = 0;
			}
		} else {
			nb += 1;

			if (dir == FACING_RIGHT)
				pic = pbk_enemies[animate(anim_droite, 4, 5)];
			else
				pic = pbk_enemies[animate(anim_gauche, 4, 5)];

			if (nb >= 100) {
				engrave(x, y, pic);
				destroyed = true;
			}
		}
	};
};
