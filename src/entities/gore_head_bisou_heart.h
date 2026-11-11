#pragma once

#include "entities/sprite.h"

class GoreHeadBisouHeart : public Sprite {
   public:
	int dx;
	bool play_sound;

	GoreHeadBisouHeart() : play_sound(false) {
		dy = -1 - rnd() % 3;
		dx = -2 + rnd() % 5;
	};

	void update() override {
		fall();

		anim_tick += 1;
		anim_tick %= 5;

		if (level().terrain.platform_y(x, y) == 0) {
			x += dx;
		}

		if (anim_tick == 0 && (anim_step < 3 || level().terrain.platform_y(x, y) != 0)) {
			if (!play_sound && level().terrain.platform_y(x, y) != 0) {
				assets().sounds.misc.play(6 + (rnd() % 2));
				play_sound = true;
			}

			anim_step += 1;

			if (anim_step > 6) {
				engrave(x, y, pic);
				destroyed = true;
				return;
			}
		}

		pic = facing(assets().pics.enemies, "heart_head")[anim_step];
	};
};
