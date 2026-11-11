#include "entities/shots/shot_snorky_base.h"

const int shot_snorky_oscillation[] = {0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 2, 2, 2, 1, 1, 1, 0, 0, 0};

ShotSnorkyBase::ShotSnorkyBase(int vx) : oscillation_step(0), accel_tick(0) {
	dy = 0;
	dx = vx;
}

void ShotSnorkyBase::update() {
	if (x < level().camera.offset - 100 || x > level().camera.offset + 700 || y > 520 || y < -50) destroyed = true;

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

	oscillation_step++;
	oscillation_step %= 43;
	if (oscillation_step >= 22) {
		y -= shot_snorky_oscillation[oscillation_step - 22];
	} else {
		y += shot_snorky_oscillation[oscillation_step];
	}
	anim_tick++;
	anim_tick %= 4;
	if (anim_tick == 0) {
		anim_step++;
		anim_step %= 3;
	}

	pic = facing(assets().pics.enemies, "snorky_shot")[anim_step];

	if (level().terrain.solid_wall(x, y) != 0) destroyed = true;

	colFromPic();
}
