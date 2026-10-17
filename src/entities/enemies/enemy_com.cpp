#include "entities/enemies/enemy_com.h"

EnemyCOM::EnemyCOM() { hp = 100; }

void EnemyCOM::update() {
	switch (status) {
		case STATE_NORMAL:
		case STATE_WALK:
			onWalk();
			break;

		case STATE_DYING:
		case STATE_CHARRED:
			onDie();
			break;
	}

	updateDestroyed();
}

void EnemyCOM::onWalk() {
	anim_tick += 1;
	anim_tick %= 8;

	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 2;
	}

	if (x - 1 < offset || solid_wall(x - 1, y))
		dir = FACING_RIGHT;
	else if (x + 1 > offset + 640 || solid_wall(x + 1, y))
		dir = FACING_LEFT;

	if (dir == FACING_RIGHT)
		x += 1;
	else
		x -= 1;

	pic = pbk_enemies.anim("com_fly")[anim_step];

	colFromPic();
}

void EnemyCOM::onDie() {
	if (anim_tick == 0 && anim_step == 0) {
		sbk_level.play(6);
	}

	anim_tick += 1;
	anim_tick %= 3;

	if (anim_tick == 0 && anim_step < 22) anim_step += 1;

	if (anim_step >= 22) {
		destroyed = true;
	} else {
		pic = pbk_enemies.anim("com_die")[anim_step];
	}
}
