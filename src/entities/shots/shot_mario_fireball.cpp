#include "entities/shots/shot_mario_fireball.h"

ShotMarioFireball::ShotMarioFireball(int vx) : speed_etape(0) {
	dy = 0;
	dx = vx;
}

void ShotMarioFireball::update() {
	if (x < offset - 100 || x > offset + 700 || y > 520 || y < -50) destroyed = true;

	speed_etape++;
	speed_etape %= DELAY_ACCELERATION;
	if (speed_etape == 0) {
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
		pic = pbk_enemies[27 + anim_step];
	} else if (dx > 6) {
		pic = pbk_enemies[24 + anim_step];
	} else if (dx > 0) {
		pic = pbk_enemies[21 + anim_step];
	}

	if (dx < -12) {
		pic = pbk_enemies[18 + anim_step];
	} else if (dx < -6) {
		pic = pbk_enemies[15 + anim_step];
	} else if (dx < 0) {
		pic = pbk_enemies[12 + anim_step];
	}

	colFromPic();
}
