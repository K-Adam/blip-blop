#include "entities/shots/shot_rayman_fist.h"
#include "core/debug.h"

ShotRaymanFist::ShotRaymanFist(int dx0, int xc) : dx(dx0), x_target(xc) { anim_tick = 1; }

void ShotRaymanFist::update() {
	if (dir == FACING_RIGHT) {
		pic = pbk_enemies[230];
	} else {
		pic = pbk_enemies[231];
	}

	if ((dx != 0) && (((dir == FACING_RIGHT) && (x > x_target)) || ((dir == FACING_LEFT) && (x < x_target)))) {
		anim_tick++;
		anim_tick %= abs(dx);
	}
	if (anim_tick == 0) {
		if (dir == FACING_RIGHT) {
			dx -= 2;
		} else {
			dx += 2;
		}
	}

	x += dx;

	colFromPic();
}
