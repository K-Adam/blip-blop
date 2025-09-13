#include "entities/shots/shot_snorky_base.h"

const int tir_snorky_oscilation[] = {0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 2, 2, 2, 1, 1, 1, 0, 0, 0};

ShotSnorkyBase::ShotSnorkyBase(int vx) : speed_etape(0), oscilation_etape(0) {
	dy = 0;
	dx = vx;
}

void ShotSnorkyBase::update() {
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

	oscilation_etape++;
	oscilation_etape %= 43;
	if (oscilation_etape >= 22) {
		y -= tir_snorky_oscilation[oscilation_etape - 22];
	} else {
		y += tir_snorky_oscilation[oscilation_etape];
	}
	anim_tick++;
	anim_tick %= 4;
	if (anim_tick == 0) {
		anim_step++;
		anim_step %= 3;
	}

	if (dir == FACING_RIGHT) {
		pic = pbk_enemies[32 + anim_step];
	} else {
		pic = pbk_enemies[35 + anim_step];
	}

	if (solid_wall(x, y) != 0) destroyed = true;

	colFromPic();
}
