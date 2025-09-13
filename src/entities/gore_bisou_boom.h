#pragma once

#include "entities/sprite.h"

class GoreFeetBisouBoom : public Sprite {
   public:
	void update() override {
		fall();

		if (anim_step < 5) {
			anim_tick += 1;
			anim_tick %= 8;

			if (anim_tick == 0) anim_step += 1;

			if (dir == FACING_RIGHT)
				pic = pbk_enemies[190 + anim_step];
			else
				pic = pbk_enemies[196 + anim_step];
		}

		if ((platform_y(x, y) != 0 && anim_step >= 5) || solid_wall(x, y)) {
			engrave(x, y, pic);
			destroyed = true;
			sbk_misc.play(6 + (rand() % 2));
		} else {
			if (dir == FACING_RIGHT)
				x -= 2;
			else
				x += 2;
		}

		updateDestroyed();
	};
};

class GoreHeadBisouBoom : public Sprite {
   public:
	void update() override {
		fall();

		if (anim_step < 5) {
			anim_tick += 1;
			anim_tick %= 8;

			if (anim_tick == 0) anim_step += 1;

			if (dir == FACING_RIGHT)
				pic = pbk_enemies[202 + anim_step];
			else
				pic = pbk_enemies[208 + anim_step];
		}

		if ((platform_y(x, y) != 0 && anim_step >= 5) || solid_wall(x, y)) {
			engrave(x, y, pic);
			destroyed = true;
			sbk_misc.play(6 + (rand() % 2));
		} else {
			if (dir == FACING_RIGHT)
				x += 2;
			else
				x -= 2;
		}

		updateDestroyed();
	};
};

class GoreBellyBisouBoom : public Sprite {
   public:
	void update() override {
		fall();

		if (anim_step < 5) {
			anim_tick += 1;
			anim_tick %= 8;

			if (anim_tick == 0) anim_step += 1;

			if (dir == FACING_RIGHT)
				pic = pbk_enemies[214 + anim_step];
			else
				pic = pbk_enemies[220 + anim_step];
		}

		if ((platform_y(x, y) != 0 && anim_step >= 5) || solid_wall(x, y)) {
			engrave(x, y, pic);
			sbk_misc.play(6 + (rand() % 2));
			destroyed = true;
		} else {
			if (dir == FACING_RIGHT)
				x += 1;
			else
				x -= 1;
		}

		updateDestroyed();
	};
};
