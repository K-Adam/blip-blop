#include "entities/shots/shot_cruiser_laser.h"

ShotCruiserLaser::ShotCruiserLaser(int vx, const Picture* p) : dx(vx) {
	pic = p;
	anim_step = 0;
	anim_tick = 2;
}

void ShotCruiserLaser::update() {
	anim_tick++;
	anim_tick %= 4;
	if (anim_tick == 0) anim_step++;
	if (anim_step == 4) {
		destroyed = true;
	} else {
		switch (npic) {
			case 10:
				if ((anim_step == 1) && (anim_tick == 0)) {
					x += 36;
					y += 19;
				}
				break;

			case 11:
				if ((anim_step == 1) && (anim_tick == 0)) {
					x += 19;
					y += 19;
				}
				break;

			case 12:
				if ((anim_step == 1) && (anim_tick == 0)) {
					x += 10;
					y += 19;
				}
				break;

			case 13:

				if ((anim_step == 1) && (anim_tick == 0)) y += 19;

				break;

			case 14:
				if ((anim_step == 1) && (anim_tick == 0)) {
					x -= 36;
					y -= 19;
				}
				break;

			case 15:
				if ((anim_step == 1) && (anim_tick == 0)) {
					x -= 19;
					y -= 19;
				}
				break;

			case 16:
				if ((anim_step == 1) && (anim_tick == 0)) {
					x -= 10;
					y -= 19;
				}
				break;

			case 17:

				if ((anim_step == 1) && (anim_tick == 0)) y -= 19;

				break;
		}

		if (anim_step > 0) {
			if (dx > 0) {
				pic = pbk_level.anim("cruiser_hit_bottom")[anim_step - 1];
			} else {
				pic = pbk_level.anim("cruiser_hit_top")[anim_step - 1];
			}
		}
	}
}
