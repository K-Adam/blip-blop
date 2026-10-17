#include "entities/shots/shot_mario_fireball.h"

ShotMarioFireball::ShotMarioFireball(int vx) : accel_tick(0) {
	dy = 0;
	dx = vx;
}

void ShotMarioFireball::update() {
	if (x < offset - 100 || x > offset + 700 || y > 520 || y < -50) destroyed = true;

	accel_tick++;
	accel_tick %= DELAY_ACCELERATION;
	if (accel_tick == 0) {
		if (dir == FACING_RIGHT) {
			dx++;
		} else {
			dx--;
		}
	}

	x += dx;

	anim_tick++;
	anim_tick %= 4;
	if (anim_tick == 0) {
		anim_step++;
		anim_step %= 3;
	}

	if (dx > 12) {
		pic = pbk_enemies.anim("fireball_right_fast")[anim_step];
	} else if (dx > 6) {
		pic = pbk_enemies.anim("fireball_right_medium")[anim_step];
	} else if (dx > 0) {
		pic = pbk_enemies.anim("fireball_right_slow")[anim_step];
	}

	if (dx < -12) {
		pic = pbk_enemies.anim("fireball_left_fast")[anim_step];
	} else if (dx < -6) {
		pic = pbk_enemies.anim("fireball_left_medium")[anim_step];
	} else if (dx < 0) {
		pic = pbk_enemies.anim("fireball_left_slow")[anim_step];
	}

	colFromPic();
}
