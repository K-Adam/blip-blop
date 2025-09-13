#include "entities/shots/shot_shaman_ball.h"

ShotShamanBall::ShotShamanBall() : ok(false), boom(false) {}

void ShotShamanBall::update() {
	if (!boom) {
		x += dx;
		y += dy;
	}

	anim_tick += 1;
	anim_tick %= 4;

	if (boom) {
		if (anim_tick == 0) {
			anim_step += 1;

			if (anim_step > 4) {
				destroyed = true;
				return;
			}
		}

		if (dx > 0)
			pic = pbk_enemies[384 + anim_step];
		else
			pic = pbk_enemies[389 + anim_step];

	} else if (ok) {
		if (anim_tick == 0) {
			anim_step += 1;
			anim_step %= 5;
		}

		if (solid_wall(x, y)) {
			anim_step = anim_tick = 0;
			boom = true;
		}

		if (dx > 0)
			pic = pbk_enemies[374 + anim_step];
		else
			pic = pbk_enemies[379 + anim_step];
	} else {
		if (anim_tick == 0) {
			anim_step += 1;

			if (anim_step > 4) {
				ok = true;
				return;
			}
		}

		if (dx > 0)
			pic = pbk_enemies[364 + anim_step];
		else
			pic = pbk_enemies[369 + anim_step];
	}

	colFromPic();
	updateDestroyed();
}
