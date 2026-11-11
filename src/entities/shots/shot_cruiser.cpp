#include "entities/shots/shot_cruiser.h"

ShotCruiser::ShotCruiser(int vx, int vy, const Picture* p) : dx(vx) {
	pic = p;
	dy = vy;
	anim_step = 0;
	anim_tick = 0;
}

void ShotCruiser::update() {
	if ((dy >= 0) && (y + dy > 43)) {
		y = 43;
		anim_tick++;
		anim_tick %= 4;
		if (anim_tick == 0) {
			anim_step++;
			if (anim_step == 3) {
				destroyed = true;
			} else {
				pic = assets().pics.level.anim("cruiser_hit_bottom")[anim_step];
			}
		}
	}

	else if ((dy <= 0) && (y - dy < 20)) {
		y = 18;
		anim_tick++;
		anim_tick %= 4;
		if (anim_tick == 0) {
			anim_step++;
			if (anim_step == 3) {
				destroyed = true;
			} else {
				pic = assets().pics.level.anim("cruiser_hit_top")[anim_step];
			}
		}
	} else {
		x += dx;
		y += dy;
	}
	updateDestroyed();
}
