#include "entities/shots/shot_mario_fireball_vertical.h"

ShotMarioFireballVertical::ShotMarioFireballVertical(int vx, int xc) : dx(vx), x_target(xc) { dy = -12; }

void ShotMarioFireballVertical::update() {
	if (x < offset - 100 || x > offset + 700 || y > 520 || y < -250) destroyed = true;
	if (dy > 0) {
		fireball_fall();
	} else {
		y += dy;
	}

	if (y < -50) {
		x = x_target;
		dy = 1;
	}

	if (dy < 0) x += dx;

	anim_tick++;
	anim_tick %= 4;
	if (anim_tick == 0) {
		anim_step++;
		anim_step %= 3;
	}

	if (dy > 12) {
		pic = pbk_enemies.anim("fireball_down_fast")[anim_step];
	} else if (dy > 6) {
		pic = pbk_enemies.anim("fireball_down_medium")[anim_step];
	} else if (dy > 0) {
		pic = pbk_enemies.anim("fireball_down_slow")[anim_step];
	}

	if (dy < -12) {
		pic = pbk_enemies.anim("fireball_up_fast")[anim_step];
	} else if (dy < -6) {
		pic = pbk_enemies.anim("fireball_up_medium")[anim_step];
	} else if (dy < 0) {
		pic = pbk_enemies.anim("fireball_up_slow")[anim_step];
	}

	colFromPic();
}

void ShotMarioFireballVertical::fireball_fall() {
	grav_tick += 1;
	grav_tick %= FIREBALL_GRAVITY_DELAY;

	if (grav_tick == 0 && dy < GRAVITY_FIREBALL) dy += 1;

	if (dy < 0 && solid_wall(x, y + dy)) dy = GRAVITY_FIREBALL;

	y += dy;
}
