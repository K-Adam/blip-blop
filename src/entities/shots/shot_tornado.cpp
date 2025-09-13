#include "entities/shots/shot_tornado.h"

ShotTornado::ShotTornado(int vx) : dx(vx), speed_tick(0), nb_loops(0) { dy = 0; }

void ShotTornado::update() {
	anim_tick++;
	anim_tick %= 6;

	if (anim_tick == 0) {
		anim_step++;
		if ((anim_step > 5) && (nb_loops < LOOPS_MAX)) {
			anim_step = 0;
			nb_loops++;
		}
	}

	grav_tick += 1;
	if (dy > -7) {
		grav_tick %= TORNADO_GRAVITY_DELAY + 5 * dy;
	} else {
		grav_tick = 0;
	}

	if (grav_tick == 0) {
		dy -= 1;
	}

	y += dy;

	if (anim_step == 10) {
		destroyed = true;
		return;
	}

	if (x < offset - 100 || x > offset + 700 || y > 520 || y < -50) destroyed = true;

	speed_tick++;
	speed_tick %= TORNADO_ACCELERATION;
	if ((speed_tick == 0) && (abs(dx) < SPEED_MAX)) {
		if (dir == FACING_RIGHT) {
			dx++;
		} else {
			dx--;
		}
	}

	x += dx;

	if (dx <= 0) {
		pic = pbk_enemies[80 + anim_step];
	} else {
		pic = pbk_enemies[63 + anim_step];
	}
	if (anim_step <= 8) {
		colFromPic();
	}
}
