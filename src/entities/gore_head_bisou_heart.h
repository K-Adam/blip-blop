#pragma once

#include "entities/sprite.h"

class GoreHeadBisouHeart : public Sprite {
   public:
	int dx;
	bool play_sound;

	GoreHeadBisouHeart() : play_sound(false) {
		dy = -1 - rand() % 3;
		dx = -2 + rand() % 5;
	};

	void update() override {
		fall();

		anim_tick += 1;
		anim_tick %= 5;

		if (platform_y(x, y) == 0) {
			x += dx;
		}

		if (anim_tick == 0 && (anim_step < 3 || platform_y(x, y) != 0)) {
			if (!play_sound && platform_y(x, y) != 0) {
				sbk_misc.play(6 + (rand() % 2));
				play_sound = true;
			}

			anim_step += 1;

			if (anim_step > 6) {
				engrave(x, y, pic);
				destroyed = true;
				return;
			}
		}

		if (dir == FACING_RIGHT)
			pic = pbk_enemies[447 + anim_step];
		else
			pic = pbk_enemies[454 + anim_step];
	};
};
