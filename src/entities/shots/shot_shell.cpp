#include "entities/shots/shot_shell.h"
#include "core/debug.h"

ShotShell::ShotShell() {
	pic = assets().pics.enemies.anim("shell_spin")[0];
	bounced = false;
}

void ShotShell::update() {
	fall();

	anim_tick++;
	anim_tick %= 3;

	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 4;
	}
	pic = assets().pics.enemies.anim("shell_spin")[anim_step];

	if (level().terrain.solid_wall(x - SHELL_SPEED, y)) {
		if (bounced) {
			destroyed = true;
		} else {
			dir = FACING_RIGHT;
			bounced = true;
		}
	} else if (level().terrain.solid_wall(x + SHELL_SPEED, y)) {
		if (bounced) {
			destroyed = true;
		} else {
			dir = FACING_LEFT;
			bounced = true;
		}
	}

	if (dir == FACING_LEFT) {
		x -= SHELL_SPEED;
	} else {
		x += SHELL_SPEED;
	}
	if (x < level().camera.offset - 100 || x > level().camera.offset + 700 || y > 520 || y < -50) destroyed = true;

	colFromPic();
}
