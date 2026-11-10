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

	if (x < level().camera.offset - 100 || x > level().camera.offset + 700 || y > 520 || y < -50) destroyed = true;

	if (level().terrain.solid_wall(x + dx, y + dy)) destroyed = true;

	if (level().terrain.solid_wall(x + 3 * dx, y + 3 * dy)) anim_step = 3;

	x += dx;
	y += dy;

	if (dir == FACING_RIGHT) {
		pic = assets().pics.enemies.anim("mage_shot_right")[anim_step];
	} else {
		pic = assets().pics.enemies.anim("mage_shot_left")[anim_step];
	}

	colFromPic();
}
