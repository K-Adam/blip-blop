#include "entities/shots/shot_shell.h"
#include "core/debug.h"

ShotShell::ShotShell() {
	pic = pbk_enemies[58];
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
	pic = pbk_enemies[58 + anim_step];

	if (solid_wall(x - SHELL_SPEED, y)) {
		if (bounced) {
			destroyed = true;
		} else {
			dir = FACING_RIGHT;
			bounced = true;
		}
	} else if (solid_wall(x + SHELL_SPEED, y)) {
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
	if (x < offset - 100 || x > offset + 700 || y > 520 || y < -50) destroyed = true;

	colFromPic();
}
