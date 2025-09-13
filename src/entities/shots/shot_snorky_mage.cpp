#include "entities/shots/shot_snorky_mage.h"

ShotSnorkyMage::ShotSnorkyMage() { dy = 6; }

void ShotSnorkyMage::update() {
	anim_tick++;
	anim_tick %= 4;

	if ((anim_tick == 0) && (anim_step < 3)) anim_step++;

	if (anim_step == 3) anim_step = 1;

	if ((anim_step == 0) && (anim_tick == 1)) {
		if (dir == FACING_RIGHT)
			dx = 6;

		else
			dx = -6;
	}

	if (x < offset - 100 || x > offset + 700 || y > 520 || y < -50) destroyed = true;

	if (solid_wall(x + dx, y + dy)) destroyed = true;

	if (solid_wall(x + 3 * dx, y + 3 * dy)) anim_step = 3;

	x += dx;
	y += dy;

	if (dir == FACING_RIGHT) {
		pic = pbk_enemies[252 + anim_step];
	} else {
		pic = pbk_enemies[261 + anim_step];
	}

	colFromPic();
}
