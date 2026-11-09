#include "entities/shots/shot_snorky_governor.h"

ShotSnorkyGovernor::ShotSnorkyGovernor(Character* target, int vx, int vy) : homing_target(target), dx(vx), dx_tick(0), dy_tick(0), time(0) { dy = vy; }

void ShotSnorkyGovernor::update() {
	anim_tick++;
	anim_tick %= 4;

	if (anim_tick == 0) {
		anim_step++;
		anim_step %= 4;
	}

	if (x < offset - 100 || x > offset + 740 || y > 580 || y < -100) destroyed = true;

	time++;

	if ((time < TIME_MAX || ((dx < 2 && dx > -2) && (dy < 2 && dy > -2))) && (homing_target->destroyed == false)) {
		if (x < homing_target->x && dx < SPEED_MAX) {
			dx_tick += 1;

			if (dx_tick > SHOT_INERTIA) {
				dx += 1;
				dx_tick = 0;
			}
		}

		else if (x > homing_target->x && dx > (-SPEED_MAX)) {
			dx_tick -= 1;

			if (dx_tick < (-SHOT_INERTIA)) {
				dx -= 1;
				dx_tick = 0;
			}
		}

		if (y < homing_target->y && dy < SPEED_MAX) {
			dy_tick += 1;

			if (dy_tick > SHOT_INERTIA) {
				dy += 1;
				dy_tick = 0;
			}
		}

		else if (y > homing_target->y && dy > (-SPEED_MAX)) {
			dy_tick -= 1;

			if (dy_tick < (-SHOT_INERTIA)) {
				dy -= 1;
				dy_tick = 0;
			}
		}
	}

	x += dx;
	y += dy;

	if (dx <= 0) {
		pic = pbk_enemies.anim("governor_shot")[anim_step];
	} else {
		pic = pbk_enemies.anim("governor_shot")[3 - anim_step];
	}

	if (solid_wall(x + dx, y + dy) != 0) destroyed = true;

	colFromPic();
}
