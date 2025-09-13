#include "entities/shots/shot_snorky_governor.h"

ShotSnorkyGovernor::ShotSnorkyGovernor(Character* target, int vx, int vy) : target_player(target), dx(vx), dx_tick(0), dy_tick(0), time(0) { dy = vy; }

void ShotSnorkyGovernor::update() {
	anim_tick++;
	anim_tick %= 4;

	if (anim_tick == 0) {
		anim_step++;
		anim_step %= 4;
	}

	if (x < offset - 100 || x > offset + 740 || y > 580 || y < -100) destroyed = true;

	time++;

	if ((time < TIME_MAX || ((dx < 2 && dx > -2) && (dy < 2 && dy > -2))) && (target_player->destroyed == false)) {
		if (x < target_player->x && dx < SPEED_MAX) {
			dx_tick += 1;

			if (dx_tick > SHOT_INERTIA) {
				dx += 1;
				dx_tick = 0;
			}
		}

		else if (x > target_player->x && dx > (-SPEED_MAX)) {
			dx_tick -= 1;

			if (dx_tick < (-SHOT_INERTIA)) {
				dx -= 1;
				dx_tick = 0;
			}
		}

		if (y < target_player->y && dy < SPEED_MAX) {
			dy_tick += 1;

			if (dy_tick > SHOT_INERTIA) {
				dy += 1;
				dy_tick = 0;
			}
		}

		else if (y > target_player->y && dy > (-SPEED_MAX)) {
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
		pic = pbk_enemies[233 + anim_step];
	} else {
		pic = pbk_enemies[236 - anim_step];
	}

	if (solid_wall(x + dx, y + dy) != 0) destroyed = true;

	colFromPic();
}
